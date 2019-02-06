// Copyright 2017 Hakan Metin

#include "sattools/Solver.h"

namespace sat {

Solver::Solver() :
    _num_variables(0),
    _is_model_unsat(false),
    _simplifier(nullptr),
    _sym_simplifier(nullptr)
{
}


Solver::~Solver() {
}

void Solver::assign(CNFModel *model) {
    _model = model;
    setNumberOfVariables(model->numberOfVariables());

    _simplifier = std::make_unique<Simplifier>(model);
    activeSymmetries();
}


void Solver::activeSymmetries() {
    // Search symmetries
    SymmetryFinder<BlissAutomorphismFinder,
                   DoubleLiteralGraphNodeAdaptor> bliss_finder;
    bliss_finder.findAutomorphisms(*_model, &_group);
    _sym_simplifier = std::make_unique<SymmetrySimplifier>(*_model, _group,
                                                           _trail.assignment());

    LOG(INFO) << std::endl << _group.debugString();
    exit(0);
}


void Solver::setNumberOfVariables(unsigned int num_variables) {
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

void Solver::detachSatisfiedClauses() {
    for (Clause *clause : _model->clauses())
        if (isClauseSatisfied(clause))
            clause->lazyDetach();
}

Solver::Status Solver::solve() {
    ClauseInjector injector;

    LOG(INFO) << "Number of clauses initial: " << _model->numberOfClauses();
    // simplifyInitialProblem();
    // LOG(INFO) << "Number of clauses after initial simplification: "
    //           << _model->numberOfClauses();

    if (_is_model_unsat)
        return UNSAT;

    // LOG(INFO) << "TRAIL : " << _trail.debugString();

    simplifyWithSBP();


    if (_is_model_unsat)
        return UNSAT;

    LOG(INFO) << "Number of clauses after SBP simplification: "
              << _model->numberOfClauses();

    // LOG(INFO) << "TRAIL : " << _trail.debugString();

    detachSatisfiedClauses();
    _model->clearDetachedClauses();

    if (_model->numberOfClauses() == 0) {
        LOG(INFO) << "Solved by simplification SAT";
    }

    for (unsigned int i = 0; i < _trail.index(); i++) {
        std::vector<Literal> literals = { _trail[i] };
        _model->addClause(&literals);
    }
    LOG(INFO) << "Number of clauses after SBP simplification and trail: "
              << _model->numberOfClauses();


    return UNKNOWN;
}

bool Solver::simplifyInitialProblem() {
    if (_simplifier == nullptr)
        return true;

    for (Clause *clause : _model->clauses()) {
        if (clause->size() == 1) {
            Literal unit = clause->literals()[0];
            if (_trail.assignment().literalIsFalse(unit)) {
                LOG(INFO) << "CONTRADICTION";
                return setModelUnsat();
            }
            if (!_trail.assignment().literalIsAssigned(unit))
                _trail.enqueueWithUnitReason(unit);
        }
        _simplifier->addClauseToProcess(clause);
    }

    if (!_simplifier->simplify(&_trail))
        return setModelUnsat();

    return true;
}

bool Solver::simplifyWithSBP() {
    if (_sym_simplifier == nullptr)
        return true;

    ClauseInjector injector;
    unsigned int last_trail_index = 0;

    while (true) {
        _sym_simplifier->simplify(&injector);
        if (injector.size() == 0)
            break;

        addSBPIntoModel(&injector);

        if (!_simplifier->simplify(&_trail))
            return setModelUnsat();

        // Unit prolongation
        while (last_trail_index < _trail.index()) {
            for (; last_trail_index < _trail.index(); last_trail_index++)
                _sym_simplifier->notifyUnit(_trail[last_trail_index],
                                            &injector);

            addSBPIntoModel(&injector);
            if (!_simplifier->simplify(&_trail))
                return setModelUnsat();
        }
    }

    _sym_simplifier->finalize();

    return true;
}

void Solver::addSBPIntoModel(ClauseInjector *injector) {
    for (std::vector<Literal>& literals : *injector) {
        if (!_model->addClause(&literals))
            continue;

        if (_simplifier != nullptr)
            _simplifier->addClauseToProcess(_model->clauses().back());
    }

    injector->clear();
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
