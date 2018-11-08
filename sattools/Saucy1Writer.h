// Copyright 2017 Hakan Metin

#ifndef SATTOOLS_SAUCY1WRITER_H_
#define SATTOOLS_SAUCY1WRITER_H_

#include <vector>
#include <string>
#include <memory>
#include <utility>

#include "sattools/Group.h"
#include "sattools/Literal.h"
#include "sattools/Logging.h"
#include "sattools/StreamBuffer.h"
#include "sattools/LiteralGraphNodeAdaptor.h"

namespace sat {

class Saucy1Writer {
 public:
    static bool dump(const std::string &filename, const Group& group);

 private:
    DISALLOW_COPY_AND_ASSIGN(Saucy1Writer);
};

}  // namespace sat

#endif  // SATTOOLS_SAUCY1WRITER_H_

/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * End:
 */
