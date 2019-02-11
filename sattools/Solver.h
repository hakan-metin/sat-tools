// Copyright 2017 Hakan Metin

#ifndef SATTOOLS_SOLVER_H_
#define SATTOOLS_SOLVER_H_

#include <algorithm>
#include <deque>
#include <utility>
#include <vector>
#include <memory>

#include "sattools/Bitset.h"
#include "sattools/CNFModel.h"
#include "sattools/Literal.h"
#include "sattools/Clause.h"
#include "sattools/Trail.h"
#include "sattools/Propagator.h"
#include "sattools/Simplifier.h"

namespace sat {

class Solver {
 public:
    Solver();
    explicit Solver(CNFModel *model);
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

 private:
    CNFModel *_model;
    unsigned int _num_variables;
    Trail _trail;
    Propagator _propagator;

    bool _is_model_unsat;

    std::unique_ptr<Simplifier> _simplifier;


    void backtrack(unsigned int target_level);

    bool setModelUnsat() { _is_model_unsat = true; return false; }

    void computeFirstUIP();

    unsigned int computeBacktrackLevel(const std::vector<Literal>& literals);



    // Returns the decision level of a given variable.
    unsigned int decisionLevel(BooleanVariable var) const {
        return _trail.info(var).level;
    }
};

}  // namespace sat

#endif  // SATTOOLS_SOLVER_H_
/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * End:
 */
