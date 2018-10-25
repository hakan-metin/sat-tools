// Copyright 2017 Hakan Metin - LIP6

#ifndef SATTOOLS_CLAUSEINJECTOR_H_
#define SATTOOLS_CLAUSEINJECTOR_H_


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

    void addClause(std::vector<Literal> && literals) {
        _clauses.emplace_back(literals);
    }

    void clear() {
        _clauses.clear();
    }

    std::vector<std::vector<Literal> >::const_iterator begin() {
        return _clauses.begin();
    }

    std::vector<std::vector<Literal> >::const_iterator end() {
        return _clauses.end();
    }

    std::string debugString() const {
        std::stringstream ss;
        for (const std::vector<Literal>& literals : _clauses) {
            ss << std::endl;

            for (const Literal &literal : literals) {
                ss << literal.debugString() << " ";
            }
        }
        return ss.str();
    }

 private:
    std::vector<std::vector<Literal> > _clauses;
};

}  // namespace sat

#endif  // SATTOOLS_CLAUSEINJECTOR_H_
/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * End:
 */
