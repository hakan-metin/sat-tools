// Copyright 2017 Hakan Metin - LIP6

#include "sattools/Breaker.h"

namespace sat {

Breaker::Breaker(const CNFModel& model, const Group& group,
                 Assignment *assignment) :
    _model(model),
    _group(group),
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
    _order_generator = std::make_unique<OrderGenerator>(_model, _group);
    _big = std::make_unique<BIG>(_model);

    while (fillOrderWithScore()) {
        // TODO(hakan) : extends order if binary clause
        generateSBPs();


        // if (!_assignment->literalIsAssigned(-5)) {
        //     _assignment->assignFromTrueLiteral(-5);
        //     _assignment->assignFromTrueLiteral(-6);
        //     for (std::unique_ptr<BreakerInfo> & breaker : _breakers)
        //         breaker->assignmentIsUpdated();
        // }

        Literal unit;
        for (const std::vector<Literal>& literals : _injector) {
            CHECK_EQ(literals.size(), 2);
            Literal a = literals[0];
            Literal b = literals[1];
            if (_big->isUnitViaResolution(a, b, &unit)) {
                LOG(INFO) << "FOUND UNIT " << unit.debugString();
                if (!_assignment->literalIsAssigned(unit))
                    _assignment->assignFromTrueLiteral(unit);
            }
        }

        for (std::unique_ptr<BreakerInfo> & breaker : _breakers)
            breaker->assignmentIsUpdated();

        LOG(INFO) << _injector.debugString();
        _injector.clear();
    }

    LOG(INFO) << _order->debugString();
}


bool Breaker::fillOrderWithScore() {
    for (const std::pair<double, PermCycleInfo> & p : *_order_generator) {
        PermCycleInfo info = p.second;
        unsigned int perm = info.perm;
        const std::unique_ptr<BreakerInfo>& breaker = _breakers[perm];
        if (breaker->isStable()) {

            Literal literal = _order_generator->minimalOccurence(info);
            CHECK(updateOrder(literal));
            addFullCycleInOrder(info, literal);
            LOG(INFO) << _order->debugString();

            return true;
        }
    }

    return false;
}

void Breaker::addFullCycleInOrder(const PermCycleInfo& info,
                                  const Literal& literal) {
    unsigned int perm = info.perm;
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
    LOG(INFO) << "GENERATE SBPS";
    for (std::unique_ptr<BreakerInfo> & breaker : _breakers) {
        breaker->generateSBP(&_injector);
    }
}

}  // namespace sat
