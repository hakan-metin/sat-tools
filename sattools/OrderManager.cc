// Copyright 2017 Hakan Metin - LIP6

#include "sattools/OrderManager.h"

namespace {
struct OccLt {
    const std::vector<int64>& values;
    bool operator() (int i, int j) {
        LOG(INFO) << i << " " << j;
        if (values[i] != values[j])
                return values[i] > values[j];
        return i < j;
    }
    explicit OccLt(const std::vector<int64>& v) :  values(v) {}
};
}  // namespace

namespace sat {

OrderManager::OrderManager(const CNFModel& model, const Group &group,
                           Order *order) :
    _model(model),
    _group(group),
    _order(order) {
    // Order Score
    _order_scoring = std::make_unique<OrderScoring>(model, group);
    _order_scoring->initialize();

    // Order Occurence
    int64 num_vars = model.numberOfVariables();
    std::vector<int64> occurences(num_vars);

    for (Clause *clause : model.clauses()) {
        for (Literal literal : *clause) {
            int64 index = literal.variable().value();
            occurences[index]++;
        }
    }
    for (int64 i = 0; i < num_vars; ++i) {
        _occurence_indexes.push_back(i);
    }
    std::sort(_occurence_indexes.begin(),
              _occurence_indexes.end(), OccLt(occurences));
}

OrderManager::~OrderManager() {
}


bool
OrderManager::nextLiteral(const std::vector<bool>& actives, Literal *next) {
    // Try to add with heuristic scoring

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

    return _order->add(*next);
}

bool OrderManager::suggestLiteralWithOcc(Literal *next) {
    // Complete with occurences
    // Really not optimized
    int64 num_vars = _model.numberOfVariables();
    for (int64 i = 0; i < num_vars; ++i) {
        *next = Literal(BooleanVariable(_occurence_indexes[i]), true);
        if (_order->add(*next)) {
            return true;
        }
    }
    return false;
}


}  // namespace sat
