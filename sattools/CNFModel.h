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

namespace sat {

class CNFModel {
 public:
    CNFModel();
    ~CNFModel();

    void addClause(std::vector<Literal>* literals);

    int64 numberOfVariables() const;
    int64 numberOfClauses() const;

    int64 numberOfUnaryClauses()   const { return _unary_clauses.size();   }
    int64 numberOfBinaryClauses()  const { return _binary_clauses.size();  }
    int64 numberOfTernaryClauses() const { return _ternary_clauses.size(); }
    int64 numberOfLargeClauses()   const { return _large_clauses.size();   }

    RangeIterator<std::unique_ptr<Clause>> unaryClauses();
    RangeIterator<std::unique_ptr<Clause>> binaryClauses();
    RangeIterator<std::unique_ptr<Clause>> ternaryClauses();
    RangeIterator<std::unique_ptr<Clause>> largeClauses();
    RangeIterator<std::unique_ptr<Clause>> clauses();

    RangeIterator<std::unique_ptr<Clause>> unaryClauses() const;
    RangeIterator<std::unique_ptr<Clause>> binaryClauses() const;
    RangeIterator<std::unique_ptr<Clause>> ternaryClauses() const;
    RangeIterator<std::unique_ptr<Clause>> largeClauses() const;
    RangeIterator<std::unique_ptr<Clause>> clauses() const;


 private:
    int64 _num_variables;
    int64 _num_trivial_clauses;
    std::vector<std::unique_ptr<Clause>> _unary_clauses;
    std::vector<std::unique_ptr<Clause>> _binary_clauses;
    std::vector<std::unique_ptr<Clause>> _ternary_clauses;
    std::vector<std::unique_ptr<Clause>> _large_clauses;

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
