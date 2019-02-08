// Copyright 2017 Hakan Metin

#include "sattools/Solver.h"

namespace sat {

Solver::Solver() :
    _num_variables(0),
    _is_model_unsat(false),
    _simplifier(nullptr)
{
}

Solver::Solver(CNFModel *model) {
    assign(model);
}

Solver::~Solver() {
}

void Solver::assign(CNFModel *model) {
    setNumberOfVariables(model->numberOfVariables());
    _model = model;
    _simplifier = std::make_unique<Simplifier>(model);
    _trail.registerPropagator(&_propagator);
}


void Solver::setNumberOfVariables(unsigned int num_variables) {
    _num_variables = num_variables;
    _trail.resize(num_variables);
    _propagator.resize(num_variables);
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


Solver::Status Solver::solve() {
    LOG(INFO) << "Solving";
    for (Clause *clause : _model->clauses())
        if (!_propagator.addClause(clause, &_trail))
            return UNSAT;

    std::vector<Literal> decisions = { -1, -6, -5 };

    for (Literal decision : decisions) {
        DCHECK(!_trail.assignment().literalIsAssigned(decision));
        _trail.newDecisionLevel();
        _trail.enqueueSearchDecision(decision);

        if (!_propagator.propagate(&_trail)) {
            LOG(INFO) << "conflict";
            computeFirstUIP();
        }
    }
    // if (!_propagator.propagate(&_trail))
    //     LOG(INFO) << "conflict";

    return UNKNOWN;
}


void Solver::computeFirstUIP() {
    std::deque<Literal> learnt;
    SparseBitset<BooleanVariable> is_marked;

    Clause *clause_to_expand = _propagator.conflictClause();

    unsigned int trail_index = _trail.index() - 1;
    unsigned int highest_level = _trail.currentDecisionLevel();

    is_marked.ClearAndResize(BooleanVariable(_num_variables));

    int num_literal_at_highest_level_that_needs_to_be_processed = 0;

    while (true) {
        DCHECK(clause_to_expand != nullptr);

        LOG(INFO) << clause_to_expand->debugString();

        for (Literal literal : *clause_to_expand) {
            const BooleanVariable var = literal.variable();
            const unsigned int level = decisionLevel(var);

            // LOG(INFO) << "Level " << literal.debugString() << " = " << level;

            if (is_marked[var] || level == 0)
                continue;

            is_marked.Set(var);

            if (level == highest_level)
                num_literal_at_highest_level_that_needs_to_be_processed++;
            else
                learnt.push_back(literal);
        }

        // Find next marked literal to expand from the trail.
        DCHECK_GT(num_literal_at_highest_level_that_needs_to_be_processed, 0);
        while (!is_marked[_trail[trail_index].variable()])
            --trail_index;

        if (num_literal_at_highest_level_that_needs_to_be_processed == 1) {
            learnt.push_front(_trail[trail_index].negated());
            break;
        }

        const Literal literal = _trail[trail_index];

        clause_to_expand = _trail.reason(literal.variable());
        num_literal_at_highest_level_that_needs_to_be_processed--;
        trail_index--;
    }

    for (Literal l : learnt)
        std::cout << l.debugString() << " ";
    std::cout << std::endl;

    LOG(INFO) <<_trail.debugString();

    // Minimize conflict clause - SIMPLE
    auto i = learnt.begin() + 1;
    auto j = learnt.begin() + 1;;

    while (i != learnt.end()) {
        BooleanVariable v = i->variable();
        Clause *reason = _trail.reason(v);
        if (reason == nullptr)
            *i = *j++;
        else
            for (Literal literal : *reason) {
                const BooleanVariable v = literal.variable();
                if (!is_marked[v] && decisionLevel(v) > 0) {
                    *i = *j++;
                    break;
                }
            }
        ++i;
    }
    learnt.erase(j, i);
    LOG(INFO) << "After simplification";
    for (Literal l : learnt)
        std::cout << l.debugString() << " ";
    std::cout << std::endl;

}


bool Solver::simplifyInitialProblem() {
    if (_simplifier == nullptr)
        return true;

    for (Clause *clause : _model->clauses()) {
        if (clause->size() == 1) {
            Literal unit = clause->literals()[0];
            if (_trail.assignment().literalIsFalse(unit)) {
                return setModelUnsat();
            }
            if (!_trail.assignment().literalIsAssigned(unit))
                _trail.enqueueWithUnitReason(unit);
        }
        _simplifier->addClauseToProcess(clause);
    }

    if (!_simplifier->simplify(&_trail))
        return setModelUnsat();

    _model->clearDetachedClauses();

    return true;
}

// void Solver::enqueue(Literal true_literal) {

// }

// void Solver::backtrack(unsigned int target_level) {

// }


}  // namespace sat
/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * End:
 */
