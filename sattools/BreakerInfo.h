// Copyright 2017 Hakan Metin - LIP6

#ifndef SATTOOLS_BREAKERINFO_H_
#define SATTOOLS_BREAKERINFO_H_

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

class BreakerInfo {
 public:
    BreakerInfo(const std::unique_ptr<Permutation>&permutation,
                const Assignment& assignment);
    ~BreakerInfo();

    void addLookupLiteral(Literal literal);
    void assignmentIsUpdated();
    void generateSBP(ClauseInjector *injector);

    bool isStable() const;
    bool isActive() const;

    std::string debugString() const;

 private:
    const std::unique_ptr<Permutation>& _permutation;
    const Assignment &_assignment;

    std::unordered_set<Literal> _used;
    std::vector<Literal> _lookup;
    unsigned int _lookup_index;
    bool _already_done;

    DISALLOW_COPY_AND_ASSIGN(BreakerInfo);
};

}  // namespace sat
#endif  // SATTOOLS_BREAKERINFO_H_
/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * End:
 */
