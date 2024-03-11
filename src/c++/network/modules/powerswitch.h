#pragma once

#include <array>
#include <string>
#include <memory>
#include <leaf/global.h>
#include <QtCore/QByteArray>
#include <QtNetwork/QHostAddress>

class QTimer;
class QUdpSocket;
namespace network::modules
{
  using std::array;
  using std::string;
  using std::string_view;
  using std::unique_ptr;
  using namespace leaf::types;

  // ReSharper disable once CppClassCanBeFinal
  class PowerSwitch : public QObject
  {
    Q_OBJECT

    constexpr static u32 REQUEST_MARKER = 0xAAAAAAAA;
    constexpr static u16 CLIENT_PORT = 11444;
    constexpr static u16 SERVER_PORT = CLIENT_PORT + 0x01;
    constexpr static u32 REQUEST_INTERVAL = 30'000;

    struct [[gnu::packed]] RequestPacket
    {
      u32 marker;
      u16 channel;
      u16 port;
      u16 checksum;
    };

    struct [[gnu::packed]] ResponsePacket
    {
      u16 header;
      u8 channel;
      u8 enabled;
      u32 voltage;
      u32 current;
    };

    struct ChannelData
    {
      f32 voltage; // V
      f32 current; // mA
      bool enabled;
    };

    public:
      explicit PowerSwitch(string_view ipv4, u16 port);
      virtual ~PowerSwitch() override;

      [[nodiscard]] auto channel_data(int channel) -> ChannelData;
      Q_INVOKABLE void toggle_channel(int channel) const;

    private:
      auto configure(string_view ipv4, u16 port) -> void;
      Q_SLOT void request() const;
      Q_SLOT void read();
      auto handle_response(const QByteArray& data) -> void;

    private:
      unique_ptr<QUdpSocket> m_server;
      unique_ptr<QUdpSocket> m_client;
      unique_ptr<QTimer> m_timer;
      QHostAddress m_host_address;
      u16 m_port;
      array<ChannelData, 8> m_channels{};
  };
}