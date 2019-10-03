// Copyright 2017 Hakan Metin

#include "sattools/IncreaseDecisionPolicy.h"

namespace sat {

IncreaseDecisionPolicy::IncreaseDecisionPolicy(const Trail& trail) :
    DecisionPolicy(trail) {
}

IncreaseDecisionPolicy::~IncreaseDecisionPolicy() {
}

void IncreaseDecisionPolicy::increaseNumVariables(unsigned int num_variables) {
    const unsigned int old_num_variables = _variables.size().value();
    DCHECK_GE(num_variables, old_num_variables);

    _var_polarity.resize(num_variables);
    _var_use_phase_saving.resize(num_variables, kUsePhaseSaving);

    for (BooleanVariable var(old_num_variables); var < num_variables; ++var)
        _variables.PushBack(!_trail.assignment().variableIsAssigned(var));
}

void IncreaseDecisionPolicy::initializeVariableOrdering() {
    const unsigned int num_variables = _variables.size().value();
    const Assignment& assignment = _trail.assignment();

    std::vector<BooleanVariable> variables;

    for (BooleanVariable var(0); var < num_variables; ++var) {
        _variables.Set(var, !assignment.variableIsAssigned(var));
    }
}

Literal IncreaseDecisionPolicy::nextBranch() {
    BooleanVariable first;
    bool polarity = kDefaultPolarity;

    const Assignment& assignment = _trail.assignment();

    for (BooleanVariable var : _variables) {
        if (assignment.variableIsAssigned(var))
            continue;  // cannot modify bitset during iterate over
        first = var;
    }
    _variables.Clear(first);

    if (_var_use_phase_saving[first])
        polarity = _trail.info(first).last_polarity;


    return Literal(first, polarity);
}

void IncreaseDecisionPolicy::onConflict() {
    // _variable_activity_increment *= (1.0 / kVariableActivityDecay);
    // _seen.assign(_activities.size(), false);
}


void IncreaseDecisionPolicy::onUnassignLiteral(Literal literal) {
    _variables.Set(literal.variable());
}


}  // namespace sat
