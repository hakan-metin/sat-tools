// Copyright 2017 Hakan Metin - LIP6

#include "sattools/DratWriter.h"

namespace sat {

DratWriter::DratWriter(bool in_compressd_format, const std::string& filename) :
    DratWriter(in_compressd_format, filename.c_str()) {
}

DratWriter::DratWriter(bool in_compressd_format, const char* filename) {
    _stream = std::make_unique<StreamBuffer>(filename,
                 (in_compressd_format ? kCompressedWrite : kUncompressedWrite));
}

DratWriter::~DratWriter() {
    _stream->flush();
}

void DratWriter::addClause(const std::vector<Literal>& clause) {
    writeClause(clause);
}

void DratWriter::deleteClause(const std::vector<Literal>& clause) {
    _stream->write("d ");
    writeClause(clause);
}

void DratWriter::writeClause(const std::vector<Literal>& clause) {
    for (Literal l : clause) {
        _stream->writeInt(l.signedValue());
        _stream->write(" ");
    }
    _stream->write("0 \n");
}

}  // namespace sat
/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * End:
 */
