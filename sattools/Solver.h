// Copyright 2017 Hakan Metin

#ifndef SATTOOLS_SOLVER_H_
#define SATTOOLS_SOLVER_H_

#include <algorithm>
#include <utility>
#include <vector>

#include "sattools/CNFModel.h"
#include "sattools/Literal.h"
#include "sattools/Clause.h"
#include "sattools/Trail.h"
#include "sattools/Propagator.h"
#include "sattools/Simplifier.h"
#include "sattools/SymmetrySimplifier.h"
#include "sattools/SymmetryFinder.h"
#include "sattools/BlissAutomorphismFinder.h"
#include "sattools/SaucyAutomorphismFinder.h"
#include "sattools/LiteralGraphNodeAdaptor.h"
#include "sattools/Orbits.h"
#include "sattools/Order.h"

namespace sat {

class Solver {
 public:
    Solver();
    virtual ~Solver();

    void assign(CNFModel *model);

    void setNumberOfVariables(unsigned int num_variables);

    enum Status {
        SAT,
        UNSAT,
        UNKNOWN,
    };
    Solver::Status solve();

    bool isClauseSatisfied(Clause *clause) const;
    bool isClauseSatisfied(const std::vector<Literal>& literals) const;


    bool simplifyInitialProblem();
    bool simplifyWithSBP();
    void addSBPIntoModel(ClauseInjector *injector);


    void activeSymmetries();

    const Group& group() { return _group; }
    const Order& order() { return _sym_simplifier->order(); }

 private:
    CNFModel *_model;
    unsigned int _num_variables;
    Trail _trail;
    bool _is_model_unsat;

    // Symmetries
    Group _group;
    Order _order;


    std::unique_ptr<Simplifier> _simplifier;
    std::unique_ptr<SymmetrySimplifier> _sym_simplifier;

    bool setModelUnsat() { _is_model_unsat = true; return false; }
    void detactSatisfiedClauses();

};

}  // namespace sat

#endif  // SATTOOLS_SOLVER_H_
/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * End:
 */
