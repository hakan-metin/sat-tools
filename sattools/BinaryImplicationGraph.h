// Copyright 2017 Hakan Metin - LIP6

#ifndef INCLUDE_SATTOOLS_BIG_H_
#define INCLUDE_SATTOOLS_BIG_H_

#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include <string>
#include <sstream>

#include "sattools/CNFModel.h"

namespace sat {

class BIG {
 public:
    BIG(const CNFModel& model);
    ~BIG();

    bool isUnitViaResolution(Literal a, Literal b,  Literal *unit) const;

    std::string debugString() const;

 private:
    std::unordered_map< Literal, std::unordered_set<Literal>> _big;
};


inline BIG::BIG(const CNFModel& model) {
    for (const std::unique_ptr<Clause>& clause : model.binaryClauses()) {
        Literal a = clause->literals()[0];
        Literal b = clause->literals()[1];

        _big[a].insert(b.negated());
        _big[b].insert(a.negated());
    }
}

inline BIG::~BIG() {
}

inline
bool BIG::isUnitViaResolution(Literal a, Literal b,  Literal *unit) const {
    if (_big.find(a) != _big.end() && _big.at(a).find(b) != _big.at(a).end()) {
        *unit = a;
        return true;
    }
    if (_big.find(b) != _big.end() && _big.at(b).find(a) != _big.at(b).end()) {
        *unit = b;
        return true;
    }
    return false;
}


inline std::string BIG::debugString() const {
    std::stringstream ss;
    for (const auto &p : _big) {
        ss << "[" << p.first.debugString() << "]" << ":";
        for (const Literal& literal : p.second)
            ss << literal.debugString() << " ";
        ss << std::endl;
    }
    return ss.str();
}


}  // namespace sat

#endif  // INCLUDE_SATTOOLS_BIG_H_
/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * End:
 */
