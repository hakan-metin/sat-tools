// Copyright 2017 Hakan Metin - LIP6

#ifndef SATTOOLS_BREAKERMANAGER_H_
#define SATTOOLS_BREAKERMAANEGR_H_

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
#include "sattools/Order.h"
#include "sattools/OrderScoring.h"
#include "sattools/Macros.h"

namespace sat {


class BreakerManager {
 public:
    BreakerManager(const Group& group, CNFModel* model, Assignment *assignment);
    virtual ~BreakerManager();


    void symsimp();

    bool updateOrder(Literal literal);
    void generateSBPs();

    std::string debugString() const;

 private:
    const Group& _group;
    CNFModel *_model;
    Assignment *_assignment;
    ClauseInjector _injector;
    std::vector<std::unique_ptr<Breaker>> _breakers;

    std::unique_ptr<OrderScoring> _order_generator;
    std::unique_ptr<Order> _order;

    bool updateAssignment(Literal literal);

    bool addUnitInOrder(Literal unit);
    bool fillOrderWithScore();
    void addFullCycleInOrder(unsigned int perm, const Literal& literal);

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
