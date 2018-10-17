// Copyright 2017 Hakan Metin - LIP6

#ifndef SATTOOLS_BREAKER_H_
#define SATTOOLS_BREAKER_H_

#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <memory>
#include <utility>

#include "sattools/CNFModel.h"
#include "sattools/Group.h"
#include "sattools/Literal.h"
#include "sattools/Order.h"


namespace sat {

class Breaker {
 public:
    Breaker();
    virtual ~Breaker();

    void addUnits(CNFModel& model, const Group& group);

 private:
    std::unordered_map<Literal, std::unordered_set<Literal> > _resolutions;
    std::unique_ptr<Order> _order;

};


}  // namespace sat
#endif  // SATTOOLS_BREAKER_H_
/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * End:
 */