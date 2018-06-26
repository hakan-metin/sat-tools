// Copyright 2017 Hakan Metin

#include <iostream>
#include <string>

#include "sattools/Assignment.h"
#include "sattools/Bitset.h"
#include "sattools/CNFModel.h"
#include "sattools/CNFReader.h"
#include "sattools/Group.h"
#include "sattools/IntType.h"
#include "sattools/IntegralTypes.h"
#include "sattools/Logging.h"
#include "sattools/RangeIterator.h"
#include "sattools/StreamBuffer.h"
#include "sattools/SymmetryFinder.h"
#include "sattools/ColoredGraph.h"
#include "sattools/BlissSymmetryFinder.h"

using sat::Clause;
using sat::CNFModel;
using sat::CNFReader;
using sat::Group;
using sat::Literal;
using sat::SymmetryFinder;
using sat::LiteralGraphNodeAdaptor;
using sat::DoubleLiteralGraphNodeAdaptor;
using sat::ConsecutiveLiteralGraphNodeAdaptor;
using sat::BlissSymmetryFinder;

int main(int argc, char *argv[]) {
    CNFReader reader;
    CNFModel model;
    Group group;
    SymmetryFinder finder(model);

    if (argc < 2)
        LOG(FATAL) << "Need CNF file";

    std::string cnf_filename(argv[1]);
    std::string symmetry_filename = cnf_filename + ".txt";

    if (!reader.load(cnf_filename, &model))
        LOG(FATAL) << "Cannot load CNF file: " << cnf_filename;


    LOG(INFO) << model.numberOfVariables() << " " << model.numberOfClauses();
    for (const std::unique_ptr<Clause>& clause : model.clauses())
        std::cout << clause->debugString() << std::endl;

    finder.loadAutomorphism(sat::SymmetryFinder::FileEncoding::SAUCY1,
                            symmetry_filename, &group);

    LOG(INFO) << group.debugString();
    return 0;
}
