#pragma once

#include <afx>
#include <functional>
#include <memory>
#include <string>
#include <yaml-cpp/yaml.h>

namespace config
{
  using std::unique_ptr;
  using std::string;
  using std::string_view;

  constexpr string_view CONFIG_DIRECTORY_NAME = "config";
  constexpr string_view CONFIG_FILENAME = "config.yaml";

  class Config final
  {
    public:
      using ValueChangedCB = std::function<void (string, string)>;
      using CreateDefaultConfigFunction = std::function<bool (string_view)>;

      explicit Config(ValueChangedCB, CreateDefaultConfigFunction);
      ~Config();

      void load() const;
      void save() const;

      [[nodiscard]] auto get_node_unchecked(string_view category, string_view key) const -> YAML::Node;

      template<typename T>
      [[nodiscard]] auto get_unchecked(string_view category, string_view key) const -> T;

    private:
      ValueChangedCB m_value_changed_cb;
      CreateDefaultConfigFunction m_create_default_config_function;
      unique_ptr<YAML::Node> m_root;
  };

  template<typename T> auto Config::get_unchecked(const string_view category, const string_view key) const -> T
  {
    return this->get_node_unchecked(category, key).as<T>();
  }
}
