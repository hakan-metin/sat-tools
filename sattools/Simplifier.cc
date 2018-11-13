// Copyright 2017 Hakan Metin - LIP6

#include "sattools/Simplifier.h"

namespace sat {

Simplifier::Simplifier(const Group &group, CNFModel *model, Order *order)
    : _group(group),
      _model(model) {
    const unsigned int num_vars = model->numberOfVariables();

    _assignment.resize(num_vars);
    _order_manager = std::make_unique<OrderManager>(*_model, _group, order);
    _breaker_manager = std::make_unique<BreakerManager>(_group, _assignment);
}

Simplifier::~Simplifier() {
}


void Simplifier::simplify() {
    ClauseInjector injector;
    bool is_model_unsat = false;

    LOG(INFO) << "Simplifier start";

    _big = std::make_unique<BinaryImplicationGraph>(*_model);
    _order_manager->initialize();

    while (!is_model_unsat && addLiteralInOrderWithScore()) {
        while (_breaker_manager->generateSBPs(&injector))
            resolution(&injector);
    }
    if (is_model_unsat) {
        LOG(INFO) << "MODEL IS UNSAT";
    }

    unsigned int num_vars = _assignment.numberOfVariables();
    unsigned int count = 0;
    for (BooleanVariable var(0); var < num_vars; ++var) {
        if (_assignment.variableIsAssigned(var)) {
            std::vector<Literal> clause =
                {_assignment.getTrueLiteralForAssignedVariable(var)};
            _model->addClause(&clause);
            LOG(INFO) << clause[0].debugString();
            count++;
        }
    }

    LOG(INFO) << "Simplifier produces " << count << " units";


    std::vector<Literal> order;
    _order_manager->completeOrderWithOccurences(*_model, &order);
    for (Literal literal : order)
        _breaker_manager->updateOrder(literal);

    injector.clear();
    _breaker_manager->generateAllStaticSBP(&injector);

    for (std::vector<Literal>& clause : injector)
        _model->addClause(&clause);

}


bool Simplifier::addUnitClause(Literal unit, bool extendsOrder) {
    if (_assignment.literalIsTrue(unit.negated()))
        return false;
    if (_assignment.literalIsAssigned(unit))
        return true;

    _assignment.assignFromTrueLiteral(unit);
    _breaker_manager->updateAssignment(unit);

    LOG(INFO) << "FOUND UNIT " << unit.debugString();

    if (extendsOrder)
        addLiteralInOrderWithUnit(unit);
    return true;
}

bool Simplifier::addLiteralInOrderWithScore() {
    std::vector<bool> actives;
    Literal next;

    _breaker_manager->activeBreakers(&actives);
    if (!_order_manager->nextLiteral(actives, &next))
        return false;

    LOG(INFO) << "Add with SCORE " << next.debugString();

    _breaker_manager->updateOrder(next);
    return true;
}

bool Simplifier::addLiteralInOrderWithUnit(Literal unit) {
    std::vector<bool> actives;
    Literal next;
    bool activated = false;

    _breaker_manager->activeBreakers(&actives);
    for (unsigned int index : _group.watch(unit)) {
        if (!actives[index])
            continue;

        const std::unique_ptr<Permutation>& perm = _group.permutation(index);
        Literal image = unit;

        do {
            if (_order_manager->suggestLiteralInOrder(image, &next)) {
                LOG(INFO) << "Add with UNIT " << next.debugString();

                _breaker_manager->updateOrder(next);
                addUnitClause(image, false);
                activated = true;
            }
            image = perm->imageOf(image);
        } while (image != unit);
    }

    return activated;
}

bool Simplifier::resolution(ClauseInjector *injector) {
    bool activated = false;
    Literal unit;

    for (const std::vector<Literal>& clause : *injector) {
        if (_big->resolve(clause, &unit)) {
            addUnitClause(unit, true);
            activated = true;
        }
    }
    return activated;
}

}  // namespace sat
/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * End:
 */
