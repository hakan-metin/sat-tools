// Copyright 2017 Hakan Metin - LIP6

#include "sattools/Order.h"

namespace sat {

bool Order::add(Literal literal) {
    if (contains(literal))
        return false;

    const unsigned int sz = _order.size();
    _indexes[literal] = sz;
    _indexes[literal.negated()] = sz;

    return true;
}

bool Order::contains(Literal literal) const {
    return _indexes.find(literal) != _indexes.end();
}

}  // namespace sat
