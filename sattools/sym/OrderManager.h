// Copyright 2017 Hakan Metin - LIP6

#ifndef SATTOOLS_SYM_ORDERMANAGER_H_
#define SATTOOLS_SYM_ORDERMANAGER_H_

#include <memory>
#include <vector>
#include <utility>

#include "sattools/sym/Order.h"
#include "sattools/sym/OrderScoring.h"


namespace sattools {

class OrderManager {
 public:
    OrderManager(const CNFModel& model, const Group &group);
    ~OrderManager();

    void initialize();
    bool nextLiteral(const std::vector<bool>& active, Literal *next);
    bool suggestLiteralInOrder(Literal unit, Literal *next);

 private:
    const CNFModel& _model;
    const Group &_group;

    std::unique_ptr<Order> _order;
    std::unique_ptr<OrderScoring> _order_scoring;

    DISALLOW_COPY_AND_ASSIGN(OrderManager);
};

}  // namespace sattools


#endif  // SATTOOLS_SYM_ORDERMANAGER_H_
/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * End:
 */
