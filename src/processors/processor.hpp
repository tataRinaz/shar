#pragma once

#include <string>
#include <atomic>

#include "logger.hpp"


namespace shar {

class Processor {
public:
  class Context {
  public:
    Context(std::string name, shar::Logger logger);
    Context(const Context&) = default;
    Context(Context&&) = default;
    ~Context() = default;

    const std::string& name() const;
    shar::Logger& logger();

  private:
    std::string  m_name;
    shar::Logger m_logger;
  };


  Processor(Context context);
  Processor(Processor&& other);

  template<typename Process>
  void run(
      typename Process::Context context,
      typename Process::Receiver receiver,
      typename Process::Sender sender
  ) {
    Process process {
      std::move(context),
      sender,
      *this
    };

    m_running = true;
    m_context.logger().info("{} started", m_context.name());

    while (m_running && sender.connected() && receiver.connected()) {
      if (auto item = receiver.receive()) {
        process(std::move(*item));
      }
      else {
        // input channel was disconnected
        break;
      }
    }

    // teardown is ~Process
    m_running = false;
    m_context.logger().info("{} finished", m_context.name());
  }

  bool is_running() const;
  void stop();
  Context& context();

private:
  Context           m_context;
  std::atomic<bool> m_running;
};

}