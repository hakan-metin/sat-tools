// Copyright 2017 Hakan Metin - LIP6

#ifndef SATTOOLS_VSIDSDECISIONPOLICY_H_
#define SATTOOLS_VSIDSDECISIONPOLICY_H_

#include "sattools/Bitset.h"
#include "sattools/DecisionPolicy.h"
#include "sattools/Trail.h"
#include "sattools/IntTypeIndexedVector.h"
#include "sattools/IntegerPriorityQueue.h"

namespace sat {

class VSIDSDecisionPolicy : public DecisionPolicy {
 public:
    VSIDSDecisionPolicy(const Trail& trail);
    virtual ~VSIDSDecisionPolicy();

    void increaseNumVariables(unsigned int num_variables) override;

    Literal nextBranch() override;

    // void onUnassignLiteral(Literal x);
    void onConflict();
    // void onRestart();

    void clauseOnConflictReason(const Clause *clause);

    // void resetDecisionHeuristics();

 private:
    bool _var_ordering_initialised;

    BitQueue64 _pq_need_update_for_var_at_trail_index;

    // Increment used to bump the variable activities.
    float _variable_activity_increment = 1.0;

    ITIVector<BooleanVariable, float> _activities;

    ITIVector<BooleanVariable, bool> _var_use_phase_saving;
    ITIVector<BooleanVariable, bool> _var_polarity;

    struct WeightVarQueueElement {
        WeightVarQueueElement(BooleanVariable v, float w) : var(v), weight(w) {}

        unsigned int index() const { return var.value(); }

        bool operator<(const WeightVarQueueElement& other) const {
            return weight < other.weight;
        }
        BooleanVariable var;
        float weight;
    };
    IntegerPriorityQueue<WeightVarQueueElement> _var_ordering;

    void initializeVariableOrdering();
    void rescaleVariableActivities(float scaling_factor);
};

}  // namespace sat


#endif  // SATTOOLS_VSIDSDECISIONPOLICY_H_
/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * End:
 */
