// Copyright 2017 Hakan Metin

#include "sattools/Solver.h"

namespace sat {

Solver::Solver() :
    _num_variables(0),
    _is_model_unsat(false),
    _drat_proof_handler(nullptr),
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
    _restart_policy = std::make_unique<LubyRestartPolicy>();

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
    std::vector<Literal> reason_used_to_infer_the_conflict;

    LOG(INFO) << "Solving";
    for (Clause *clause : _model->clauses())
        if (!_propagator.addClause(clause, &_trail))
            return UNSAT;

    Clause *conflict;
    std::vector<Literal> learnt;
    ConflictManager conflict_manager(_trail);


    while (!_is_model_unsat) {
        if (!_propagator.propagate(&_trail, &conflict)) {
            if (_trail.currentDecisionLevel() == 0) {
                setModelUnsat();
                break;
            }

            conflict_manager.computeFirstUIP(conflict,
                                             &reason_used_to_infer_the_conflict,
                                             &learnt);
            backtrack(computeBacktrackLevel(learnt));
            Clause *clause = Clause::create(learnt, true);
            _model->addClause(clause);
            _propagator.addClause(clause, &_trail);

            _decision_policy->literalsOnConflict(learnt);
            _decision_policy->literalsOnConflict(
                                            reason_used_to_infer_the_conflict);

            _decision_policy->onConflict();
            _restart_policy->onConflict();
            _counters.num_failures++;

            if (_drat_proof_handler != nullptr)
                _drat_proof_handler->addClause(learnt);

            if (_restart_policy->shouldRestart()) {
                backtrack(0);
            }
        } else {
            if (_trail.index() == _num_variables)
                break;

            Literal decision = _decision_policy->nextBranch();
            enqueueNewDecision(decision);
            _counters.num_branches++;
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

        std::cout << "Num decisions : " << _counters.num_branches << std::endl;
        std::cout << "Num conflicts : " << _counters.num_failures << std::endl;

        return SAT;
    }

    return UNKNOWN;
}

void Solver::enqueueNewDecision(Literal true_literal) {
    _trail.newDecisionLevel();
    _trail.enqueueSearchDecision(true_literal);
}

void Solver::setDratProofHandler(DratProofHandler *drat_proof_handler) {
    _drat_proof_handler = drat_proof_handler;
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

    DCHECK_LT(index, old_index);
    for (unsigned int i = index; i < old_index; i++) {
        Literal l = _trail[i];
        _decision_policy->onUnassignLiteral(l);
    }

    // LOG(INFO) << "trail remove from " << index << " to " << old_index;
    // LOG(INFO) << _trail.debugString();
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
