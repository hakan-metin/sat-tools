// Copyright 2017 Hakan Metin - LIP6

#include "sattools/OrderManager.h"

namespace sat {

OrderManager::OrderManager(const CNFModel& model, const Group &group,
                           Order *order) :
    _model(model),
    _group(group),
    _order(order) {
    _order_scoring = std::make_unique<OrderScoring>(model, group);
}

OrderManager::~OrderManager() {
}

void OrderManager::initialize() {
    _order_scoring->initialize();
}

bool
OrderManager::nextLiteral(const std::vector<bool>& actives, Literal *next) {
    for (const std::pair<double, PermCycleInfo> & p : *_order_scoring) {
        PermCycleInfo info = p.second;
        unsigned int perm_index = info.perm;

        if (!actives[perm_index])
            continue;

        *next = _order_scoring->minimalOccurence(info);
        if (next->isNegative())
            *next = next->negated();

        if (!_order->add(*next))
            continue;

        return true;
    }
    return false;
}

bool OrderManager::suggestLiteralInOrder(Literal unit, Literal *next) {
    Literal positive = unit;
    if (unit.isNegative())
        positive = unit.negated();

    *next = positive;

    return _order->add(positive);
}

void OrderManager::completeOrder() {
    for (BooleanVariable var(0); var < _model.numberOfVariables(); ++var)
        _order->add(Literal(var, true));
}


}  // namespace sat
