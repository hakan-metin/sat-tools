// Copyright 2017 Hakan Metin - LIP6

#include "sattools/BinaryImplicationGraph.h"
namespace sat {


void BinaryImplicationGraph::addBinaryClause(Literal a, Literal b) {
    _big[a].insert(b.negated());
    _big[b].insert(a.negated());
}

BinaryImplicationGraph::BinaryImplicationGraph(const CNFModel& model) {
    for (Clause *clause : model.clauses()) {
        if (clause->size() != 2)
            continue;

        Literal a = clause->literals()[0];
        Literal b = clause->literals()[1];

        _big[a].insert(b.negated());
        _big[b].insert(a.negated());
    }
}

BinaryImplicationGraph::~BinaryImplicationGraph() {
}

bool BinaryImplicationGraph::resolve(const std::vector<Literal>& clause,
                                     Literal *unit) const {
    if (clause.size() == 2) {
        Literal a = clause[0];
        Literal b = clause[1];
        return resolveBinary(a, b, unit);
    }

    return false;
}

bool BinaryImplicationGraph::resolveBinary(Literal a, Literal b,
                                           Literal *unit) const {
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


std::string BinaryImplicationGraph::debugString() const {
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
/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * End:
 */
