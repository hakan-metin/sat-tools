#include "sattools/VSIDSDecisionPolicy.h"

namespace sat {

VSIDSDecisionPolicy::VSIDSDecisionPolicy(const Trail& trail) :
    DecisionPolicy(trail),
    _var_ordering_initialised(false) {
}

VSIDSDecisionPolicy::~VSIDSDecisionPolicy() {
}

static const float kInitialVariableActivity = 0.0;
static const bool kDefaultPolarity = false;

void VSIDSDecisionPolicy::increaseNumVariables(unsigned int num_variables) {
    const unsigned int old_num_variables = _activities.size();
    DCHECK_GE(num_variables, old_num_variables);

    _activities.resize(num_variables, kInitialVariableActivity);
    _pq_need_update_for_var_at_trail_index.IncreaseSize(num_variables);

    _var_polarity.resize(num_variables);
    static const bool kUsePhaseSaving = true;  // Put in Parameters
    _var_use_phase_saving.resize(num_variables, kUsePhaseSaving);

    if (_var_ordering_initialised) {
        for (BooleanVariable var(old_num_variables); var < num_variables; ++var)
            _var_ordering.add({var, _activities[var]});
    }
}


void VSIDSDecisionPolicy::initializeVariableOrdering() {
    const unsigned int num_variables = _activities.size();
    const Assignment& assignment = _trail.assignment();

    _var_ordering.clear();
    std::vector<BooleanVariable> variables;

    for (BooleanVariable var(0); var < num_variables; ++var) {
        if (assignment.variableIsAssigned(var))
            continue;
        if (_activities[var] > kInitialVariableActivity) {
            _var_ordering.add({var, _activities[var]});
        } else {
            variables.push_back(var);
        }
    }

    // This allow to have diffent order when initialised
    // Add the variables without activity to the queue (in the default order)
    for (const BooleanVariable var : variables) {
        _var_ordering.add({var, kInitialVariableActivity});
    }

    _pq_need_update_for_var_at_trail_index.ClearAndResize(num_variables);
    _pq_need_update_for_var_at_trail_index.SetAllBefore(_trail.index());

    _var_ordering_initialised = true;
}


void VSIDSDecisionPolicy::clauseOnConflictReason(const Clause *clause) {
    const float max_activity_value = 1e100;

    for (const Literal literal : *clause) {
        const BooleanVariable var = literal.variable();
        const AssignmentInfo &info = _trail.info(var);
        const unsigned int level = info.level;

        if (level == 0)
            continue;

        _activities[var] += _variable_activity_increment;
        _pq_need_update_for_var_at_trail_index.Set(info.trail_index);

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

    DCHECK(!_var_ordering.empty());

    var = _var_ordering.top().var;
    const Assignment &assignment = _trail.assignment();
    while (assignment.variableIsAssigned(var)) {
        _var_ordering.pop();
        const unsigned int trail_index = _trail.info(var).trail_index;
        _pq_need_update_for_var_at_trail_index.Set(trail_index);

        DCHECK(!_var_ordering.empty());
        var = _var_ordering.top().var;
    }

    // TODO(hakan) Add random polarity

    if (_var_use_phase_saving[var])
        polarity = _trail.info(var).last_polarity;


    return Literal(var, polarity);
}

void VSIDSDecisionPolicy::onConflict() {
    static const double kVariableActivityDecay = 0.91;
    _variable_activity_increment *= (1.0 / kVariableActivityDecay);
}


void VSIDSDecisionPolicy::rescaleVariableActivities(float scaling_factor) {
    _variable_activity_increment *= scaling_factor;
    for (BooleanVariable var(0); var < _activities.size(); ++var)
        _activities[var] *= scaling_factor;

    _var_ordering_initialised = false;
}

}  // namespace sat
