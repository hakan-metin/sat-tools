// Copyright 2017 Hakan Metin

#include "sattools/Solver.h"

namespace sat {

Solver::Solver() :
    _num_variables(0),
    _is_model_unsat(false),
    _simplifier(nullptr) {
}

Solver::Solver(CNFModel *model) {
    assign(model);
}

Solver::~Solver() {
}

void Solver::assign(CNFModel *model) {
    _model = model;
    _simplifier = std::make_unique<Simplifier>(model);
    _decision_policy = std::make_unique<VSIDSDecisionPolicy>(_trail);
    _trail.registerPropagator(&_propagator);
    setNumberOfVariables(model->numberOfVariables());
}


void Solver::setNumberOfVariables(unsigned int num_variables) {
    _num_variables = num_variables;
    _trail.resize(num_variables);
    _propagator.resize(num_variables);
    _decision_policy->increaseNumVariables(num_variables);
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

    Clause *conflict;
    std::vector<Literal> learnt;


    while (!_is_model_unsat) {
        if (!_propagator.propagate(&_trail, &conflict)) {
            if (_trail.currentDecisionLevel() == 0) {
                setModelUnsat();
                break;
            }

            // ConflictManager conflict_manager;
            // conflict_manager.computeFirstUIP(_trail, conflict, &learnt);
            // backtrack(computeBacktrackLevel(learnt));

            // for (Literal l : learnt)
            //     std::cout << l.debugString() << " ";
            // std::cout << std::endl;


            // computeFirstUIP();


            Clause *clause = Clause::create(learnt, true);
            _model->addClause(clause);
            _propagator.addClause(clause, &_trail);

            // _decision_policy->onConflict();
        } else {
            if (_trail.index() == _num_variables)
                break;

            for (BooleanVariable var(0); var < _num_variables; ++var) {
                if (_trail.assignment().variableIsAssigned(var))
                    continue;

                Literal decision(var, false);
                enqueueNewDecision(decision);
                break;
            }
        }
    }

    if (_is_model_unsat) {
        std::cout << "s UNSATISFIABLE" <<std::endl;
        return UNSAT;
    } else {
        std::cout << "s SATISFIABLE" <<std::endl << "v ";
        const Assignment& assignment = _trail.assignment();
        for (BooleanVariable var(0); var < _num_variables; ++var) {
            CHECK(assignment.variableIsAssigned(var));

            Literal l = assignment.getTrueLiteralForAssignedVariable(var);
            std::cout << l.debugString() << " ";
        }
        std::cout << "0" << std::endl;
        return SAT;
    }

    return UNKNOWN;
}

void Solver::enqueueNewDecision(Literal true_literal) {
    _trail.newDecisionLevel();
    _trail.enqueueSearchDecision(true_literal);
}


void Solver::backtrack(unsigned int target_level) {
    unsigned int current_level = _trail.currentDecisionLevel();
    unsigned int old_index = _trail.index();

    if (target_level >= current_level)
        return;

    // LOG(INFO) << _trail.debugString();

    _trail.cancelUntil(target_level);
    _propagator.untrail(_trail.index());

    unsigned int index = _trail.index();

    for (unsigned int i=index; i<old_index; i++) {
        Literal l = _trail[i];
        _decision_policy->onUnassignLiteral(l);
    }

    // LOG(INFO) << "trail remove from " << index << " to " << old_index;
    // LOG(INFO) << _trail.debugString();
}


void Solver::computeFirstUIP() {
    std::vector<Literal> learnt;
    SparseBitset<BooleanVariable> is_marked;

    Clause *clause_to_expand = _propagator.conflictClause();

    unsigned int trail_index = _trail.index() - 1;
    unsigned int highest_level = _trail.currentDecisionLevel();

    is_marked.ClearAndResize(BooleanVariable(_num_variables));

    int num_literal_at_highest_level_that_needs_to_be_processed = 0;

    if (highest_level == 0) {
        setModelUnsat();
        return;
    }

    while (true) {
        DCHECK(clause_to_expand != nullptr);

        _decision_policy->clauseOnConflictReason(clause_to_expand);

        for (Literal literal : *clause_to_expand) {
            const BooleanVariable var = literal.variable();
            const unsigned int level = decisionLevel(var);

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
            learnt.push_back(_trail[trail_index].negated());
            std::swap(learnt.front(), learnt.back());
            break;
        }

        const Literal literal = _trail[trail_index];

        clause_to_expand = _trail.reason(literal.variable());
        num_literal_at_highest_level_that_needs_to_be_processed--;
        trail_index--;
    }

    // for (Literal l : learnt)
    //     std::cout << l.debugString() << " ";
    // std::cout << std::endl;

    // LOG(INFO) <<_trail.debugString();

    // // Minimize conflict clause - SIMPLE
    // auto i = learnt.begin() + 1;
    // auto j = learnt.begin() + 1;;

    // while (i != learnt.end()) {
    //     BooleanVariable v = i->variable();
    //     Clause *reason = _trail.reason(v);
    //     if (reason == nullptr) {
    //         *i = *j++;
    //     } else {
    //         for (Literal literal : *reason) {
    //             const BooleanVariable v = literal.variable();
    //             if (!is_marked[v] && decisionLevel(v) > 0) {
    //                 *i = *j++;
    //                 break;
    //             }
    //         }
    //     }
    //     ++i;
    // }
    // learnt.erase(j, i);
    LOG(INFO) << "After simplification";
    // for (Literal l : learnt)
    //     std::cout << l.debugString() << " ";
    // std::cout << std::endl;
    // exit(0);
    unsigned int backtrack_level = computeBacktrackLevel(learnt);

    backtrack(backtrack_level);

    Clause *clause = Clause::create(learnt, true);
    _model->addClause(clause);
    _propagator.addClause(clause, &_trail);
}



unsigned int
Solver::computeBacktrackLevel(const std::vector<Literal>& literals) {
    unsigned int backtrack_level = 0;

    for (unsigned int i = 1; i < literals.size(); i++) {
        const unsigned int level = decisionLevel(literals[i].variable());
        backtrack_level = std::max(backtrack_level, level);
    }

    DCHECK_LT(backtrack_level,  _trail.currentDecisionLevel());

    return backtrack_level;
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

}  // namespace sat
/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * End:
 */
