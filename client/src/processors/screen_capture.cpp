#include "processors/screen_capture.hpp"


namespace {

shar::Frame convert(const sc::Image& image) noexcept {
  std::size_t width  = static_cast<std::size_t>(Width(image));
  std::size_t height = static_cast<std::size_t>(Height(image));
  std::size_t pixels = width * height;

  const std::size_t PIXEL_SIZE = 4;

  auto bytes = std::make_unique<uint8_t[]>(pixels * PIXEL_SIZE);
  auto size  = shar::Size {height, width};

  assert(bytes.get() != nullptr);
  sc::Extract(image, bytes.get(), pixels * PIXEL_SIZE);

  return shar::Frame {std::move(bytes), size};
}


struct FrameHandler {
  FrameHandler(shar::Sender<shar::Frame>& frames_consumer)
      : m_frames_consumer(frames_consumer) {}

  void operator()(const sc::Image& frame, const sc::Monitor& /* monitor */) {
    shar::Frame buffer = convert(frame);
    // ignore return value here, if channel was disconnected ScreenCapture will stop
    // processing new frames anyway
    m_frames_consumer.send(std::move(buffer));
  }

  shar::Sender<shar::Frame>& m_frames_consumer;
};

}

namespace shar {

ScreenCapture::ScreenCapture(Context context,
                             Milliseconds interval,
                             const sc::Monitor& monitor,
                             Sender<Frame> output)
    : Source(std::move(context), std::move(output))
    , m_interval(std::move(interval))
    , m_wakeup_timer(std::chrono::seconds(1))
    , m_config(nullptr)
    , m_capture(nullptr) {

  m_config = sc::CreateCaptureConfiguration([&] {
    return std::vector<sc::Monitor> {monitor};
  });

  m_config->onNewFrame(FrameHandler {Processor::output()});
}

void ScreenCapture::process(FalseInput) {
  m_wakeup_timer.restart();
  m_wakeup_timer.wait();
}

void ScreenCapture::setup() {
  m_capture = m_config->start_capturing();
  m_capture->setFrameChangeInterval(m_interval);
}

void ScreenCapture::teardown() {
  m_capture.reset();
}

}
