// Copyright 2017 Hakan Metin - LIP6

#include "sattools/Breaker.h"

namespace sat {

Breaker::Breaker(const std::unique_ptr<Permutation>&permutation,
                         const Assignment& assignment) :
    _permutation(permutation),
    _assignment(assignment),
    _lookup_index(0),
    _already_done(false) {
}

Breaker::~Breaker() {
}

void Breaker::addLookupLiteral(Literal literal) {
    if (_used.find(literal) != _used.end())
        return;

    _lookup.push_back(literal);

    // Add all cycle
    Literal image = _permutation->imageOf(literal);
    _used.insert(literal);
    while (image != literal) {
        _used.insert(image);
        image = _permutation->imageOf(image);
    }
}


bool Breaker::isStable() const {
    if (!isActive())
        return true;

    Literal element = _lookup[_lookup_index];

    // TODO(hakan): this code can be removed all image of element will be
    // units by definition of the algortithm - PROVE IT BEFORE REMOVE
    Literal image = _permutation->imageOf(element);

    while (element != image) {
        if (!_assignment.hasSameAssignmentValue(element, image))
            return false;
        image = _permutation->imageOf(image);
    }
    //

    return _assignment.literalIsFalse(element);
}

bool Breaker::isActive() const {
    return _lookup_index < _lookup.size();
}


void Breaker::assignmentIsUpdated() {
    if (isStable()) {
        if (isActive())
            _lookup_index++;
        _already_done = false;
    }
}

void Breaker::generateSBP(ClauseInjector *injector) {
    if (_already_done)
        return;

    for (; _lookup_index < _lookup.size(); _lookup_index++) {
        Literal literal = _lookup[_lookup_index];

        Literal negate = literal.negated();
        Literal image = _permutation->imageOf(negate);

        while (image != negate) {
            std::vector<Literal> literals = {literal, image};
            injector->addClause(std::move(literals));
            image = _permutation->imageOf(image);
        }

        if (!isStable())
            break;
    }

    _already_done = true;
}

std::string Breaker::debugString() const {
    std::stringstream ss;

    ss << " order: ";
    for (Literal literal : _lookup)
        ss << literal.debugString() << " ";
    ss << " | index: " << _lookup_index;
    ss << " | active : " << isActive();
    ss << " | stable: " << isStable();

    return ss.str();
}


}  // namespace sat
/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * End:
 */
