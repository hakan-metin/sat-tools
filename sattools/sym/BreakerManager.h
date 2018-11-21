// Copyright 2017 Hakan Metin - LIP6

#ifndef SATTOOLS_SYM_BREAKERMANAGER_H_
#define SATTOOLS_SYM_BREAKERMANAGER_H_

#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <memory>
#include <utility>
#include <string>
#include <sstream>

#include "sattools/sat/Assignment.h"
#include "sattools/sym/Breaker.h"
#include "sattools/sat/CNFModel.h"
#include "sattools/sat/ClauseInjector.h"
#include "sattools/sym/Group.h"
#include "sattools/sat/Literal.h"
#include "sattools/base/Macros.h"

namespace sattools {


class BreakerManager {
 public:
    BreakerManager(const Group& group, const Assignment &assignment);
    virtual ~BreakerManager();

    void updateOrder(Literal literal);
    void updateAssignment(Literal literal);
    bool generateSBPs(ClauseInjector *injector);

    void activeBreakers(std::vector<bool> *actives);

    std::string debugString() const;

 private:
    const Group& _group;
    const Assignment& _assignment;

    std::vector<std::unique_ptr<Breaker>> _breakers;

    DISALLOW_COPY_AND_ASSIGN(BreakerManager);
};

}  // namespace sattools
#endif  // SATTOOLS_SYM_BREAKERMANAGER_H_
/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * End:
 */
