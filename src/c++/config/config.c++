//
// Created by user on 07.01.2024.
//

#include <filesystem>
#include <fstream>
#include <sstream>
#include <config/config.h>
#include <leafconf/serializers/toml.h>

using namespace std;

namespace config
{
  Config::Config(const ValueChangedCB& cb, const CreateDefaultConfigFunction& cdcf)
    : m_value_changed_cb(cb)
    , m_create_default_config_function(cdcf)
    , m_root(make_unique<YAML::Node>())
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

    call_once(of, [file_path]() { set_permissions(file_path); });

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
    call_once(of, [path]() { set_permissions(path); });

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

  auto ConfigData::serialize(const leaf::serialization::Serializer serializer) const -> expected<string, string>
  {
    const auto out = toml::table{
      {"network", toml::table{
        {"ipv4", toml::table{
          {"de10", this->network.ipv4.de10},
          {"jetson", this->network.ipv4.jetson},
          {"power_switch", this->network.ipv4.power_switch}
        }},
        {"ports", toml::table{
          {"power_switch", this->network.ports.power_switch}
        }}
      }}
    };

    stringstream ss;
    switch(serializer)
    {
      case leaf::serialization::TOML: ss << out; break;
      case leaf::serialization::JSON: ss << toml::json_formatter(out); break;
      case leaf::serialization::YAML: ss << toml::yaml_formatter(out); break;
      default: leaf::unreachable();
    }
    return ss.str();
  }

  auto ConfigData::deserialize(const string_view data, const leaf::serialization::Serializer serializer) -> expected<void, string>
  {
    if(serializer != leaf::serialization::Serializer::TOML)
      return leaf::Err("unsupported deserialization format");
    toml::table in;
    try {
      in = toml::parse(data);
    } catch(const toml::parse_error& err) {
      return leaf::Err(err.what());
    }
    try {
      this->network.ipv4.de10 = in["network"]["ipv4"]["de10"].value<string>().value();
      this->network.ipv4.jetson = in["network"]["ipv4"]["jetson"].value<string>().value();
      this->network.ipv4.power_switch = in["network"]["ipv4"]["power_switch"].value<string>().value();

      this->network.ports.power_switch = in["network"]["ports"]["power_switch"].value<u16>().value();
    } catch(const bad_optional_access& err) {
      return leaf::Err(err.what());
    }
    return {};
  }
} // config
