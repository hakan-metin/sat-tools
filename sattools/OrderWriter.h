// Copyright 2017 Hakan Metin

#ifndef SATTOOLS_ORDERWRITER_H_
#define SATTOOLS_ORDERWRITER_H_

#include <vector>
#include <string>
#include <memory>
#include <utility>

#include "sattools/Order.h"
#include "sattools/Literal.h"
#include "sattools/Logging.h"
#include "sattools/StreamBuffer.h"

namespace sat {

class OrderWriter {
 public:
    static bool dump(const std::string &filename, const Order& order);

 private:
    DISALLOW_COPY_AND_ASSIGN(OrderWriter);
};
}  // namespace sat

#endif  // SATTOOLS_ORDERWRITER_H_

/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * End:
 */
