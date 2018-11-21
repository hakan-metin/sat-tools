// Copyright 2017 Hakan Metin - LIP6

#ifndef SATTOOLS_SYM_BREAKER_H_
#define SATTOOLS_SYM_BREAKER_H_

#include <vector>
#include <memory>
#include <unordered_set>
#include <utility>
#include <string>
#include <sstream>

#include "sattools/sat/Assignment.h"
#include "sattools/sat/CNFModel.h"
#include "sattools/sat/ClauseInjector.h"
#include "sattools/sym/Group.h"
#include "sattools/sat/Literal.h"
#include "sattools/sym/Order.h"
#include "sattools/base/Macros.h"

namespace sattools {

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

}  // namespace sattools
#endif  // SATTOOLS_SYM_BREAKER_H_
/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * End:
 */
