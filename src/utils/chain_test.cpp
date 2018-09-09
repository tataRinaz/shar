#include <cstdlib>

#include "logger.hpp"
#include "processors/pipeline.hpp"
#include "processors/packet_receiver.hpp"
#include "processors/h264decoder.hpp"


using namespace shar;

int main() {
  auto logger = Logger("chain_test.txt");

  auto [runner, sender, receiver] =
      pipeline<PacketReceiver>({IpAddress()}, {"PacketReceiver", logger})
        .chain<H264Decoder>({}, {"H264Decoder", logger})
        .build();

  return EXIT_SUCCESS;
}