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

#define CATCH_CONFIG_RUNNER

#include <dl.h>
#include <sys/stat.h>
#include <string>
#include <utility>
#include <vector>
#ifdef _WIN32
#include <direct.h>
#endif
#include <Catch/catch.hpp>

using std::cout;
using std::endl;
using std::unique_ptr;
using std::vector;
using std::pair;
using std::string;

using cv::Mat;
using cv::Size;

using dl::Factory;
using dl::DC;

struct stat info_db;

int main(int argc, char *const argv[]) {
    Catch::Session session;
    int returnCode = session.applyCommandLine(argc, argv);
    if (returnCode != 0)
        return returnCode;

    string outputFolderPath = "output";
    if (stat(outputFolderPath.c_str(), &info_db) != 0) {
    #ifndef _MSC_VER
        if (mkdir(imageFolderPath.c_str(), 0744) != 0) {
            cout << "ERROR: mkdir " << outputFolderPath << " failed."
                << endl;
            return false;
        }
    #else
        if (_mkdir(outputFolderPath.c_str()) != 0) {
            cout << "ERROR: mkdir " << outputFolderPath << " failed."
                << endl;
            return false;
        }
    #endif
    }

    return session.run();
}

bool imagesMatch(const Mat &image1, const Mat &image2) {
    if (image1.rows != image2.rows || image1.cols != image2.cols) {
        return false;
    } else {
        for (int y = 0; y < image1.rows; ++y) {
            for (int x = 0; x < image1.cols; ++x) {
                if (image1.at<uchar>(y, x) != image2.at<uchar>(y, x))
                    return false;
            }
        }
    }

    return true;
}

bool sampleListsMatch(const vector< pair<Mat, int> > &sampleList1,
    const vector< pair<Mat, int> > &sampleList2) {
    int n = static_cast<int>(sampleList1.size());
    if (sampleList2.size() != n)
        return false;

    for (int i = 0; i < n; ++i) {
        if (!imagesMatch(sampleList1[i].first, sampleList2[i].first))
            return false;

        if (sampleList1[i].second != sampleList2[i].second)
            return false;
    }

    return true;
}

SCENARIO("DC functions work correctly") {
    GIVEN("A set of data samples") {
        int n = 5;
        vector< pair<Mat, int> > samples(n);
        for (int i = 0; i < n; ++i) {
            samples[i].first = Mat::zeros(n, n, CV_8U);
            samples[i].second = i;
        }

        WHEN("they are set to the DC instance") {
            unique_ptr<DC> dc =
                Factory::getInstance()->createDC();
            dc->setSamples(samples);

            THEN("they can be correctly converted to HDF5 format") {
                REQUIRE(dc->saveToHDF5(
                    "output/testC.h5", Size(n, n)));
                REQUIRE(dc->loadFromHDF5(
                    "output/testC.h5"));
                REQUIRE(sampleListsMatch(dc->getSamples(), samples));
            }
        }
    }
}
