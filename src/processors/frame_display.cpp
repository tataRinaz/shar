#include "processors/frame_display.hpp"

#include "processors/processor.hpp"
#include "primitives/point.hpp"
#include "primitives/size.hpp"


namespace shar {

FrameDisplay::FrameDisplay(Context context, Processor& processor)
    : m_window(context.m_size, processor.context().logger())
    , m_texture(context.m_size)
    , m_processor(processor) {
  m_texture.bind();
}

FrameDisplay::~FrameDisplay() {
  m_texture.unbind();
}

void FrameDisplay::operator()(Image frame, channel::Sender<bool>&) {
  m_texture.update(Point::origin(),
                   frame.size(),
                   frame.bytes());
  m_window.draw_texture(m_texture);
  m_window.swap_buffers();
  m_window.poll_events();

  if (m_window.should_close()) {
    m_processor.stop();
  }
}

}