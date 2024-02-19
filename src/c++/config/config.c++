//
// Created by user on 07.01.2024.
//

#include <filesystem>
#include <fstream>
#include <config/config.h>

using namespace std;

namespace config
{
  Config::Config(const ValueChangedCB& cb, const CreateDefaultConfigFunction& cdcf)
    : m_value_changed_cb(cb),
      m_create_default_config_function(cdcf),
      m_root(make_unique<YAML::Node>())
  {
    // this->load();
  }

  Config::~Config() = default;

  once_flag of;
  auto set_permissions(const string_view file_path) -> void
  {
    filesystem::permissions(file_path, filesystem::perms::owner_read | filesystem::perms::owner_write);
  }

  auto Config::load() const -> void
  {
    const auto file_path = fmt::format("{}/{}/{}",
      filesystem::current_path().string(),
      CONFIG_DIRECTORY_NAME,
      CONFIG_FILENAME
    );

    if(not filesystem::exists(file_path))
    {
      llog::debug("config file is missing from {}", file_path);
      llog::trace("calling create default config function");
      filesystem::create_directories(filesystem::path(file_path).parent_path());
      if(auto res = this->m_create_default_config_function(file_path))
        llog::debug("default config created");
      else
        llog::error("failed to create default config");
    }
    else
      llog::debug("found existing config file at {}", file_path);

    call_once(of, [file_path](){ set_permissions(file_path); });

    ifstream stream(file_path);
    const string contents((istreambuf_iterator(stream)), istreambuf_iterator<char>());
    *(this->m_root) = Clone(YAML::Load(contents));
    llog::debug("config loaded");
  }

  auto Config::save() const -> void
  {
    llog::debug("saving config file");
    const auto path = fmt::format("{}/{}/{}",
      filesystem::current_path().string(),
      CONFIG_DIRECTORY_NAME,
      CONFIG_FILENAME
    );

    filesystem::create_directories(filesystem::path(path).parent_path());
    call_once(of, [path](){ set_permissions(path); });

    if(filesystem::exists(path))
      filesystem::remove(path);

    ofstream stream(path);
    stream << YAML::Dump(*(this->m_root));
    stream.close();
  }

  auto Config::get_node_unchecked(const string_view category, const string_view key) const -> YAML::Node
  {
    return (*this->m_root)[category][key];
  }

  auto Config::get_node(const string_view category, const string_view key) const -> expected<YAML::Node, string>
  {
    try { return this->get_node_unchecked(category, key); }
    catch(const exception& e) { return leaf::Err(e.what()); }
  }
} // config