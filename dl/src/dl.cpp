/**
 * Copyright (c) 2016, Luis Unzueta
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 * 
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

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
