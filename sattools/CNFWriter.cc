// Copyright 2017 Hakan Metin

#include "sattools/CNFWriter.h"

namespace sat {

CNFWriter::CNFWriter() {
}

CNFWriter::~CNFWriter() {
}

bool CNFWriter::dump(const std::string &filename, const CNFModel& model) {
    StreamBuffer in(filename, kUncompressedWrite);

    in.write("p cnf ");
    in.writeInt(model.numberOfVariables()); in.write(" ");
    in.writeInt(model.numberOfClauses()); in.write("\n");

    in.flush();

    return true;
}


}  // namespace sat
/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * End:
 */
