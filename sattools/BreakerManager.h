// Copyright 2017 Hakan Metin - LIP6

#ifndef SATTOOLS_BREAKERMANAGER_H_
#define SATTOOLS_BREAKERMANAGER_H_

#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <memory>
#include <utility>
#include <string>
#include <sstream>

#include "sattools/Assignment.h"
#include "sattools/Breaker.h"
#include "sattools/CNFModel.h"
#include "sattools/ClauseInjector.h"
#include "sattools/Group.h"
#include "sattools/Literal.h"
#include "sattools/Macros.h"
#include "sattools/Order.h"

namespace sat {


class BreakerManager {
 public:
    BreakerManager(const Group& group, const Order& order,
                   const Assignment &assignment);
    virtual ~BreakerManager();

    void updateAssignment(Literal literal);
    void updateAssignmentForAll();
    bool generateSBPs(ClauseInjector *injector);

    void activeBreakers(std::vector<bool> *actives);

    std::string debugString() const;

 private:
    const Group& _group;
    const Assignment& _assignment;

    std::vector<std::unique_ptr<Breaker>> _breakers;

    void updateOrder(Literal literal);

    DISALLOW_COPY_AND_ASSIGN(BreakerManager);
};

}  // namespace sat
#endif  // SATTOOLS_BREAKERMANAGER_H_
/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * End:
 */
