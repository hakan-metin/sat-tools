// Copyright 2017 Hakan Metin

#ifndef SATTOOLS_CNFREADER_H_
#define SATTOOLS_CNFREADER_H_

#include <vector>
#include <string>

#include "sattools/CNFModel.h"
#include "sattools/Literal.h"
#include "sattools/Logging.h"
#include "sattools/StreamBuffer.h"

namespace sat {

class CNFReader {
 public:
    CNFReader();
    ~CNFReader();

    bool load(const std::string &filename, CNFModel *model);

 private:
    DISALLOW_COPY_AND_ASSIGN(CNFReader);
};
}  // namespace sat

#endif  // SATTOOLS_CNFREADER_H_

/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * End:
 */
