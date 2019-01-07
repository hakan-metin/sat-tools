// Copyright 2017 Hakan Metin - LIP6

#ifndef SATTOOLS_SIMPLIFIER_H_
#define SATTOOLS_SIMPLIFIER_H_

#include <unordered_set>
#include <vector>

#include "sattools/CNFModel.h"
#include "sattools/Propagator.h"

namespace sat {

class Simplifier {
 public:
    Simplifier(Propagator *propagator, CNFModel *model);
    virtual ~Simplifier();

    bool simplify();

 private:
    Propagator *_propagator;
    CNFModel *_model;

    std::unordered_set<Clause*> _clauses_to_process;

    bool processAllClauses();
    bool processClauseToSimplifiy(Clause *clause);
    bool simplifyClause(Clause *a, Clause *b, LiteralIndex *opposite_literal);
};


}  // namespace sat


#endif  // SATTOOLS_SYMMETRYSIMPLIFIER_H_
/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * End:
 */
