// Copyright 2017 Hakan Metin - LIP6

#ifndef SATTOOLS_DECISIONPOLICY_H_
#define SATTOOLS_DECISIONPOLICY_H_

#include <vector>

#include "sattools/Trail.h"

namespace sat {

static const double kVariableActivityDecay = 0.95;
static const double kInitialVariableActivity = 0.0;
static const bool  kDefaultPolarity = false;
static const bool  kUsePhaseSaving = false;

class DecisionPolicy {
 public:
    explicit DecisionPolicy(const Trail& trail) : _trail(trail) {}
    virtual ~DecisionPolicy() {}

    virtual
    void increaseNumVariables(ATTRIBUTE_UNUSED unsigned int num_variables) {}

    virtual Literal nextBranch() = 0;

    virtual void onUnassignLiteral(ATTRIBUTE_UNUSED Literal literal) {}
    virtual void onConflict() {}
    virtual void onRestart() {}

    virtual void
    literalsOnConflict(ATTRIBUTE_UNUSED const std::vector<Literal>& literals) {}

    virtual void resetDecisionHeuristics() {}

 protected:
    const Trail& _trail;
};

}  // namespace sat


#endif  // SATTOOLS_DECISIONPOLICY_H_
/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * End:
 */
