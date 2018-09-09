#pragma once

#include <array>
#include <vector>
#include <cstdint>

#include "disable_warnings_push.hpp"
#include <boost/asio.hpp>
#include "disable_warnings_pop.hpp"

#include "packet.hpp"
#include "processors/processor.hpp"
#include "channels/bounded.hpp"
#include "channels/default.hpp"


namespace shar {

using IpAddress = boost::asio::ip::address;

class PacketReceiver {
public:
  using Sender = channel::Sender<Packet>;
  using Receiver = channel::DefaultReceiver<bool>;

  struct Context {
    IpAddress ip;
  };

  PacketReceiver(
      Context context,
      Processor& processor,
      Sender sender
  );
  ~PacketReceiver();

  void operator()(bool);

private:
  void start_read();

  using Buffer = std::vector<std::uint8_t>;

  struct PacketReader {
    enum class State {
      ReadingLength,
      ReadingContent
    };

    PacketReader();

    std::vector<Packet> update(const Buffer& buffer, std::size_t size);

    State       m_state;
    std::size_t m_packet_size;
    std::size_t m_remaining;
    Buffer      m_buffer;
  };

  using IOContext = boost::asio::io_context;
  using Socket = boost::asio::ip::tcp::socket;

  PacketReader m_reader;
  Buffer       m_buffer;
  IpAddress    m_server_address;
  IOContext    m_context;
  Socket       m_receiver;

  Sender   m_sender;
  Processor& m_processor;
};

}