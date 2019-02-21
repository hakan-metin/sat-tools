// Copyright 2017 Hakan Metin - LIP6

#ifndef SATTOOLS_DRATWRITER_H_
#define SATTOOLS_DRATWRITER_H_

#include <vector>
#include <memory>
#include <string>

#include "sattools/Literal.h"
#include "sattools/Clause.h"
#include "sattools/StreamBuffer.h"

namespace sat {

class DratWriter {
 public:
    DratWriter(bool in_compressd_format, const std::string& filename);
    DratWriter(bool in_compressd_format, const char* filename);

    ~DratWriter();

    void addClause(const std::vector<Literal>& clause);
    void deleteClause(const std::vector<Literal>& clause);

 private:
    std::unique_ptr<StreamBuffer> _stream;

    void writeClause(const std::vector<Literal>& clause);
};


}  // namespace sat


#endif  // SATTOOLS_DRATWRITER_H_
/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * End:
 */
