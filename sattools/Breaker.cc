// Copyright 2017 Hakan Metin - LIP6

#include "sattools/Breaker.h"

namespace sat {

Breaker::Breaker(const CNFModel& model, const Group& group,
                 const Assignment& assignment) :
    _model(model),
    _group(group),
    _assignment(assignment) {

    for (const std::unique_ptr<Permutation>& perm : _group.permutations()) {
        _breakers.emplace_back(BreakerInfo(perm, _assignment));
    }
}

Breaker::~Breaker() {
}


bool Breaker::updateOrder(Literal literal) {
    if (!_order->add(literal))
        return false;

    for (unsigned int idx : _group.watch(literal))
        _breakers[idx].addLookupLiteral(literal);

    return true;
}




}  // namespace sat
