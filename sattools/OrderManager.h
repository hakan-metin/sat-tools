// Copyright 2017 Hakan Metin - LIP6

#ifndef SATTOOLS_ORDERMANAGER_H_
#define SATTOOLS_ORDERMANAGER_H_

#include <memory>
#include <vector>
#include <utility>
#include <string>

#include "sattools/CNFModel.h"
#include "sattools/Order.h"
#include "sattools/OrderScoring.h"


namespace sat {

class OrderManager {
 public:
    OrderManager(const CNFModel& model, const Group &group, Order *order);
    ~OrderManager();

    void initialize();
    bool nextLiteral(const std::vector<bool>& active, Literal *next);
    bool suggestLiteralInOrder(Literal unit, Literal *next);
    void completeOrder();
    void completeOrderWithOccurences(const CNFModel& model);
    void exportOrder(const std::string filename);

 private:
    const CNFModel& _model;
    const Group &_group;

    Order *_order;
    std::unique_ptr<OrderScoring> _order_scoring;

    DISALLOW_COPY_AND_ASSIGN(OrderManager);
};

}  // namespace sat


#endif  // SATTOOLS_ORDERMANAGER_H_
/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * End:
 */
