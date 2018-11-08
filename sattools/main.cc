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
#include "sattools/Order.h"
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
#include "sattools/OrderWriter.h"
#include "sattools/Saucy1Writer.h"

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
using sat::Order;
using sat::OrderScoring;
using sat::SymmetryFinder;
using sat::Simplifier;
using sat::LiteralGraphNodeAdaptor;
using sat::DoubleLiteralGraphNodeAdaptor;
using sat::ConsecutiveLiteralGraphNodeAdaptor;
using sat::OrderWriter;
using sat::Saucy1Writer;

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

    LOG(INFO) << bliss_group.debugString();


    Order order;

    Simplifier simplifier(bliss_group, &model, &order);
    simplifier.simplify();

    std::string base_name(basename(cnf_filename.c_str()));
    std::string output_cnf = "/tmp/reduce-" + base_name;
    std::string output_order = "/tmp/order-" + base_name;
    std::string output_group = "/tmp/group-" + base_name;

    CNFWriter::dump(output_cnf, model);
    OrderWriter::dump(output_order, order);
    Saucy1Writer::dump(output_group, bliss_group);

    LOG(INFO) << "CNF is written in " + output_cnf;
    LOG(INFO) << "Order is written in " + output_order;
    LOG(INFO) << "Group is written in " + output_group;

    return 0;
}
