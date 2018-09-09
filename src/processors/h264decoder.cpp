#include "h264decoder.hpp"


namespace shar {

H264Decoder::H264Decoder(Processor::Context context)
    : Processor(std::move(context))
    , m_decoder(Processor::context().logger()) {}

void H264Decoder::operator()(Packet packet, channel::Sender<Image>& output) {
  auto frame = m_decoder.decode(std::move(packet));
  if (!frame.empty()) {
    output.send(std::move(frame));
  }
}

}