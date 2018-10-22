// Copyright 2017 Hakan Metin - LIP6

#ifndef INCLUDE_SATTOOLS_CLAUSEINJECTOR_H_
#define INCLUDE_SATTOOLS_CLAUSEINJECTOR_H_


#include <deque>
#include <unordered_set>
#include <vector>
#include <unordered_map>
#include <utility>
#include <string>
#include <sstream>

#include "sattools/Clause.h"
#include "sattools/Macros.h"
#include "sattools/Literal.h"
#include "sattools/Logging.h"

namespace sat {

class ClauseInjector {
 public:
    ClauseInjector() {}
    ~ClauseInjector() {}

    void addClause( std::vector<Literal> && literals) {
        _clauses.emplace_back(literals);
    }

    std::string debugString() const {
        std::stringstream ss;
        for (const std::vector<Literal>& literals : _clauses) {
            for (const Literal &literal : literals) {
                ss << literal.debugString() << " ";
            }
            ss << std::endl;
        }
        return ss.str();
    }
 private:
    std::vector<std::vector<Literal> > _clauses;
};

}  // namespace sat

#endif  // INCLUDE_SATTOOLS_CLAUSEINJECTOR_H_
/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * End:
 */
