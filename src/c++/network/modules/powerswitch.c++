#include <vector>
#include <QtCore/QTimer>
#include <QtNetwork/QUdpSocket>
#include <network/localaddr.h>
#include <network/modules/powerswitch.h>

using std::vector;
using std::make_unique;

namespace network::modules
{
  constexpr auto REQUEST_CHANNEL = 9'999;

  PowerSwitch::PowerSwitch(const string_view ipv4, const u16 port)
    : m_server(make_unique<QUdpSocket>(nullptr)),
      m_client(make_unique<QUdpSocket>(nullptr)),
      m_timer(make_unique<QTimer>(nullptr)),
      m_host_address(QHostAddress()),
      m_port(std::numeric_limits<u16>::max())
  {
    this->configure(ipv4, port);
  }

  PowerSwitch::~PowerSwitch() = default;

  auto PowerSwitch::toggle_channel(const int channel) const -> void
  {
    // todo: checks

    const auto request = RequestPacket {
      .marker = REQUEST_MARKER,
      .channel = static_cast<u16>(channel),
      .port = CLIENT_PORT,
      .checksum = 0x0000
    };

    this->m_client->writeDatagram(
      reinterpret_cast<const char*>(&request),
      sizeof(request),
      this->m_host_address,
      this->m_port
    );

    llog::info("toggling {} channel", channel);
  }

  auto PowerSwitch::configure(string_view ipv4, u16 port) -> void
  {
    if(const auto a = Ipv4Address::local_address(); not a.has_value())
    {
      llog::error("failed to obtain local ipv4 address, reason: {}", a.error());
      return;
    }
    this->m_server->close();
    this->m_server->bind(QHostAddress(Ipv4Address::local_address().value().ip.c_str()), SERVER_PORT);

    this->m_client->close();
    this->m_client->bind(QHostAddress(Ipv4Address::local_address().value().ip.c_str()), CLIENT_PORT);

    this->m_timer->stop();
    this->m_timer->start(REQUEST_INTERVAL);
    QObject::connect(
      this->m_timer.get(),
      &QTimer::timeout,
      this,
      &PowerSwitch::request
    );

    QObject::connect(
      this->m_client.get(),
      &QUdpSocket::readyRead,
      this,
      &PowerSwitch::read
    );

    this->m_host_address = QHostAddress(QString::fromStdString(string(ipv4)));
    this->m_port = port;

    llog::debug("powerswitch server started at {}:{}", Ipv4Address::local_address().value().ip, SERVER_PORT);
    llog::debug("powerswitch client started at {}:{}", Ipv4Address::local_address().value().ip, CLIENT_PORT);
    llog::debug("powerswitch client state: {}", this->m_client->isOpen() ? "OK" : "CLOSED");
    llog::debug("powerswitch target: {}:{}", ipv4, port);
    llog::debug("powerswitch timer started with interval {}", REQUEST_INTERVAL);

    this->request();
  }

  auto PowerSwitch::request() const -> void {
    this->toggle_channel(REQUEST_CHANNEL);
    llog::trace("auto request sent ({} s)", REQUEST_INTERVAL / 1'000);
  }

  auto PowerSwitch::read() -> void
  {
    while(this->m_client->hasPendingDatagrams())
    {
      QByteArray data;
      data.resize(this->m_client->pendingDatagramSize());
      this->m_client->readDatagram(data.data(), data.size());
      this->handle_response(data);
    }
  }

  auto PowerSwitch::handle_response(const QByteArray& data) -> void
  {
    if(data.size() < sizeof(ResponsePacket))
      return;

    constexpr auto CHUNKS_SIZE = 12;

    const auto raw = reinterpret_cast<const array<ResponsePacket, 12>*>(data.data());
    vector<ResponsePacket> packets = {raw->begin(), raw->end()};
    packets.erase(packets.end() - 4, packets.end());
    // for(const auto& [header, channel, enabled, voltage, current] : packets)
    //   llog::trace("packet received: {} : {} mV/{} mA", channel, voltage, current);
  }
}
