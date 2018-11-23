// Copyright 2017 Hakan Metin

#ifndef SATTOOLS_CNFMODEL_H_
#define SATTOOLS_CNFMODEL_H_

#include <algorithm>
#include <memory>
#include <vector>

#include "sattools/Clause.h"
#include "sattools/Literal.h"
#include "sattools/Logging.h"
#include "sattools/RangeIterator.h"
#include "sattools/Watcher.h"

namespace sat {

class CNFModel {
 public:
    CNFModel();
    ~CNFModel();

    void addClause(std::vector<Literal>* literals);



    int64 numberOfVariables() const { return _num_variables + 1; }
    int64 numberOfClauses() const { return _clauses.size(); }

    int64 numberOfBinaryClauses() const { return _num_binary_clauses;  }

    const std::vector<Clause*>& clauses() const { return _clauses; }

    Watcher<int, int>::Iterator watch(Literal literal) const;

    const std::vector<int64>& occurences() const { return _occurences; }


    Watcher<int, int>::Iterator watch(Literal negated);
    const Clause* clause(unsigned int clause_index) const {
        return _clauses[clause_index];
    }

 private:
    int64 _num_variables;
    int64 _num_trivial_clauses;
    int64 _num_binary_clauses;

    std::vector<Clause*> _clauses;

    Watcher<int, int> _watchers;
    std::vector<int64> _occurences;

    DISALLOW_COPY_AND_ASSIGN(CNFModel);
};

}  // namespace sat

#endif  // SATTOOLS_CNFMODEL_H_
/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * End:
 */
