// Copyright 2017 Hakan Metin

#ifndef SATTOOLS_IO_CNFREADER_H_
#define SATTOOLS_IO_CNFREADER_H_

#include <algorithm>
#include <vector>
#include <string>
#include <utility>

#include "sattools/sat/CNFModel.h"
#include "sattools/sat/Literal.h"
#include "sattools/base/Logging.h"
#include "sattools/io/StreamBuffer.h"

namespace sattools {

class CNFReader {
 public:
    CNFReader();
    ~CNFReader();

    bool load(const std::string &filename, CNFModel *model);

 private:
    DISALLOW_COPY_AND_ASSIGN(CNFReader);
};
}  // namespace sattools

#endif  // SATTOOLS_IO_CNFREADER_H_

/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * End:
 */
