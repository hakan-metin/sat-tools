// Copyright 2017 Hakan Metin - LIP6

#include "sattools/OrderManager.h"



namespace {
struct OrderLt {
    const std::vector<int64>& values;
    bool operator() (int i, int j) {
        if (values[i] != values[j])
                return values[i] > values[j];
        return i < j;
    }
    explicit OrderLt(const std::vector<int64>& v) :  values(v) {}
};
}  // namespace

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
    // LOG(INFO) << _order_scoring->debugString();
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

void OrderManager::completeOrderWithOccurences(const CNFModel& model) {
    // LOG(INFO) << "Before Complete Occurence: " << _order->debugString();

    int64 num_vars = model.numberOfVariables();
    std::vector<int64> indexes;
    for (int64 i = 0; i < num_vars; ++i) {
        indexes.push_back(i);
    }

    std::sort(indexes.begin(), indexes.end(), OrderLt(model.occurences()));

    for (int64 i = 0; i < num_vars; ++i)
        _order->add(Literal(BooleanVariable(indexes[i]), true));

    // LOG(INFO) << "Complete Occurence Done " << _order->debugString();
}



void OrderManager::completeOrderWithOrbits() {
    std::vector<Permutation*> Q, filter;
    std::unordered_map<BooleanVariable, int64> occurence_generators;
    Orbits orbits;
    unsigned int largestOrbit;
    BooleanVariable next;

    LOG(INFO) << "Orbits " << _order->debugString();

    for (const std::unique_ptr<Permutation>& perm : _group.permutations())
        Q.push_back(perm.get());

    while (Q.size() > 0) {
        // Compute occurence in remain generators
        occurence_generators.clear();
        for (const Permutation* permutation : Q) {
            for (const Literal& literal : permutation->support())
                if (literal.isPositive())
                    occurence_generators[literal.variable()]++;
        }
        occurence_generators[kNoBooleanVariable] =
            std::numeric_limits<int>::max();

        // Find next variable with less occurences in the largest orbit
        orbits.assign(Q);

        largestOrbit = 0;
        next = kNoBooleanVariable;

        for (const std::vector<BooleanVariable>& orbit : orbits) {
            if (orbit.size() < largestOrbit)
                continue;
            for (const BooleanVariable& variable : orbit) {
                if (occurence_generators[variable] == 0)
                    continue;
                const int occ_v = occurence_generators[variable];
                const int occ_n = occurence_generators[next];
                if (next == kNoBooleanVariable ||
                    occ_v < occ_n || (occ_v == occ_n && variable < next)) {
                    next = variable;
                    largestOrbit = orbit.size();
                }
            }
        }

        if (next == kNoBooleanVariable) {
            LOG(ERROR) << "Problem with generators ??";
            break;
        }

        _order->add(Literal(next, true));

        // Remove permutation contains next
        filter.clear();
        const Literal next_literal = Literal(next, true);
        for (Permutation* permutation : Q)
            if (permutation->isTrivialImage(next_literal))
                filter.push_back(permutation);

        Q = filter;
    }
}

}  // namespace sat
