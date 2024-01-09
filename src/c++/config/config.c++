//
// Created by user on 07.01.2024.
//

#include "config.h"
#include <filesystem>
#include <fstream>

using namespace std;

namespace config
{
  Config::Config(ValueChangedCB cb, CreateDefaultConfigFunction cdcf)
    : m_value_changed_cb(cb),
      m_create_default_config_function(cdcf),
      m_root(make_unique<YAML::Node>())
  {
    // this->load();
  }

  Config::~Config() = default;

  once_flag of;
  void set_permissions(string_view file_path)
  {
    filesystem::permissions(file_path, filesystem::perms::owner_read | filesystem::perms::owner_write);
  }

  void Config::load() const
  {
    const auto file_path = fmt::format("{}/{}/{}",
      filesystem::current_path().string(),
      CONFIG_DIRECTORY_NAME,
      CONFIG_FILENAME
    );

    if(not filesystem::exists(file_path))
    {
      lldebug("config file is missing from {}", file_path);
      lltrace("calling create default config function");
      if(auto res = this->m_create_default_config_function(file_path))
        lldebug("default config created");
      else
        llerror("failed to create default config");
    }
    else
      lldebug("found existing config file at {}", file_path);

    call_once(of, [file_path](){ set_permissions(file_path); });

    ifstream stream(file_path);
    const string contents((istreambuf_iterator(stream)), istreambuf_iterator<char>());
    *(this->m_root) = Clone(YAML::Load(contents));
    lldebug("config loaded");
  }

  void Config::save() const
  {
    lldebug("saving config file");
    const auto path = fmt::format("{}/{}/{}",
      filesystem::current_path().string(),
      CONFIG_DIRECTORY_NAME,
      CONFIG_FILENAME
    );

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
} // config