#include <dl.h>
#include <mutex>

#include "./dc.h"

using std::unique_ptr;
using std::make_unique;

namespace dl {
class DLFactory : public Factory {
 public:
    DLFactory() {}
    ~DLFactory() {}

    unique_ptr<DC> createDC() {
        return make_unique<DLDC>();
    }
};

std::mutex mutexFactory;

unique_ptr<Factory> &Factory::getInstance() {
    static unique_ptr<Factory> instance = nullptr;

    if (!instance) {
        std::lock_guard<std::mutex> lock(mutexFactory);

        if (!instance) {
            instance.reset(new DLFactory());
            ::google::InitGoogleLogging("dl");
            FLAGS_minloglevel = 0;
            FLAGS_alsologtostderr = 1;
        }
    }

    return instance;
}
}  // namespace dl
