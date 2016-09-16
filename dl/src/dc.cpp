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

#include "./dc.h"

using std::ofstream;
using std::ifstream;
using std::pair;
using std::string;
using std::cout;
using std::endl;
using std::vector;
using cv::Mat;
using cv::Size;
using caffe::Datum;
using caffe::Blob;
using caffe::BlobProto;
using dl::DLDC;

DLDC::DLDC() {}
DLDC::~DLDC() {}

void DLDC::addSample(const pair<Mat, int> &sample) {
    Mat img;
    sample.first.convertTo(img, CV_MAKETYPE(CV_8U,
        sample.first.channels()));

    m_samples.push_back(pair<Mat, int>(img, sample.second));
}

void DLDC::setSamples(const vector< pair<Mat, int> > &samples) {
    m_samples.clear();
    for (int i = 0, n = static_cast<int>(samples.size()); i < n; ++i) {
        addSample(samples[i]);
    }
}

vector< pair<Mat, int> > &DLDC::getSamples() {
    return m_samples;
}

bool DLDC::loadFromHDF5(const string &filename) {
    ifstream filein(filename);
    if (filein.fail()) {
        cout << "ERROR: Can't open " << filename << endl;
        return false;
    }

    m_samples.clear();

    cout << "Loading from " << filename << endl;

    hid_t file_id = H5Fopen(filename.c_str(), H5F_ACC_RDONLY, H5P_DEFAULT);
    if (file_id < 0) {
        cout << "ERROR: H5Fopen failed." << endl;
        return false;
    }

    Blob<float> blob_data, blob_label;
    hdf5_load_nd_dataset(file_id, HDF5_DATA_DATASET_NAME, 0, 4, &blob_data);
    hdf5_load_nd_dataset(file_id, HDF5_DATA_LABEL_NAME, 0, 4, &blob_label);

    herr_t status = H5Fclose(file_id);
    if (status < 0) {
        cout << "ERROR: H5Fclose failed." << endl;
        return false;
    }

    int nSamples = blob_data.num();
    int nChannels = blob_data.channels();
    int height = blob_data.height();
    int width = blob_data.width();

    int count = 0;
    for (int i = 0; i < nSamples; ++i) {
        vector<Mat> imageChannels(nChannels);
        for (int c = 0; c < nChannels; ++c) {
            imageChannels[c] = Mat(height, width, CV_8U);

            for (int h = 0; h < height; ++h) {
                for (int w = 0; w < width; ++w) {
                    imageChannels[c].at<uchar>(h, w) =
                        static_cast<uchar>(blob_data.data_at(i, c, h, w));
                }
            }
        }

        pair<Mat, int> sample;
        Mat img;
        cv::merge(imageChannels, img);
        sample.first = img;
        sample.second = static_cast<int>(blob_label.data_at(i, 0, 0, 0));
        addSample(sample);

        ++count;
        if (count % 10000 == 0) {
            cout << "Processed " << count << " files." << endl;
        }
    }

    if (count % 10000 != 0) {
        cout << "Processed " << count << " files." << endl;
    }

    cout << "Done." << endl;

    return true;
}

bool DLDC::saveToHDF5(const string &filename, const Size &imgSize) {
    int n = static_cast<int>(filename.find_last_of("."));
    string filename_;
    if (n >= 0) {
        filename_ = filename.substr(0, n);
    } else {
        filename_ = filename;
    }

    filename_ = filename_ + ".h5";
    ofstream fileout(filename_);
    if (fileout.fail()) {
        cout << "ERROR: Can't open " << filename_ << endl;
        return false;
    }

    fileout.close();

    cout << "Saving data to " << filename_ << endl;

    Blob<float> blob_data, blob_label;
    vector<Blob<float> *> bottomDataBlobVec, topDataBlobVec;
    caffe::LayerParameter param;
    param.mutable_hdf5_output_param()->set_file_name(filename_);

    int nSamples = static_cast<int>(m_samples.size());
    if (nSamples > 0) {
        blob_label.Reshape(nSamples, 1, 1, 1);
        blob_data.Reshape(nSamples, m_samples[0].first.channels(),
            m_samples[0].first.rows, m_samples[0].first.cols);

        int count = 0;
        for (int i = 0; i < nSamples; ++i) {
            Mat img, resizedImg;
            m_samples[i].first.copyTo(img);
            cv::resize(img, resizedImg, imgSize, cv::INTER_LINEAR);

            vector<Mat> resizedImgChannels;
            cv::split(resizedImg, resizedImgChannels);

            int nChannels = resizedImg.channels();
            int height = imgSize.height;
            int width = imgSize.width;
            for (int c = 0; c < nChannels; ++c) {
                for (int h = 0; h < height; ++h) {
                    for (int w = 0; w < width; ++w) {
                        int idx = i * nChannels * height * width +
                            c * height * width +h * width + w;
                        float *blob_data_array = blob_data.mutable_cpu_data();
                        blob_data_array[idx] =
                            static_cast<float>(
                            resizedImgChannels[c].at<uchar>(h, w));
                    }
                }
            }

            float *blob_label_array = blob_label.mutable_cpu_data();
            blob_label_array[i] =
                static_cast<float>(m_samples[i].second);

            if (++count % 1000 == 0) {
                cout << "Processed " << count << " files." << endl;
            }
        }

        if (count % 1000 != 0) {
            cout << "Processed " << count << " files." << endl;
        }

        bottomDataBlobVec.push_back(&blob_data);
        bottomDataBlobVec.push_back(&blob_label);

        caffe::HDF5OutputLayer<float> layer(param);
        layer.SetUp(bottomDataBlobVec, topDataBlobVec);
        layer.Forward(bottomDataBlobVec, topDataBlobVec);
    }

    cout << "Done." << endl;

    return true;
}
