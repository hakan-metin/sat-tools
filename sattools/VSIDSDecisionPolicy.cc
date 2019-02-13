#include "sattools/VSIDSDecisionPolicy.h"

namespace sat {

VSIDSDecisionPolicy::VSIDSDecisionPolicy(const Trail& trail) :
    DecisionPolicy(trail),
    _var_ordering_initialised(false),
    _var_order(0, ActivityLt( _activities)) {
}

VSIDSDecisionPolicy::~VSIDSDecisionPolicy() {
}

static const float kVariableActivityDecay = 0.91;
static const float kInitialVariableActivity = 0.0;
static const bool  kDefaultPolarity = false;
static const bool  kUsePhaseSaving = true;

void VSIDSDecisionPolicy::increaseNumVariables(unsigned int num_variables) {
    const unsigned int old_num_variables = _activities.size();
    DCHECK_GE(num_variables, old_num_variables);

    _activities.resize(num_variables, kInitialVariableActivity);

    _var_polarity.resize(num_variables);
    _var_use_phase_saving.resize(num_variables, kUsePhaseSaving);


    _var_order.reserve(num_variables);

    _var_ordering.reserve(num_variables);
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

    LOG(INFO) << "empty : " << _var_ordering.empty();

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

    // TODO(hakan) Add random decition (maybe in DecisionPolicy

    do {
        DCHECK(!_var_ordering.empty());
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
}


void VSIDSDecisionPolicy::onUnassignLiteral(Literal literal) {
    if (!_var_ordering_initialised)
        return;

    const BooleanVariable var = literal.variable();
    const WeightVarQueueElement element = { var, _activities[var] };

    if (_var_ordering.contains(var.value())) {
        _var_ordering.increasePriority(element);
    } else {
        _var_ordering.add(element);
    }

}

void VSIDSDecisionPolicy::rescaleVariableActivities(float scaling_factor) {
    _variable_activity_increment *= scaling_factor;
    for (BooleanVariable var(0); var < _activities.size(); ++var)
        _activities[var] *= scaling_factor;

    _var_ordering_initialised = false;
}

}  // namespace sat
