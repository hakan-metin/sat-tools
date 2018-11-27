// Copyright 2017 Hakan Metin - LIP6

#include "sattools/Propagator.h"

namespace sat {

Propagator::Propagator() : _propgation_trail_index(0) {
}

Propagator::~Propagator() {
}


void Propagator::resize(unsigned int num_vars) {
    _watchers.resize(num_vars << 2);
    _reasons.resize(num_vars);
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
    const Assignment& assignment = trail->assignment();
    std::vector<Watch>& watchers = _watchers[false_literal.index()];
    Clause *conflict = nullptr;

    auto i = watchers.begin();
    auto j = watchers.begin();

    const auto end = watchers.end();

    LOG(INFO) << "PROP START";

    while (i != end) {
        const Watch watch = *j++ = *i++;

        if (assignment.literalIsTrue(watch.blocking_literal))
            continue;

        Literal *literals = watch.clause->literals();
        Literal other(LiteralIndex(literals[0].index().value() ^
                                   literals[1].index().value() ^
                                   false_literal.index().value()));

        literals[0] = other;
        literals[1] = false_literal;

        if (assignment.literalIsTrue(other)) {
            j[-1].blocking_literal = other;
        } else {
            const Literal *literals_end = watch.clause->end();
            Literal *k = literals + 2;
            while (k != literals_end && assignment.literalIsFalse(*k))
                k++;

            if (k != literals_end) {
                const Literal lit = *k;
                if (assignment.literalIsTrue(lit)) {
                    j[-1].blocking_literal = lit;
                } else {
                    literals[1] = lit;
                    *k = false_literal;
                    attachOnFalse(lit, other, watch.clause);
                    j--;   // drop this watch
                }
            } else {
                if (assignment.literalIsFalse(other)) {
                    conflict = watch.clause;
                    break;
                } else {  // Found unit clause
                    LOG(INFO) << "UNIT " << other.debugString();

                    _reasons[trail->index()] = watch.clause;
                    trail->enqueue(other);
                }
            }
        }
    }

    if (j < i) {
        while (i != end)
            *j++ = *i++;
        watchers.resize(j - watchers.begin());
    }

    return conflict == nullptr;
}


std::string Propagator::debugString() const {
    std::stringstream ss;

    for (const auto &watchers : _watchers) {
        for (const Watch &watch : watchers) {
            ss << watch.blocking_literal.debugString();
        }
    }

    return ss.str();
}



}  // namespace sat

/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * End:
 */
