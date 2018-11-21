// Copyright 2017 Hakan Metin - LIP6

#include "sattools/sym/Breaker.h"

namespace sattools {

Breaker::Breaker(const std::unique_ptr<Permutation>&permutation,
                         const Assignment& assignment) :
    _permutation(permutation),
    _assignment(assignment),
    _lookup_index(0),
    _already_generated(false) {
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

bool Breaker::isActive() const {
    return _lookup_index < _lookup.size();
}

bool Breaker::isStable() const {
    if (!isActive())
        return true;

    Literal element = _lookup[_lookup_index];
    Literal image = _permutation->imageOf(element);

    while (element != image) {
        if (!_assignment.hasSameAssignmentValue(element, image))
            return false;
        image = _permutation->imageOf(image);
    }

    return _assignment.literalIsFalse(element);
}


void Breaker::assignmentIsUpdated() {
    while (isActive() && isStable()) {
        _lookup_index++;
        _already_generated = false;
    }
}

bool Breaker::generateSBP(ClauseInjector *injector) {
    if (_already_generated || !isActive())
        return false;

    Literal literal = _lookup[_lookup_index];

    Literal negate = literal.negated();
    Literal image = _permutation->imageOf(negate);

    while (image != negate) {
        injector->addClause({literal, image});
        image = _permutation->imageOf(image);
    }

    _already_generated = true;
    return true;
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


}  // namespace sattools
/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * End:
 */