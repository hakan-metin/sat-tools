// Copyright 2017 Hakan Metin

#ifndef SATTOOLS_IO_SAUCY1WRITER_H_
#define SATTOOLS_IO_SAUCY1WRITER_H_

#include <vector>
#include <string>
#include <memory>
#include <utility>
#include <ostream>

#include "sattools/sat/Literal.h"
#include "sattools/sym/Group.h"
#include "sattools/sym/LiteralGraphNodeAdaptor.h"
#include "sattools/base/Logging.h"
#include "sattools/io/StreamBuffer.h"

namespace sattools {

class Saucy1Writer {
 public:
    static bool dump(const std::string &filename, const Group& group);
    static bool dump(std::ostream &os, const Group& group);

 private:
    DISALLOW_COPY_AND_ASSIGN(Saucy1Writer);
};

}  // namespace sattools

#endif  // SATTOOLS_IO_SAUCY1WRITER_H_

/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * End:
 */
