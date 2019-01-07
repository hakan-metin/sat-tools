// Copyright 2017 Hakan Metin - LIP6

#ifndef SATTOOLS_DECISIONPOLICY_H_
#define SATTOOLS_DECISIONPOLICY_H_


#include "sattools/Trail.h"

namespace sat {

class DecisionPolicy {
 public:
    explicit DecisionPolicy(const Trail& trail);
    virtual ~DecisionPolicy();

    Literal next();

    void onConflict(Literal x);
    void onRestart();

    void reset();

 private:
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
