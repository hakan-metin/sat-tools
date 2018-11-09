// Copyright 2017 Hakan Metin - LIP6

#ifndef SATTOOLS_BINARYIMPLICATIONGRAPH_H_
#define SATTOOLS_BINARYIMPLICATIONGRAPH_H_

#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>

#include "sattools/CNFModel.h"

namespace sat {

class BinaryImplicationGraph {
 public:
    explicit BinaryImplicationGraph(const CNFModel& model);
    ~BinaryImplicationGraph();

    void addBinaryClause(Literal a, Literal b);
    bool resolve(const std::vector<Literal>& clause, Literal *unit) const;

    std::string debugString() const;

 private:
    std::unordered_map< Literal, std::unordered_set<Literal>> _big;

    bool resolveBinary(Literal a, Literal b,  Literal *unit) const;
};

}  // namespace sat

#endif  // SATTOOLS_BINARYIMPLICATIONGRAPH_H_
/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * End:
 */
