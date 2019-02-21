// Copyright 2017 Hakan Metin - LIP6

#include "sattools/DratProofHandler.h"

namespace sat {

DratProofHandler::DratProofHandler(bool in_compressd_format,
                                   const std::string& filename) :
    DratProofHandler(in_compressd_format, filename.c_str()) {
}

DratProofHandler::DratProofHandler(bool in_compressd_format,
                                   const char* filename) {
    _drat_writer = std::make_unique<DratWriter>(in_compressd_format,
                                                filename);
}

DratProofHandler::~DratProofHandler() {
}

void DratProofHandler::addClause(const std::vector<Literal>& clause) {
    _drat_writer->addClause(clause);
}

void DratProofHandler::deleteClause(const std::vector<Literal>& clause) {
    _drat_writer->deleteClause(clause);
}

}  // namespace sat
/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * End:
 */
