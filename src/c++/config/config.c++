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

  void Config::load() const
  {
    const auto file_path = fmt::format("{}/{}/{}",
      filesystem::current_path().string(),
      CONFIG_DIRECTORY_NAME,
      CONFIG_FILENAME
    );

    if(not filesystem::exists(file_path))
    {
      logging::debug("config file is missing from {}", file_path);
      logging::trace("calling create default config function");
      if(auto res = this->m_create_default_config_function(file_path))
        logging::debug("default config created");
      else
        logging::error("failed to create default config");
    }
    else
      logging::debug("found existing config file at {}", file_path);

    ifstream stream(file_path);
    const string contents((istreambuf_iterator(stream)), istreambuf_iterator<char>());
    *(this->m_root) = Clone(YAML::Load(contents));
    logging::debug("config loaded");
  }

  void Config::save() const
  {
    const auto path = fmt::format("{}/{}/{}",
      filesystem::current_path().string(),
      CONFIG_DIRECTORY_NAME,
      CONFIG_FILENAME
    );

    ofstream stream(path);
    stream << YAML::Dump(*(this->m_root));
    stream.close();
  }

  auto Config::get_node_unchecked(const string_view category, const string_view key) const -> YAML::Node
  {
    return (*this->m_root)[category][key];
  }
} // config