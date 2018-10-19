// Copyright 2017 Hakan Metin - LIP6

#ifndef SATTOOLS_ORDERGENERATOR_H_
#define SATTOOLS_ORDERGENERATOR_H_

#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <map>
#include <memory>
#include <utility>

#include "sattools/CNFModel.h"
#include "sattools/Group.h"
#include "sattools/Literal.h"
#include "sattools/Order.h"


namespace sat {

// A comparator class
struct OrderLt {
    const std::vector<int64>& values;

    bool operator() (int i, int j) {
        if (values[i] != values[j])
                return values[i] > values[j];
        return i < j;
    }
    explicit OrderLt(const std::vector<int64>& v) : values(v) {}
};


class OrderGenerator {
 public:
    OrderGenerator(const CNFModel& model, const Group& group);
    virtual ~OrderGenerator();

    void score();
    void score_per_cycle();
    void score2();

 private:
    const CNFModel &_model;
    const Group& _group;
    std::unique_ptr<Order> _order;
};


}  // namespace sat
#endif  // SATTOOLS_ORDERGENERATOR_H_
/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * End:
 */
