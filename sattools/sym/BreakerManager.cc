// Copyright 2017 Hakan Metin - LIP6

#include "sattools/sym/BreakerManager.h"

namespace sattools {

BreakerManager::BreakerManager(const Group& group,
                               const Assignment &assignment) :
    _group(group),
    _assignment(assignment) {
    for (const std::unique_ptr<Permutation>& perm : _group.permutations()) {
        std::unique_ptr<Breaker> breaker =
            std::make_unique<Breaker>(perm, _assignment);
        _breakers.push_back(std::move(breaker));
    }
}

BreakerManager::~BreakerManager() {
}

void BreakerManager::updateOrder(Literal literal) {
    DCHECK(literal.isPositive());
    const BooleanVariable variable = literal.variable();

    for (unsigned int index : _group.watch(variable)) {
        const std::unique_ptr<Breaker>& breaker = _breakers[index];
        breaker->addLookupLiteral(literal);
    }
}

void BreakerManager::updateAssignment(Literal literal) {
    const BooleanVariable variable = literal.variable();

    for (const unsigned int index : _group.watch(variable)) {
        const std::unique_ptr<Breaker>& breaker = _breakers[index];
        breaker->assignmentIsUpdated();
    }
}

bool BreakerManager::generateSBPs(ClauseInjector *injector) {
    bool added = false;
    for (const std::unique_ptr<Breaker> & breaker : _breakers) {
        if (breaker->generateSBP(injector))
            added = true;
    }
    return added;
}

void BreakerManager::activeBreakers(std::vector<bool> *actives) {
    actives->clear();
    for (const std::unique_ptr<Breaker>& breaker : _breakers)
        actives->push_back(breaker->isStable());
}

std::string BreakerManager::debugString() const {
    std::stringstream ss;

    for (unsigned int i = 0; i < _breakers.size(); i++) {
        const std::unique_ptr<Breaker>& info = _breakers[i];
        ss << "[" << i << "] : " << info->debugString() << std::endl;
    }
    return ss.str();
}


}  // namespace sattools
