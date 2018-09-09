#include <cassert>

#include "processors/processor.hpp"

namespace shar {

Processor::Context::Context(std::string name, shar::Logger logger)
    : m_name(std::move(name))
    , m_logger(std::move(logger))
{}

const std::string& Processor::Context::name() const {
  return m_name;
}

shar::Logger& Processor::Context::logger() {
  return m_logger;
}

Processor::Processor(Context context)
  : m_context(std::move(context))
  , m_running(false)
{}

Processor::Processor(Processor&& other)
  : m_context(std::move(other.m_context))
  , m_running(false) {
  assert(!other.m_running);
}

bool Processor::is_running() const {
  return m_running;
}

void Processor::stop() {
  m_running = false;
}

Processor::Context& Processor::context() {
  return m_context;
}

}