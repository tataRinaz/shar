#pragma once

#include <fstream>
#include <string>

#include "primitives/size.hpp"
#include "primitives/image.hpp"
#include "channels/bounded.hpp"
#include "processors/processor.hpp"
#include "processors/sink.hpp"


namespace shar {

using FramesReceiver = channel::Receiver<Image>;

class FrameFileWriter;

template <>
struct SinkTraits<FrameFileWriter> {
  using Input = Image;
  using InputChannel = channel::Receiver<Input>;
};

class FrameFileWriter : public Sink<FrameFileWriter> {
public:
  FrameFileWriter(const std::string& path, Logger logger, FramesReceiver input);
  void process(shar::Image frame);
  void teardown();

private:
  bool write_frame(const shar::Image&);
  std::ofstream m_stream;
};

}