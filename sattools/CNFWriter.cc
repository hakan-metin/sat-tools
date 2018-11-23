// Copyright 2017 Hakan Metin

#include "sattools/CNFWriter.h"

namespace sat {

bool CNFWriter::dump(const std::string &filename, const CNFModel& model) {
    StreamBuffer out(filename, kUncompressedWrite);

    out.write("p cnf ");
    out.writeInt(model.numberOfVariables());
    out.write(" ");
    out.writeInt(model.numberOfClauses());
    out.write("\n");


    for (Clause *clause : model.clauses()) {
        for (Literal l : *clause) {
            out.writeInt(l.signedValue());
            out.write(" ");
        }
        out.write("0 \n");
    }
    out.flush();

    return true;
}

}  // namespace sat
/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * End:
 */
