#include <qdebugenv/rhi/class_renderer.h>

#include <cassert>
#include <qfile.h>
#include <qevent.h>
#include <qimage.h>
#include <floppy/logging.h>
#include <qdebugenv/vendored/imgui.h>

namespace llog = ::floppy::log;
namespace checks
{
  static_assert(sizeof(::ImDrawVert) == 20, "sizeof(ImDrawVert) must be 20 bytes");
  static_assert(sizeof(ImDrawIdx) == 4, "sizeof(ImDrawIdx) must be 32 bits");
} // namespace checks

namespace
{
  [[nodiscard]] auto shader(std::string_view name) -> ::QShader {
    ::QFile file(name.data());
    if(file.open(::QIODevice::ReadOnly))
      return ::QShader::fromSerialized(file.readAll());
    llog::error("failed to load shader: {}", name);
    return {};
  }
} // namespace

namespace qdebugenv
{
  CRhiRenderer::CRhiRenderer() = default;
  CRhiRenderer::~CRhiRenderer() {
    this->release_resources();
  }

  auto CRhiRenderer::prepare(
    ::QRhi* rhi,
    ::QRhiRenderTarget* rt,
    ::QRhiCommandBuffer* cb,
    QMatrix4x4 const& mvp,
    f32 const opacity,
    f32 const hdr_white_level
  ) -> void
  {
    if(not this->rhi_)
      this->rhi_ = rhi;
    else if(this->rhi_ != rhi) {
      this->release_resources();
      this->rhi_ = rhi;
    }
    if(not this->rhi_ or this->f.draw.isEmpty())
      return;
    this->rt_ = rt;
    this->cb_ = cb;
    if(not this->vbuf_) {
      this->vbuf_.reset(this->rhi_->newBuffer(::QRhiBuffer::Dynamic, ::QRhiBuffer::VertexBuffer, this->f.total_vbuf_size));
      this->vbuf_->setName(QByteArrayLiteral("imgui vertex buffer"));
      if(not this->vbuf_->create())
        return;
    } else {
      if(this->f.total_vbuf_size > this->vbuf_->size()) {
        this->vbuf_->setSize(this->f.total_vbuf_size);
        if(not this->vbuf_->create())
          return;
      }
    }
    if(not this->ibuf_) {
      this->ibuf_.reset(this->rhi_->newBuffer(::QRhiBuffer::Dynamic, ::QRhiBuffer::IndexBuffer, this->f.total_ibuf_size));
      this->ibuf_->setName(QByteArrayLiteral("imgui index buffer"));
      if(not this->ibuf_->create())
        return;
    } else {
      if(this->f.total_ibuf_size > this->ibuf_->size()) {
        this->ibuf_->setSize(this->f.total_ibuf_size);
        if(not this->ibuf_->create())
          return;
      }
    }
    if(not this->ubuf_) {
      this->ubuf_.reset(this->rhi_->newBuffer(::QRhiBuffer::Dynamic, ::QRhiBuffer::UniformBuffer, 64 + 4 + 4));
      this->ubuf_->setName(QByteArrayLiteral("imgui uniform buffer"));
      if(not this->ubuf_->create())
        return;
    }
    if(not this->linear_sampler_) {
      this->linear_sampler_.reset(this->rhi_->newSampler(
        ::QRhiSampler::Linear,
        ::QRhiSampler::Linear,
        ::QRhiSampler::None,
        ::QRhiSampler::Repeat,
        ::QRhiSampler::Repeat
      ));
      this->linear_sampler_->setName(QByteArrayLiteral("imgui linear sampler"));
      if(not this->linear_sampler_->create())
        return;
    }
    if(not this->nearest_sampler_) {
      this->nearest_sampler_.reset(this->rhi_->newSampler(
        ::QRhiSampler::Nearest,
        ::QRhiSampler::Nearest,
        ::QRhiSampler::None,
        ::QRhiSampler::Repeat,
        ::QRhiSampler::Repeat
      ));
      this->nearest_sampler_->setName(QByteArrayLiteral("imgui nearest sampler"));
      if(not this->nearest_sampler_->create())
        return;
    }
    if(this->textures_.empty()) {
      auto const font_tex = Texture { .image = this->sf.font_texture_data };
      this->textures_[nullptr] = font_tex;
      this->sf.reset();
    } else if(this->sf.valid()) {
      auto const font_tex = Texture { .image = this->sf.font_texture_data };
      auto& font_tex_entry = this->textures_[nullptr];
      delete font_tex_entry.tex; // NOLINT(*-owning-memory)
      delete font_tex_entry.srb; // NOLINT(*-owning-memory)
      font_tex_entry = font_tex;
      this->sf.reset();
    }
    auto textures_need_update = ::QVarLengthArray<void*, 8>();
    for(auto& [id, tex] : this->textures_) {
      if(not tex.tex) {
        tex.tex = this->rhi_->newTexture(::QRhiTexture::RGBA8, tex.image.size());
        tex.tex->setName(QByteArrayLiteral("imgui texture ") + ::QByteArray::number(reinterpret_cast<uintptr_t>(id))); // NOLINT(*-pro-type-reinterpret-cast)
        if(not tex.tex->create())
          return;
        textures_need_update.push_back(id);
      }
      if(not tex.srb) {
        auto* sampler = tex.filter == ::QRhiSampler::Filter::Nearest
          ? this->nearest_sampler_.get()
          : this->linear_sampler_.get();
        tex.srb = this->rhi_->newShaderResourceBindings();
        tex.srb->setBindings({
          ::QRhiShaderResourceBinding::uniformBuffer(
            0,
            ::QRhiShaderResourceBinding::VertexStage | ::QRhiShaderResourceBinding::FragmentStage,
            this->ubuf_.get()
          ),
          ::QRhiShaderResourceBinding::sampledTexture(
            1,
            ::QRhiShaderResourceBinding::FragmentStage,
            tex.tex,
            sampler
          )
        });
        if(not tex.srb->create())
          return;
      }
    }
    auto render_pass_format_qvec = ::QVector<u32>(this->render_pass_format_.begin(), this->render_pass_format_.end());
    if(this->ps_ and this->rt_->renderPassDescriptor()->serializedFormat() != render_pass_format_qvec)
      this->ps_.reset();
    if(this->ps_ and this->rt_->sampleCount() != this->ps_->sampleCount())
      this->ps_.reset();
    if(not this->ps_) {
      auto vs = ::shader(":/shaders/imgui.vert.qsb");
      auto fs = ::shader(":/shaders/imgui.frag.qsb");
      if(not vs.isValid() or not fs.isValid())
        return;
      this->ps_.reset(this->rhi_->newGraphicsPipeline());
      auto blend = ::QRhiGraphicsPipeline::TargetBlend {
        .enable = true,
        .srcColor = ::QRhiGraphicsPipeline::One,
        .dstColor = ::QRhiGraphicsPipeline::OneMinusSrcAlpha,
        .srcAlpha = ::QRhiGraphicsPipeline::One,
        .dstAlpha = ::QRhiGraphicsPipeline::OneMinusSrcAlpha
      };
      this->ps_->setTargetBlends({ blend });
      this->ps_->setCullMode(::QRhiGraphicsPipeline::None);
      this->ps_->setDepthTest(true);
      this->ps_->setDepthOp(::QRhiGraphicsPipeline::LessOrEqual);
      this->ps_->setDepthWrite(false);
      this->ps_->setFlags(::QRhiGraphicsPipeline::UsesScissor);
      this->ps_->setShaderStages({
        { ::QRhiShaderStage::Vertex, vs },
        { ::QRhiShaderStage::Fragment, fs }
      });

      auto input_layout = ::QRhiVertexInputLayout();
      input_layout.setBindings({{ 4 * sizeof(f32) + sizeof(u32) }});
      input_layout.setAttributes({
        { 0, 0, ::QRhiVertexInputAttribute::Float2, 0 },
        { 0, 1, ::QRhiVertexInputAttribute::Float2, 2 * sizeof(f32) },
        { 0, 2, ::QRhiVertexInputAttribute::UNormByte4, 4 * sizeof(f32) }
      });
      this->ps_->setVertexInputLayout(input_layout);
      this->ps_->setSampleCount(rt->sampleCount());
      this->ps_->setShaderResourceBindings(this->textures_[nullptr].srb);
      this->ps_->setRenderPassDescriptor(this->rt_->renderPassDescriptor());
      render_pass_format_qvec = this->rt_->renderPassDescriptor()->serializedFormat();
      this->render_pass_format_ = { render_pass_format_qvec.begin(), render_pass_format_qvec.end() };
      if(not this->ps_->create())
        return;
    }
    auto* u = this->rhi_->nextResourceUpdateBatch();
    for(auto const& b : this->f.vbuf)
      u->updateDynamicBuffer(this->vbuf_.get(), b.offset, b.data.size(), b.data.constData());
    for(auto const& b : this->f.ibuf)
      u->updateDynamicBuffer(this->ibuf_.get(), b.offset, b.data.size(), b.data.constData());
    u->updateDynamicBuffer(this->ubuf_.get(), 0, 64, mvp.constData());
    u->updateDynamicBuffer(this->ubuf_.get(), 64, 4, &opacity);
    u->updateDynamicBuffer(this->ubuf_.get(), 68, 4, &hdr_white_level);
    for(auto i = 0; i < textures_need_update.count(); ++i) {
      auto& t = this->textures_[textures_need_update[i]];
      if(not t.image.isNull()) {
        u->uploadTexture(t.tex, t.image);
        t.image = ::QImage();
      }
    }
    this->cb_->resourceUpdate(u);
  }

