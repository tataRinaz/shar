#pragma once

#include "channels/bounded.hpp"
#include "packet.hpp"
#include "primitives/image.hpp"
#include "processors/processor.hpp"
#include "codecs/ffmpeg/decoder.hpp"


namespace shar {

class H264Decoder : public Processor {
public:
  struct Context{};
  using Sender = channel::Sender<Image>;
  using Receiver = channel::Receiver<Packet>;

  H264Decoder(Processor::Context context);

  void operator()(Packet packet, channel::Sender<Image>& output);

private:
  codecs::ffmpeg::Decoder m_decoder;
};

}