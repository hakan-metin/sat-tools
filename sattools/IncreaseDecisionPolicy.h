// Copyright 2017 Hakan Metin - LIP6

#ifndef SATTOOLS_INCREASEDECISIONPOLICY_H_
#define SATTOOLS_INCREASEDECISIONPOLICY_H_

#include <algorithm>
#include <vector>

#include "sattools/Bitset.h"
#include "sattools/DecisionPolicy.h"
#include "sattools/Trail.h"
#include "sattools/IntTypeIndexedVector.h"
#include "sattools/IntegerPriorityQueue.h"

namespace sat {

class IncreaseDecisionPolicy : public DecisionPolicy {
 public:
    explicit IncreaseDecisionPolicy(const Trail& trail);
    virtual ~IncreaseDecisionPolicy();

    void increaseNumVariables(unsigned int num_variables) override;

    Literal nextBranch() override;

    void onUnassignLiteral(Literal x);
    void onConflict();
    // void onRestart();
    // void resetDecisionHeuristics();

 private:
    sat::Bitset64<BooleanVariable> _variables;

    ITIVector<BooleanVariable, bool> _var_use_phase_saving;
    ITIVector<BooleanVariable, bool> _var_polarity;

    void initializeVariableOrdering();
};

}  // namespace sat


#endif  // SATTOOLS_INCREASEDECISIONPOLICY_H_
/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * End:
 */
