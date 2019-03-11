// Copyright 2017 Hakan Metin

#ifndef SATTOOLS_CNFSTATS_H_
#define SATTOOLS_CNFSTATS_H_

#include <algorithm>
#include <memory>
#include <vector>

#include "sattools/Clause.h"
#include "sattools/Literal.h"
#include "sattools/Logging.h"
#include "sattools/Stats.h"
#include "sattools/IntTypeIndexedVector.h"

namespace sat {

class CNFStats {
 public:
    CNFStats();
    ~CNFStats();

    bool addClause(std::vector<Literal>* literals);
    void addClause(Clause *clause);

    int64 numberOfVariables() const { return _num_variables + 1; }
    int64 numberOfClauses() const { return _clauses.size(); }

    int64 numberOfUnaryClauses()   const { return _num_unary_clauses;   }
    int64 numberOfBinaryClauses()  const { return _num_binary_clauses;  }
    int64 numberOfTernaryClauses() const { return _num_ternary_clauses; }
    int64 numberOfLargeClauses()   const { return _num_large_clauses;   }


    const std::vector<Clause*>& clauses() const { return _clauses; }

    void printStats() { _stats.print(true); }
 private:
    int64 _num_variables;
    int64 _num_trivial_clauses;

    int64 _num_unary_clauses;
    int64 _num_binary_clauses;
    int64 _num_ternary_clauses;
    int64 _num_large_clauses;

    struct BaseStats : public StatsGroup {
        BaseStats() : StatsGroup("CNF Statistics"),
                      num_unary_clauses("num unary clauses", this),
                      num_binary_clauses("num binary clauses", this),
                      num_ternary_clauses("num ternary clauses", this),
                      num_large_clauses("num large clauses", this) {}

        CounterStat num_unary_clauses;
        CounterStat num_binary_clauses;
        CounterStat num_ternary_clauses;
        CounterStat num_large_clauses;
    };

    BaseStats _stats;


    // The set of all clauses.
    std::vector<Clause*> _clauses;

    DISALLOW_COPY_AND_ASSIGN(CNFStats);
};

}  // namespace sat

#endif  // SATTOOLS_CNFSTATS_H_
/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * End:
 */
