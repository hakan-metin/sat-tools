// Copyright 2017 Hakan Metin - LIP6

#ifndef INCLUDE_SATTOOLS_BREAKIDREADER_H_
#define INCLUDE_SATTOOLS_BREAKIDREADER_H_

#include <memory>
#include <string>

#include "sattools/Group.h"
#include "sattools/Permutation.h"
#include "sattools/StreamBuffer.h"

namespace sat {

class BreakIDReader {
 public:
    BreakIDReader() {}
    ~BreakIDReader() {}

    bool load(const std::string& symmetry_filename, unsigned int num_vars,
              Group *group);
};


}  // namespace sat

#endif  // INCLUDE_SATTOOLS_BREAKIDREADER_H_

/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * End:
 */
