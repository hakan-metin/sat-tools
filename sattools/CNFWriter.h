// Copyright 2017 Hakan Metin

#ifndef SATTOOLS_CNFWRITER_H_
#define SATTOOLS_CNFWRITER_H_

#include <algorithm>
#include <vector>
#include <string>
#include <utility>

#include "sattools/CNFModel.h"
#include "sattools/Literal.h"
#include "sattools/Logging.h"
#include "sattools/StreamBuffer.h"

namespace sat {

class CNFWriter {
 public:
    CNFWriter();
    ~CNFWriter();

    bool dump(const std::string &filename, const CNFModel& model);

 private:
    DISALLOW_COPY_AND_ASSIGN(CNFWriter);
};
}  // namespace sat

#endif  // SATTOOLS_CNFWRITE_H_

/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * End:
 */
