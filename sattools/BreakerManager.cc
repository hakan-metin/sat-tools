// Copyright 2017 Hakan Metin - LIP6

#include "sattools/BreakerManager.h"

namespace sat {

BreakerManager::BreakerManager(const Group& group, CNFModel* model, Assignment *assignment) :
    _group(group),
    _model(model),
    _assignment(assignment) {
    _order = std::make_unique<Order>();

    for (const std::unique_ptr<Permutation>& perm : _group.permutations()) {
        std::unique_ptr<Breaker> breaker =
            std::make_unique<Breaker>(perm, *_assignment);
        _breakers.push_back(std::move(breaker));
    }
}

BreakerManager::~BreakerManager() {
}


void BreakerManager::symsimp() {
    _order_generator = std::make_unique<OrderScoring>(*_model, _group);

    while (fillOrderWithScore()) {
        for (std::unique_ptr<Breaker> & breaker : _breakers)
            breaker->generateSBP(&_injector);

        Literal unit;
        for (const std::vector<Literal>& literals : _injector) {
            CHECK_EQ(literals.size(), 2);
            Literal a = literals[0];
            Literal b = literals[1];
            // if (_big->isUnitViaResolution(a, b, &unit))
            //     updateAssignment(unit);
            // if (_assignment->literalIsFalse(a))
            //     updateAssignment(b);
        }

        // _injector.clear();
    }

    unsigned int count = 0;
    for (BooleanVariable var(0); var < _model->numberOfVariables(); ++var) {
        if (_assignment->variableIsAssigned(var)) {
            Literal l = _assignment->getTrueLiteralForAssignedVariable(var);
            std::vector<Literal> literals = { l };
            _model->addClause(&literals);
            count++;
        }
    }

    LOG(INFO) << "BreakerManager add " << count << " units";
}

bool BreakerManager::updateAssignment(Literal literal) {
    if (!_assignment->literalIsAssigned(literal)) {
        _assignment->assignFromTrueLiteral(literal);
        updateOrder(literal);

        for (std::unique_ptr<Breaker> & breaker : _breakers)
            breaker->assignmentIsUpdated();

        return true;
    }

    return false;
}



bool BreakerManager::addUnitInOrder(Literal unit) {
    bool added = false;

    if (updateOrder(unit)) {
        added = true;
    }

    return added;
}

bool BreakerManager::fillOrderWithScore() {
    for (const std::pair<double, PermCycleInfo> & p : *_order_generator) {
        PermCycleInfo info = p.second;
        unsigned int perm = info.perm;
        const std::unique_ptr<Breaker>& breaker = _breakers[perm];
        if (breaker->isStable()) {
            Literal literal = _order_generator->minimalOccurence(info);
            if (_assignment->literalIsAssigned(literal))
                continue;

            CHECK(updateOrder(literal));
            addFullCycleInOrder(perm, literal);

            return true;
        }
    }

    return false;
}

void BreakerManager::addFullCycleInOrder(unsigned int perm,
                                  const Literal& literal) {
    const std::unique_ptr<Permutation>& p = _group.permutation(perm);

    Literal image = p->imageOf(literal);
    while (image != literal) {
        updateOrder(image);
        image = p->imageOf(image);
    }
}

bool BreakerManager::updateOrder(Literal literal) {
    if (literal.isNegative())
        literal = literal.negated();

    if (!_order->add(literal))
        return false;

    for (unsigned int idx : _group.watch(literal))
        _breakers[idx]->addLookupLiteral(literal);

    return true;
}

void BreakerManager::generateSBPs() {
    for (std::unique_ptr<Breaker> & breaker : _breakers) {
        breaker->generateSBP(&_injector);
    }
}

std::string BreakerManager::debugString() const {
    std::stringstream ss;

    ss << _order->debugString() <<  std::endl;

    for (unsigned int i = 0; i < _breakers.size(); i++) {
        const std::unique_ptr<Breaker>& info = _breakers[i];
        ss << "[" << i << "] : " << info->debugString() << std::endl;
    }
    return ss.str();
}


}  // namespace sat
