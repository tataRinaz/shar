#pragma once

#include <utility>

#include "processors/processor.hpp"
#include "channels/bounded.hpp"


namespace shar {

static const int DEFAULT_CHANNEL_SIZE = 30;

template<typename P, int count>
struct Bounded {
  using Channel = typename P::Sender;
  using Item = typename Channel::Item;

  static auto create() {
    return channel::bounded<Item>(count);
  }
};

template<typename P>
using DefaultChannelPolicy = Bounded<P, DEFAULT_CHANNEL_SIZE>;


struct EmptyRunner {
  void start() {}
};

template<typename C>
struct EmptyPipeline {
  auto build() {
    auto[sender, receiver] = C::create();
    return std::tuple {
        EmptyRunner {},
        std::move(sender),
        std::move(receiver)
    };
  }
};


template<typename P>
struct ThreadRunner {
  using Context = typename P::Context;
  using Sender = typename P::Sender;
  using Receiver = typename P::Receiver;

  ThreadRunner(
      Context context,
      Processor::Context processor_context,
      Sender sender,
      Receiver receiver
  )
      : m_processor_context(std::move(processor_context))
      , m_context(std::move(context))
      , m_sender(std::move(sender))
      , m_receiver(std::move(receiver)) {}

  void start() {
    std::thread worker {[this] {
      Processor processor {std::move(m_processor_context)};

      processor.run<P>(
          std::move(m_context),
          std::move(m_sender),
          std::move(m_receiver)
      );
    }};

    worker.detach();
  }

  Processor::Context m_processor_context;
  Context            m_context;
  Sender             m_sender;
  Receiver           m_receiver;
};

template<typename R1, typename R2>
struct RunnerPair {
  R1 first;
  R2 second;

  void start() {
    first.start();
    second.start();
  }
};

template<
    typename P, // Process
    typename C, // ChannelCreatePolicy
    typename Previous
>
class Pipeline {
public:
  using Current = Pipeline<P, C, Previous>;
  using ProcessContext = typename P::Context;

  Pipeline(ProcessContext context, Processor::Context processor_context, Previous previous)
      : m_previous(std::move(previous))
      , m_context(std::move(context))
      , m_processor_context(std::move(processor_context)) {}

  Pipeline(Pipeline&& other) = default;

  template<typename NextP, typename NextC=DefaultChannelPolicy<NextP>>
  auto chain(
      typename NextP::Context context,
      Processor::Context processor_context
  ) {
    return Pipeline<NextP, NextC, Current> {
        std::move(context),
        std::move(processor_context),
        Current {std::move(*this)}
    };
  }

  // returns (runner, sender, receiver)
  auto build() {
    auto[runner, start, receiver] = m_previous.build();
    auto[sender, end] = C::create();
    auto current_runner = ThreadRunner<P> {
        std::move(m_context),
        std::move(m_processor_context),
        std::move(sender),
        std::move(receiver)
    };
    return std::tuple {
        RunnerPair {runner, current_runner},
        std::move(start),
        std::move(end)
    };
  }

private:
  Previous           m_previous;
  ProcessContext     m_context;
  Processor::Context m_processor_context;
};

template<typename P, typename C=DefaultChannelPolicy<P>>
Pipeline<P, C, EmptyPipeline<C>> pipeline(
    typename P::Context context,
    Processor::Context process_context
) {
  return Pipeline<P, C, EmptyPipeline<C>> {
      std::move(context),
      std::move(process_context),
      EmptyPipeline<C> {}
  };
}


};