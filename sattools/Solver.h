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
    };
    Solver::Status solve();

    bool isClauseSatisfied(Clause *clause) const;
    bool isClauseSatisfied(const std::vector<Literal>& literals) const;

 private:
    unsigned int _num_variables;
    Propagator _propagator;
    Trail _trail;
    bool _is_model_unsat;

    std::unique_ptr<Simplifier> _simplifier;

    bool setModelUnsat() { _is_model_unsat = true; return false; }
};

}  // namespace sat

#endif  // SATTOOLS_SOLVER_H_
/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * End:
 */
