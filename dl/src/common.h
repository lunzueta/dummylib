#ifndef DL_SRC_COMMON_H_
#define DL_SRC_COMMON_H_

#include <dl.h>
#include <leveldb/db.h>
#include <leveldb/write_batch.h>
#include <lmdb.h>
#include <caffe/util/db.hpp>
#include <caffe/layers/hdf5_data_layer.hpp>
#include <caffe/layers/hdf5_output_layer.hpp>
#include <caffe/util/hdf5.hpp>
#include <caffe/caffe.hpp>
#include <caffe/util/rng.hpp>

namespace dl {
    void df();
}  // namespace dl

#endif  // DL_SRC_COMMON_H_
