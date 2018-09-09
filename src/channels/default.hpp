#include <optional>


namespace shar::channel {

template<typename T>
class DefaultReceiver {
public:
  std::optional<T> receive() {
    return T {};
  }
};

}