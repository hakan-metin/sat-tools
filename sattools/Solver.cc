// Copyright 2017 Hakan Metin

#include "sattools/Solver.h"

namespace sat {

Solver::Solver() :
    _num_variables(0),
    _is_model_unsat(false)
{
}


Solver::~Solver() {
}

void Solver::assign(CNFModel *model) {
    setNumberOfVariables(model->numberOfVariables());

    _simplifier = std::make_unique<Simplifier>(&_propagator, model);
    _simplifier->simplify();

    auto i = model->clauses().begin();
    auto j = model->clauses().begin();
    const auto end = model->clauses().end();

    LOG(INFO) << model->numberOfClauses();

    // Add clauses and clean lazyDetach and satisfied clauses
    while (i != end) {
        Clause *clause = *j++ = *i++;
        if (isClauseSatisfied(clause) || clause->size() == 0)
            j--;

        if (!_propagator.addClause(clause, &_trail))
            setModelUnsat();
    }

    if (j < i) {
        while (i != end)
            *j++ = *i++;
        model->clauses().resize(j - model->clauses().begin());
    }

    LOG(INFO) << model->numberOfClauses();
}

void Solver::setNumberOfVariables(unsigned int num_variables) {
    _propagator.resize(num_variables);
    _trail.resize(num_variables);
}


bool Solver::isClauseSatisfied(Clause *clause) const {
    for (const Literal &literal : *clause)
        if (_trail.assignment().literalIsTrue(literal))
            return true;
    return false;
}
bool Solver::isClauseSatisfied(const std::vector<Literal>& literals) const {
    for (const Literal &literal : literals)
        if (_trail.assignment().literalIsTrue(literal))
            return true;
    return false;
}



// void Solver::enqueue(Literal true_literal) {

// }

// void Solver::backtrack(unsigned int target_level) {

// }


// Solver::Status Solver::solve() {




//     return UNSAT;
// }



}  // namespace sat
/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * End:
 */




// bool Solver::addClause(Clause *clause) {
//     const int size = clause->size();
//     Literal *literals = clause->literals();

//     if (size == 1) {
//         const Literal unit = literals[0];
//         if (_trail.assignment().literalIsFalse(unit)) return setModelUnsat();
//         if (_trail.assignment().literalIsTrue(unit))  return true;
//         _trail.enqueueWithUnitReason(unit);
//     } else {
//         int num_literal_not_false = 0;

//         for (int i = 0; i < size; ++i) {
//             if (!_trail.assignment().literalIsFalse(literals[i])) {
//                 std::swap(literals[i), literals[num_literal_not_false]);
//                 ++num_literal_not_false;
//                 if (num_literal_not_false == 2) {
//                     break;
//                 }
//             }
//         }

//         if (num_literal_not_false == 1) {
//             if (!_trail.assignment().literalIsTrue(literals[0]))
//                 _trail.enqueueWithAssertiveReason(literals[0]);
//         } else {
//             if (!_propagator.addClause(clause, &_trail))
//                 return setModelUnsat();
//         }
//     }

//     return true;
// }