  auto CRhiRenderer::render() -> void
  {
    if(not this->rhi_ or this->f.draw.isEmpty() or not this->ps_)
      return;
    this->cb_->setGraphicsPipeline(this->ps_.get());

    auto const viewport_size = this->rt_->pixelSize();
    auto needs_viewport = true;

    for(auto const& c : this->f.draw) {
      auto vbuf_binding = ::QRhiCommandBuffer::VertexInput(
        this->vbuf_.get(),
        this->f.vbuf[c.command_list_buffer_index].offset
      );
      if(needs_viewport) {
        needs_viewport = false;
        this->cb_->setViewport({
          0,
          0,
          static_cast<f32>(viewport_size.width()),
          static_cast<f32>(viewport_size.height())
        });
      }
      auto const sx1 = static_cast<f32>(c.clip_rect.x() + c.item_pixel_offset.x());
      auto const sy1 = static_cast<f32>(c.clip_rect.y() + c.item_pixel_offset.y());
      auto const sx2 = static_cast<f32>(c.clip_rect.z() + c.item_pixel_offset.x());
      auto const sy2 = static_cast<f32>(c.clip_rect.w() + c.item_pixel_offset.y());
      auto scissor_pos = ::QPointF(sx1, static_cast<f32>(viewport_size.height()) - sy2).toPoint();
      auto scissor_size = ::QSizeF(sx2 - sx1, sy2 - sy1).toSize();
      scissor_pos.setX(std::max(0, scissor_pos.x()));
      scissor_pos.setY(std::max(0, scissor_pos.y()));
      scissor_size.setWidth(std::min(viewport_size.width(), scissor_size.width()));
      scissor_size.setHeight(std::min(viewport_size.height(), scissor_size.height()));
      this->cb_->setScissor({
        scissor_pos.x(),
        scissor_pos.y(),
        scissor_size.width(),
        scissor_size.height()
      });
      this->cb_->setShaderResources(this->textures_[c.texture_id].srb);
      this->cb_->setVertexInput(
        0,
        1,
        &vbuf_binding,
        this->ibuf_.get(),
        c.index_offset,
        ::QRhiCommandBuffer::IndexUInt32
      );
      this->cb_->drawIndexed(c.element_count);
    }
  }

