// Copyright 2017 Hakan Metin

#include <iostream>
#include <string>

#include "sattools/Assignment.h"
#include "sattools/Bitset.h"
#include "sattools/Breaker.h"
#include "sattools/CNFModel.h"
#include "sattools/CNFReader.h"
#include "sattools/CNFWriter.h"
#include "sattools/Group.h"
#include "sattools/IntType.h"
#include "sattools/IntegralTypes.h"
#include "sattools/Logging.h"
#include "sattools/Orbits.h"
#include "sattools/OrderScoring.h"
#include "sattools/RangeIterator.h"
#include "sattools/StreamBuffer.h"
#include "sattools/SymmetryFinder.h"
#include "sattools/Trail.h"
#include "sattools/ColoredGraph.h"
#include "sattools/BlissAutomorphismFinder.h"
#include "sattools/SaucyAutomorphismFinder.h"
#include "sattools/LiteralGraphNodeAdaptor.h"
#include "sattools/Simplifier.h"

using sat::Assignment;
using sat::Breaker;
using sat::Clause;
using sat::CNFModel;
using sat::CNFReader;
using sat::CNFWriter;
using sat::ColoredGraph;
using sat::AdjacencyColoredGraph;
using sat::BlissAutomorphismFinder;
using sat::SaucyAutomorphismFinder;
using sat::Group;
using sat::Literal;
using sat::Orbits;
using sat::OrderScoring;
using sat::SymmetryFinder;
using sat::Simplifier;
using sat::LiteralGraphNodeAdaptor;
using sat::DoubleLiteralGraphNodeAdaptor;
using sat::ConsecutiveLiteralGraphNodeAdaptor;

int main(int argc, char *argv[]) {
    CNFReader reader;
    CNFModel model;
    Group bliss_group, saucy_group;

    if (argc < 2)
        LOG(FATAL) << "Need CNF file";

#ifdef USE_GLOG
    google::InitGoogleLogging(argv[0]);
    FLAGS_logtostderr = 1;
#endif

    std::string cnf_filename(argv[1]);
    std::string symmetry_filename = cnf_filename + ".txt";

    if (!reader.load(cnf_filename, &model))
        LOG(FATAL) << "Cannot load CNF file: " << cnf_filename;

    // LOG(INFO) << "Vars: " << model.numberOfVariables() <<
    //     " Clauses: " << model.numberOfClauses();

    // for (const std::unique_ptr<Clause>& clause : model.clauses())
    //     std::cout << clause->debugString() << std::endl;


    SymmetryFinder<BlissAutomorphismFinder,
                   DoubleLiteralGraphNodeAdaptor> bliss_finder;
    bliss_finder.findAutomorphisms(model, &bliss_group);


    Simplifier simplifier(bliss_group, &model);
    simplifier.simplify();


    std::string output = "/tmp/reduce-" +
        std::string(basename(cnf_filename.c_str()));
    CNFWriter::dump(output, model);

    LOG(INFO) << "Simplified CNF in written in " + output;


    // LOG(INFO) << bliss_group.debugString();

    // Assignment assignment(model.numberOfVariables());
    // Breaker breaker(bliss_group, &model, &assignment);

    // breaker.symsimp();



    // SymmetryFinder<SaucyAutomorphismFinder,
    //                DoubleLiteralGraphNodeAdaptor> saucy_finder;
    // saucy_finder.findAutomorphisms(model, &saucy_group);
    // LOG(INFO) << std::endl << saucy_group.debugString() << std::endl;
    // breaker.addUnits(model, saucy_group);

    // Orbits orbits;
    // orbits.assign(group);
    // LOG(INFO) << "number of orbits " << orbits.numberOfOrbits();

    return 0;
}
