// Copyright 2017 Hakan Metin

#ifndef SATTOOLS_CNFMODEL_H_
#define SATTOOLS_CNFMODEL_H_

#include <algorithm>
#include <memory>
#include <vector>

#include "sattools/Clause.h"
#include "sattools/Literal.h"
#include "sattools/Logging.h"

namespace sat {

class CNFModel {
 public:
    CNFModel();
    ~CNFModel();

    void addClause(std::vector<Literal>* literals);


    int64 numberOfVariables() const { return _num_variables + 1; }
    int64 numberOfClauses() const { return _clauses.size(); }

    int64 numberOfBinaryClauses() const { return _num_binary_clauses;  }
    int64 numberOfTernaryClauses() const { return _num_ternary_clauses;  }

    const std::vector<std::vector<Literal>>& clauses() const {
        return _clauses;
    }

 private:
    int64 _num_variables;
    int64 _num_trivial_clauses;
    int64 _num_binary_clauses;
    int64 _num_ternary_clauses;

    // The set of all clauses.
    std::vector<std::vector<Literal>> _clauses;

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