  auto CRhiRenderer::release_resources() -> void {
    for(auto& [id, tex] : this->textures_) {
      if(tex.own_texture)
        delete tex.tex; // NOLINT(*-owning-memory)
      delete tex.srb; // NOLINT(*-owning-memory)
    }
    this->textures_.clear();
    this->vbuf_.reset();
    this->ibuf_.reset();
    this->ubuf_.reset();
    this->ps_.reset();
    this->linear_sampler_.reset();
    this->nearest_sampler_.reset();
    this->rhi_ = nullptr;
  }

  auto CRhiRenderer::register_custom_texture(
    void* id,
    ::QRhiTexture* texture,
    ::QRhiSampler::Filter filter,
    CRhiRenderer::CustomTextureOwnership ownership
  ) -> void
  {
    assert(id and "id must not be nullptr");
    if(this->textures_.contains(id)) {
      if(this->textures_[id].own_texture)
        delete this->textures_[id].tex; // NOLINT(*-owning-memory)
      delete this->textures_[id].srb; // NOLINT(*-owning-memory)
    }
    auto const tex = Texture {
      .image = ::QImage(),
      .tex = texture,
      .srb = nullptr,
      .filter = filter,
      .own_texture = (ownership == CRhiRenderer::CustomTextureOwnership::Take)
    };
    this->textures_[id] = tex;
  }
} // namespace qdebugenv