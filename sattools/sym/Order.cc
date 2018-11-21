// Copyright 2017 Hakan Metin - LIP6

#include "sattools/sym/Order.h"

namespace sattools {

bool Order::add(Literal literal) {
    if (contains(literal))
        return false;

    const unsigned int sz = _order.size();
    _indexes[literal] = sz;
    _indexes[literal.negated()] = sz;

    _order.push_back(literal);

    return true;
}

bool Order::contains(Literal literal) const {
    return _indexes.find(literal) != _indexes.end();
}

std::string Order::debugString() const {
    std::string output;

    for (Literal l : _order)
        output += l.debugString() + " ";
    // output += "\n";

    return output;
}

}  // namespace sattools
