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

    while (true) {

        for (const std::pair<double, PermCycleInfo> & p : *_order_generator) {
            PermCycleInfo info = p.second;
            unsigned int perm = info.perm;

            const std::unique_ptr<BreakerInfo>& breaker = _breakers[perm];
            if (breaker->isStable()) {
                Literal literal = _order_generator->minimalOccurence(info);
                CHECK(updateOrder(literal));
                addFullCycleInOrder(info, literal);
            }
        }

        LOG(INFO) << _order->debugString();
        std::vector<Literal> add_order;

        for (const Literal& literal : add_order)
            updateOrder(literal);

        generateSBPs();

        LOG(INFO) << _injector.debugString();

        bool found_units = false;
        // Literal unit;
        // for (const std::vector<Literal>& literals : _injector) {
        //     if (_big.findUnits(literals, *unit)) {
        //         found_units = true;
        //         _assignment->assignFromTrueLiteral(unit);
        //     }
        // }

        if (found_units)
            update();

        break;
    }
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
    LOG(INFO) << "BREAK";
    for (std::unique_ptr<BreakerInfo> & breaker : _breakers) {
        breaker->generateSBP(&_injector);
    }
}


void Breaker::update() {
    for (std::unique_ptr<BreakerInfo> & breaker : _breakers) {
        breaker->assignmentIsUpdated();
    }
}



void Breaker::print() {
    LOG(INFO) << _injector.debugString();
}

}  // namespace sat
