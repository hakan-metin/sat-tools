// Copyright 2017 Hakan Metin - LIP6

#include "sattools/SymmetrySimplifier.h"

namespace sat {

SymmetrySimplifier::SymmetrySimplifier(const CNFModel &model,
                                       const Group &group,
                                       const Assignment& assignment)
    : _model(model), _group(group), _assignment(assignment) {
    _orbits.assign(group);
    _order_manager = std::make_unique<OrderManager>(_model, _group, &_order);
    _breaker_manager = std::make_unique<BreakerManager>(_group, _assignment);
}

SymmetrySimplifier::~SymmetrySimplifier() {
}

void SymmetrySimplifier::simplify(ClauseInjector *injector) {
    DCHECK_EQ(injector->size(), 0);

    if (_group.numberOfPermutations() == 0)
        return;

    _breaker_manager->updateAssignmentForAll();
    if (addLiteralInOrderWithScore(injector))
        _breaker_manager->generateSBPs(injector);


    if (injector->size() == 0 && _order.size() != _model.numberOfVariables()) {
        addLiteralInOrderWithOccurence();
        _breaker_manager->updateAssignmentForAll();
        _breaker_manager->generateSBPs(injector);
    }
}

void SymmetrySimplifier::notifyUnit(Literal unit, ClauseInjector *injector) {
    _breaker_manager->updateAssignmentForAll();
    addLiteralInOrderWithUnit(unit, injector);
}

bool SymmetrySimplifier::addLiteralInOrderWithOccurence() {
    Literal next;

    while (_order_manager->suggestLiteralWithOcc(&next)) {
        _breaker_manager->updateOrder(next);
        LOG(INFO) << "OCc " << next.debugString();
    }

    return true;
}

bool SymmetrySimplifier::addLiteralInOrderWithScore(ClauseInjector *injector) {
    std::vector<bool> actives;
    Literal next;
    bool allSameOrbit;

    _breaker_manager->activeBreakers(&actives);
    if (!_order_manager->nextLiteral(actives, &next))
        return false;
    _breaker_manager->updateOrder(next);

    LOG(INFO) << "Next with score " << next.debugString() << std::endl <<
        _breaker_manager->debugString();

    // An optimization to add more unit
    for (Clause *clause : _model.occurenceListOf(next)) {
        allSameOrbit = true;
        // Check if all literals in clause is in orbit of next => next is unit
        for (Literal literal : *clause) {
            if (_assignment.literalIsTrue(literal) ||
                !_orbits.isInSameOrbit(next, literal)) {
                allSameOrbit = false;
                break;
            }
        }
        if (allSameOrbit) {
            injector->addClause({next});
            break;
        }
    }

    return true;
}

bool SymmetrySimplifier::addLiteralInOrderWithUnit(Literal unit,
                                                   ClauseInjector *injector) {
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
                injector->addClause({image});

                activated = true;
            }
            image = perm->imageOf(image);
        } while (image != unit);
    }

    return activated;
}

void SymmetrySimplifier::finalize() {
    // int index = _order.size();
    //LOG(INFO) << "Index " << index;
    // _order_manager->completeOrderWithOccurences(_model);
    // LOG(INFO) << _order_manager->debugString();
    LOG(INFO) << _breaker_manager->debugString();
}

}  // namespace sat
/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * End:
 */
