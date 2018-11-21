// Copyright 2017 Hakan Metin

#ifndef SATTOOLS_IO_CNFWRITER_H_
#define SATTOOLS_IO_CNFWRITER_H_

#include <vector>
#include <string>
#include <memory>
#include <utility>

#include "sattools/sat/CNFModel.h"
#include "sattools/sat/Literal.h"
#include "sattools/base/Logging.h"
#include "sattools/io/StreamBuffer.h"

namespace sattools {

class CNFWriter {
 public:
    static bool dump(const std::string &filename, const CNFModel& model);

 private:
    DISALLOW_COPY_AND_ASSIGN(CNFWriter);
};
}  // namespace sattools

#endif  // SATTOOLS_IO_CNFWRITER_H_

/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * End:
 */
