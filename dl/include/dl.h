#ifndef DL_INCLUDE_DL_H_
#define DL_INCLUDE_DL_H_

#ifdef _WIN32
    #define WIN32_LEAN_AND_MEAN
#endif

#include <memory>

#ifdef dl_EXPORTS
    #ifdef _MSC_VER
        #define DL_LIB __declspec(dllexport)
    #else
        #define DL_LIB
    #endif
#else
    #ifdef _MSC_VER
        #define DL_LIB __declspec(dllimport)
    #else
        #define DL_LIB
    #endif
#endif

namespace dl {
class DC {
 public:
    virtual ~DC() {}

    virtual void df() = 0;
};

class Factory {
 public:
    virtual ~Factory() {}

    static DL_LIB std::unique_ptr<Factory> &getInstance();

    virtual std::unique_ptr<DC> createDC() = 0;
};
}  // namespace dl

#endif  // DL_INCLUDE_DL_H_
