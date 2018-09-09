#pragma once

#include "window.hpp"
#include "texture.hpp"
#include "channels/bounded.hpp"
#include "primitives/image.hpp"


namespace shar {

class Processor;

class FrameDisplay {
public:
  struct Context {
    Size m_size;
  };

  FrameDisplay(Context context, Processor& processor);
  ~FrameDisplay();

  void operator()(Image frame, channel::Sender<bool>&);

private:
  Window m_window;
  Texture m_texture;
  Processor& m_processor;
};

}