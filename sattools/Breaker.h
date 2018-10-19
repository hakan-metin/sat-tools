// Copyright 2017 Hakan Metin - LIP6

#ifndef SATTOOLS_BREAKER_H_
#define SATTOOLS_BREAKER_H_

#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <memory>
#include <utility>

#include "sattools/Assignment.h"
#include "sattools/BreakerInfo.h"
#include "sattools/CNFModel.h"
#include "sattools/Group.h"
#include "sattools/Literal.h"
#include "sattools/Order.h"
#include "sattools/Macros.h"

namespace sat {


class Breaker {

 public:
    Breaker(const CNFModel& model, const Group& group,
            const Assignment& assignment);
    virtual ~Breaker();

    bool updateOrder(Literal literal);

 private:
    const CNFModel &_model;
    const Group& _group;
    const Assignment &_assignment;

    std::unique_ptr<Order> _order;
    std::vector<BreakerInfo> _breakers;
};

}  // namespace sat
#endif  // SATTOOLS_BREAKER_H_
/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * End:
 */
