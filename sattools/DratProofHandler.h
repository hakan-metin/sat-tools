// Copyright 2017 Hakan Metin - LIP6

#ifndef SATTOOLS_DRATPROOFHANDLER_H_
#define SATTOOLS_DRATPROOFHANDLER_H_

#include <memory>
#include <vector>
#include <string>

#include "sattools/DratWriter.h"
#include "sattools/Literal.h"
#include "sattools/Clause.h"


namespace sat {

class DratProofHandler {
 public:
    DratProofHandler(bool in_compressd_format, const std::string& filename);
    DratProofHandler(bool in_compressd_format, const char* filename);

    ~DratProofHandler();

    void addClause(const std::vector<Literal>& clause);
    void deleteClause(const std::vector<Literal>& clause);

 private:
    std::unique_ptr<DratWriter> _drat_writer;
};

}  // namespace sat


#endif  // SATTOOLS_DRATPROOFHANDLER_H_
/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * End:
 */
