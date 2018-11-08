// Copyright 2017 Hakan Metin - LIP6

#include "sattools/Simplifier.h"

namespace sat {

Simplifier::Simplifier(const Group &group, CNFModel *model, Order *order)
    : _group(group),
      _model(model) {
    const unsigned int num_vars = model->numberOfVariables();

    _assignment.resize(num_vars);
    _order_manager = std::make_unique<OrderManager>(*_model, _group, order);
    _breaker_manager = std::make_unique<BreakerManager>(_group, _assignment);
}

Simplifier::~Simplifier() {
}


void Simplifier::simplify() {
    Literal next;
    std::vector<bool> actives;
    bool hasNext;
    ClauseInjector injector;
    bool is_model_unsat = false;

    bool is_unit;
    Literal unit;

    LOG(INFO) << "Simplifier start";

    _big = std::make_unique<BinaryImplicationGraph>(*_model);
    _order_manager->initialize();

    while (true) {
        _breaker_manager->activeBreakers(&actives);

        hasNext = _order_manager->nextLiteral(actives, &next);
        if (!hasNext)
            break;

        _breaker_manager->updateOrder(next);
        while (!is_model_unsat && _breaker_manager->generateSBPs(&injector)) {
            for (const std::vector<Literal>& clause : injector) {
                CHECK_EQ(clause.size(), 2);
                Literal a = clause[0];
                Literal b = clause[1];

                is_unit = _big->isUnitViaResolution(a, b, &unit);
                if (!is_unit)
                    continue;

                if (!addUnitClause(unit)) {
                    is_model_unsat = true;
                    break;
                }

                _breaker_manager->activeBreakers(&actives);
                extendsOrder(actives, unit);
            }
        }

        if (is_model_unsat)
            break;
    }

    if (is_model_unsat) {
        LOG(INFO) << "MODEL IS UNSAT";
    }

    unsigned int num_vars = _assignment.numberOfVariables();
    unsigned int count = 0;
    for (BooleanVariable var(0); var < num_vars; ++var) {
        if (_assignment.variableIsAssigned(var)) {
            std::vector<Literal> clause =
                {_assignment.getTrueLiteralForAssignedVariable(var)};
            _model->addClause(&clause);
            count++;
        }
    }

    LOG(INFO) << "Simplifier produces " << count << " units";

    _order_manager->completeOrder();
}

void Simplifier::extendsOrder(const std::vector<bool>& actives, Literal unit) {
    bool accepted;
    Literal next;

    for (unsigned int index : _group.watch(unit)) {
        if (!actives[index])
            continue;

        accepted = _order_manager->suggestLiteralInOrder(unit, &next);
        if (accepted)
            _breaker_manager->updateOrder(next);

        const std::unique_ptr<Permutation>& perm = _group.permutation(index);
        Literal image = perm->imageOf(unit);
        while (image != unit) {
            accepted = _order_manager->suggestLiteralInOrder(image, &next);
            if (accepted)
                _breaker_manager->updateOrder(next);

            image = perm->imageOf(image);
        }
    }
}

bool Simplifier::addUnitClause(Literal unit) {
    if (_assignment.literalIsTrue(unit.negated()))
        return false;
    if (!_assignment.literalIsAssigned(unit))
        _assignment.assignFromTrueLiteral(unit);

    _breaker_manager->updateAssignment(unit);

    return true;
}


}  // namespace sat
/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * End:
 */
