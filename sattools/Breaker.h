// Copyright 2017 Hakan Metin - LIP6

#ifndef SATTOOLS_BREAKER_H_
#define SATTOOLS_BREAKER_H_

#include <vector>
#include <memory>
#include <unordered_set>
#include <utility>
#include <string>
#include <sstream>

#include "sattools/Assignment.h"
#include "sattools/CNFModel.h"
#include "sattools/ClauseInjector.h"
#include "sattools/Group.h"
#include "sattools/Literal.h"
#include "sattools/Order.h"
#include "sattools/Macros.h"

namespace sat {

class Breaker {
 public:
    Breaker(const std::unique_ptr<Permutation>&permutation,
                const Assignment& assignment);
    ~Breaker();

    void addLookupLiteral(Literal literal);
    bool generateSBP(ClauseInjector *injector);

    void assignmentIsUpdated();

    bool isStable() const;
    bool isActive() const;

    std::string debugString() const;

 private:
    const std::unique_ptr<Permutation>& _permutation;
    const Assignment &_assignment;

    std::unordered_set<Literal> _used;
    std::vector<Literal> _lookup;
    unsigned int _lookup_index;
    bool _already_generated;

    DISALLOW_COPY_AND_ASSIGN(Breaker);
};

}  // namespace sat
#endif  // SATTOOLS_BREAKER_H_
/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * End:
 */
