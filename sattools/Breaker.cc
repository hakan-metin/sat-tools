// Copyright 2017 Hakan Metin - LIP6

#include "sattools/Breaker.h"

namespace sat {

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

bool Breaker::generateStaticSBP(ClauseInjector *injector) {
    CHECK_GT(_lookup.size(), 0);

    LOG(INFO) << _permutation->debugString();

    for (const Literal& lookup : _lookup) {
        if (_assignment.literalIsFalse(lookup))  // Because T < F
            continue;
        else if (_assignment.literalIsTrue(lookup))  // Always minimal
            break;

        // Generate Base
        std::vector<Literal> sbp;
        unsigned int index = 0;
        while (_lookup[index] != lookup)
            sbp.push_back(_lookup[index++]);
        sbp.push_back(lookup);

        Literal negate = lookup.negated();
        Literal image = _permutation->imageOf(negate);

        while (image != negate) {
            sbp.push_back(image);
            for (Literal lit : sbp)
                std::cout << lit.debugString() << " ";
            std::cout << std::endl;

            injector->addClause(sbp);
            image = _permutation->imageOf(image);
        }

    }

    return false;
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
