// Copyright 2017 Hakan Metin - LIP6

#include "sattools/Breaker.h"

namespace sat {

Breaker::Breaker(const Group& group, CNFModel* model, Assignment *assignment) :
    _group(group),
    _model(model),
    _assignment(assignment) {
    _order = std::make_unique<Order>();

    for (const std::unique_ptr<Permutation>& perm : _group.permutations()) {
        std::unique_ptr<BreakerInfo> info =
            std::make_unique<BreakerInfo>(perm, *_assignment);
        _breakers.push_back(std::move(info));
    }
}

Breaker::~Breaker() {
}


void Breaker::symsimp() {
    _order_generator = std::make_unique<OrderGenerator>(*_model, _group);
    _big = std::make_unique<BIG>(*_model);

    while (fillOrderWithScore()) {
        for (std::unique_ptr<BreakerInfo> & breaker : _breakers)
            breaker->generateSBP(&_injector);

        Literal unit;
        for (const std::vector<Literal>& literals : _injector) {
            CHECK_EQ(literals.size(), 2);
            Literal a = literals[0];
            Literal b = literals[1];
            if (_big->isUnitViaResolution(a, b, &unit))
                updateAssignment(unit);
            if (_assignment->literalIsFalse(a))
                updateAssignment(b);
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

    LOG(INFO) << "Breaker add " << count << " units";
}

bool Breaker::updateAssignment(Literal literal) {
    if (!_assignment->literalIsAssigned(literal)) {
        _assignment->assignFromTrueLiteral(literal);
        updateOrder(literal);

        for (std::unique_ptr<BreakerInfo> & breaker : _breakers)
            breaker->assignmentIsUpdated();

        return true;
    }

    return false;
}



bool Breaker::addUnitInOrder(Literal unit) {
    bool added = false;

    if (updateOrder(unit)) {
        added = true;
    }

    return added;
}

bool Breaker::fillOrderWithScore() {
    for (const std::pair<double, PermCycleInfo> & p : *_order_generator) {
        PermCycleInfo info = p.second;
        unsigned int perm = info.perm;
        const std::unique_ptr<BreakerInfo>& breaker = _breakers[perm];
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

void Breaker::addFullCycleInOrder(unsigned int perm,
                                  const Literal& literal) {
    const std::unique_ptr<Permutation>& p = _group.permutation(perm);

    Literal image = p->imageOf(literal);
    while (image != literal) {
        updateOrder(image);
        image = p->imageOf(image);
    }
}

bool Breaker::updateOrder(Literal literal) {
    if (literal.isNegative())
        literal = literal.negated();

    if (!_order->add(literal))
        return false;

    for (unsigned int idx : _group.watch(literal))
        _breakers[idx]->addLookupLiteral(literal);

    return true;
}

void Breaker::generateSBPs() {
    for (std::unique_ptr<BreakerInfo> & breaker : _breakers) {
        breaker->generateSBP(&_injector);
    }
}

std::string Breaker::debugString() const {
    std::stringstream ss;

    ss << _order->debugString() <<  std::endl;

    for (unsigned int i = 0; i < _breakers.size(); i++) {
        const std::unique_ptr<BreakerInfo>& info = _breakers[i];
        ss << "[" << i << "] : " << info->debugString() << std::endl;
    }
    return ss.str();
}


}  // namespace sat
