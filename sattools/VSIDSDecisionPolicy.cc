// Copyright 2017 Hakan Metin

#include "sattools/VSIDSDecisionPolicy.h"

namespace sat {

VSIDSDecisionPolicy::VSIDSDecisionPolicy(const Trail& trail) :
    DecisionPolicy(trail),
    _var_ordering_initialised(false),
    _variable_activity_increment(1.0) {
}

VSIDSDecisionPolicy::~VSIDSDecisionPolicy() {
}


void VSIDSDecisionPolicy::increaseNumVariables(unsigned int num_variables) {
    const unsigned int old_num_variables = _activities.size();
    DCHECK_GE(num_variables, old_num_variables);

    _activities.resize(num_variables, kInitialVariableActivity);

    _var_polarity.resize(num_variables);
    _var_use_phase_saving.resize(num_variables, kUsePhaseSaving);

    _seen.assign(num_variables, false);

    _var_ordering.reserve(num_variables);
    if (_var_ordering_initialised) {
        for (BooleanVariable var(old_num_variables); var < num_variables; ++var)
            _var_ordering.add({var, &_activities[var]});
    }
}

void VSIDSDecisionPolicy::initializeVariableOrdering() {
    const unsigned int num_variables = _activities.size();
    const Assignment& assignment = _trail.assignment();

    std::vector<BooleanVariable> variables;

    DCHECK_EQ(_var_ordering.capacity(), num_variables);

    for (BooleanVariable var(0); var < num_variables; ++var) {
        if (assignment.variableIsAssigned(var))
            continue;
        if (_activities[var] > 0.0) {
            _var_ordering.add({var, &_activities[var]});
        } else {
            variables.push_back(var);
        }
    }

    std::sort(variables.begin(), variables.end());

    // This allow to have diffent order when initialised
    // Add the variables without activity to the queue (in the default order)
    for (const BooleanVariable var : variables) {
        const WeightVarQueueElement element {var, &_activities[var] };
        DCHECK(!_var_ordering.contains(element.index()));
        _var_ordering.add(element);
    }

    _var_ordering_initialised = true;
}


void
VSIDSDecisionPolicy::literalsOnConflict(const std::vector<Literal>& literals) {
    const double max_activity_value = 1e100;

    for (const Literal literal : literals) {
        const BooleanVariable var = literal.variable();
        const AssignmentInfo &info = _trail.info(var);
        const unsigned int level = info.level;

        if (level == 0 || _seen[var.value()])
            continue;

        _seen[var.value()] = true;

        _activities[var] += _variable_activity_increment;

        const WeightVarQueueElement element = { var, &_activities[var] };
        _var_ordering.increasePriority(element);

        if (_var_ordering.contains(var.value())) {
            _var_ordering.increasePriority(element);
        } else {
            _var_ordering.add(element);
        }

        if (_activities[var] > max_activity_value) {
            rescaleVariableActivities(1.0 / max_activity_value);
        }
    }
}

Literal VSIDSDecisionPolicy::nextBranch() {
    if (!_var_ordering_initialised)
        initializeVariableOrdering();

    BooleanVariable var;
    bool polarity = kDefaultPolarity;
    const Assignment &assignment = _trail.assignment();

    // TODO(hakan) Add random decision (maybe in DecisionPolicy)

    do {
        DCHECK(!_var_ordering.empty());
        // LOG(INFO) << "Take next decision var " << _var_ordering.top().var
        //          << " with score " << *(_var_ordering.top().weight);
        var = _var_ordering.top().var;
        _var_ordering.pop();
    } while (assignment.variableIsAssigned(var));


    // TODO(hakan) Add random polarity

    if (_var_use_phase_saving[var])
        polarity = _trail.info(var).last_polarity;

    return Literal(var, polarity);
}

void VSIDSDecisionPolicy::onConflict() {
    _variable_activity_increment *= (1.0 / kVariableActivityDecay);
    _seen.assign(_activities.size(), false);
}


void VSIDSDecisionPolicy::onUnassignLiteral(Literal literal) {
    DCHECK(!_trail.assignment().literalIsAssigned(literal));
    if (!_var_ordering_initialised)
        return;

    const BooleanVariable var = literal.variable();
    const WeightVarQueueElement element = { var, &_activities[var] };

    if (_var_ordering.contains(var.value())) {
        _var_ordering.increasePriority(element);
    } else {
        _var_ordering.add(element);
    }
}

void VSIDSDecisionPolicy::rescaleVariableActivities(double scaling_factor) {
    _variable_activity_increment *= scaling_factor;
    for (BooleanVariable var(0); var < _activities.size(); ++var)
        _activities[var] *= scaling_factor;
}

}  // namespace sat
