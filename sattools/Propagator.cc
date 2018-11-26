// Copyright 2017 Hakan Metin - LIP6

#include "sattools/Propagator.h"

namespace sat {

Propagator::Propagator() : _propgation_trail_index(0) {
}

Propagator::~Propagator() {
}


void Propagator::attachClause(Clause *clause, Trail *trail) {
    Literal* literals = clause->literals();
    CHECK(!trail->assignment().literalIsAssigned(literals[0]));
    CHECK(!trail->assignment().literalIsAssigned(literals[1]));

    attachOnFalse(literals[0], literals[1], clause);
    attachOnFalse(literals[1], literals[0], clause);
}

void
Propagator::attachOnFalse(Literal literal, Literal blocking, Clause *clause) {
    _watchers[literal.index()].emplace_back(clause, blocking);
}


void Propagator::detachClause(Clause *clause) {
    LOG(FATAL) << "Not yet Implemented";
    clause;  // remove compile warning
}

bool Propagator::propagate(Trail *trail) {
    const unsigned int index = trail->index();

    while (_propgation_trail_index < index) {
        const Literal literal = (*trail)[_propgation_trail_index++];
        if (!propagateOnFalse(literal.negated(), trail))
            return false;
    }
    return true;
}


bool Propagator::propagateOnFalse(Literal false_literal, Trail *trail) {

    return true;
}


}  // namespace sat

/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * End:
 */
