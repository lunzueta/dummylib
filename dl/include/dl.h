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
#ifndef DL_INCLUDE_DL_H_
#define DL_INCLUDE_DL_H_

#ifdef _WIN32
    #define WIN32_LEAN_AND_MEAN
#endif

#include <memory>
#include <string>
#include <utility>
#include <vector>
#include <opencv2/opencv.hpp>

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

    virtual void addSample(const std::pair<cv::Mat, int> &sample) = 0;
    virtual void setSamples(
        const std::vector< std::pair<cv::Mat, int> > &samples) = 0;
    virtual std::vector< std::pair<cv::Mat, int> > &getSamples() = 0;
    virtual bool loadFromHDF5(const std::string &filename) = 0;
    virtual bool saveToHDF5(const std::string &filename,
        const cv::Size &imgSize) = 0;
};

class Factory {
 public:
    virtual ~Factory() {}

    static DL_LIB std::unique_ptr<Factory> &getInstance();

    virtual std::unique_ptr<DC> createDC() = 0;
};
}  // namespace dl

#endif  // DL_INCLUDE_DL_H_
