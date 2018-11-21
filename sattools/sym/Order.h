// Copyright 2017 Hakan Metin - LIP6

#ifndef SATTOOLS_SYM_ORDER_H_
#define SATTOOLS_SYM_ORDER_H_

#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <memory>
#include <utility>
#include <string>

#include "sattools/sat/Literal.h"

namespace sattools {

class Order {
 public:
    Order() {}
    virtual ~Order() {}

    bool add(Literal x);
    bool contains(Literal x) const;

    std::string debugString() const;
 private:
    std::vector<Literal> _order;
    std::unordered_map<Literal, unsigned int> _indexes;
};


class StaticOrder : public Order {
};


class DynamicOrder : public Order {
};

}  // namespace sattools
#endif  // SATTOOLS_SYM_ORDER_H_
/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * End:
 */