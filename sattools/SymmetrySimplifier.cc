// Copyright 2017 Hakan Metin - LIP6

#include "sattools/SymmetrySimplifier.h"

namespace sat {

SymmetrySimplifier::SymmetrySimplifier(const Group &group, CNFModel *model, Order *order)
    : _group(group),  _model(model) {
    const unsigned int num_vars = model->numberOfVariables();

    _propagator.resize(num_vars);
    _trail.resize(num_vars);

    _order_manager = std::make_unique<OrderManager>(*_model, _group, order);
    _breaker_manager = std::make_unique<BreakerManager>(_group,
                                                        _trail.assignment());
}

SymmetrySimplifier::~SymmetrySimplifier() {
}


void SymmetrySimplifier::init() {
    // for (Clause* clause : _model->clauses()) {
    //     if (clause->size() == 1) {
    //         addUnitClause(clause->literals()[0], false);
    //         continue;
    //     }
    //     _propagator.addClause(clause, &_trail);

    //     for (Literal literal : *clause)
    //         _clauses_map[literal].push_back(clause);
    // }

    // _orbits.assign(_group);
    // // LOG(INFO) << _orbits.debugString();

    // _big = std::make_unique<BinaryImplicationGraph>(*_model);
    // _order_manager->initialize();
}

void SymmetrySimplifier::simplify() {
    ClauseInjector injector;
    Orbits orbits;

    if (_group.numberOfPermutations() == 0)
        return;

    init();

    _propagator.propagate(&_trail);
    _breaker_manager->updateAssignmentForAll();


    LOG(INFO) << "SymmetrySimplifier start";

    while (addLiteralInOrderWithScore()) {
        while (_breaker_manager->generateSBPs(&injector))
            resolution(&injector);
    }

    for (unsigned int i = 0; i < _trail.index(); i++) {
        std::vector<Literal> clause = {_trail[i]};
        _model->addClause(&clause);
    }

    LOG(INFO) << "SymmetrySimplifier produces " << _trail.index() <<
        " units (including unit propagation): ";
    // LOG(INFO) << _trail.debugString();

    // LOG(INFO) << _group.debugString();
    // LOG(INFO) << _breaker_manager->debugString();

    _order_manager->completeOrderWithOccurences(*_model);
}


bool SymmetrySimplifier::addUnitClause(Literal unit, bool extendsOrder) {
    const Assignment &assignment = _trail.assignment();

    if (assignment.literalIsTrue(unit.negated()))
        return false;
    if (assignment.literalIsAssigned(unit))
        return true;

    _trail.enqueueWithUnitReason(unit);
    if (!_propagator.propagate(&_trail))
        return false;

    _breaker_manager->updateAssignmentForAll();

    if (extendsOrder)
        addLiteralInOrderWithUnit(unit);
    return true;
}


bool SymmetrySimplifier::addLiteralInOrderWithScore() {
    std::vector<bool> actives;
    Literal next;
    bool allSameOrbit;

    _breaker_manager->activeBreakers(&actives);
    if (!_order_manager->nextLiteral(actives, &next))
        return false;

    for (Clause *clause : _clauses_map[next]) {
        allSameOrbit = true;
        // Check if all literals in clause is in orbit of next => next is unit
        for (Literal literal : *clause) {
            if (_trail.assignment().literalIsTrue(literal) ||
                !_orbits.isInSameOrbit(next, literal)) {
                allSameOrbit = false;
                break;
            }
        }
        if (allSameOrbit) {
            addUnitClause(next, false);
            break;
        }
    }



    // LOG(INFO) << "Next with score " << next.debugString();

    _breaker_manager->updateOrder(next);
    return true;
}

bool SymmetrySimplifier::addLiteralInOrderWithUnit(Literal unit) {
    std::vector<bool> actives;
    Literal next;
    bool activated = false;

    _breaker_manager->activeBreakers(&actives);
    for (unsigned int index : _group.watch(unit)) {
        if (!actives[index])
            continue;

        const std::unique_ptr<Permutation>& perm = _group.permutation(index);
        Literal image = perm->imageOf(unit);
        do {
            if (_order_manager->suggestLiteralInOrder(image, &next)) {
                _breaker_manager->updateOrder(next);
                addUnitClause(image, false);

                activated = true;
            }
            image = perm->imageOf(image);
        } while (image != unit);
    }

    return activated;
}

bool SymmetrySimplifier::resolution(ClauseInjector *injector) {
    Literal unit;

    for (const std::vector<Literal>& clause : *injector) {
        if (_big->resolve(clause, &unit)) {
            if (!addUnitClause(unit, true))
                return false;
        }
    }
    return true;
}

}  // namespace sat
/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * End:
 */
