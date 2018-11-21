// Copyright 2017 Hakan Metin

#include "sattools/io/CNFWriter.h"

namespace sattools {

bool CNFWriter::dump(const std::string &filename, const CNFModel& model) {
    StreamBuffer out(filename, kUncompressedWrite);

    out.write("p cnf ");
    out.writeInt(model.numberOfVariables());
    out.write(" ");
    out.writeInt(model.numberOfClauses() + model.numberOfUnaryClauses());
    out.write("\n");


    for (const std::unique_ptr<Clause>& clause : model.unaryClauses()) {
        for (Literal l : *clause) {
            out.writeInt(l.signedValue());
            out.write(" ");
        }
        out.write("0 \n");
    }

    for (const std::unique_ptr<Clause>& clause : model.clauses()) {
        for (Literal l : *clause) {
            out.writeInt(l.signedValue());
            out.write(" ");
        }
        out.write("0 \n");
    }
    out.flush();

    return true;
}


}  // namespace sattools
/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * End:
 */
