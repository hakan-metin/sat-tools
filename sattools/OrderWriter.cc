// Copyright 2017 Hakan Metin

#include "sattools/OrderWriter.h"

namespace sat {

bool OrderWriter::dump(const std::string &filename, const Order& order) {
    StreamBuffer out(filename, kUncompressedWrite);

    for (Literal literal : order)
        out.write(literal.debugString() + " ");
    out.write("0\n");
    out.flush();

    return true;
}


}  // namespace sat
/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * End:
 */
