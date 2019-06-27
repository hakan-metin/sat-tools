// Copyright 2017 Hakan Metin

#include <iostream>
#include <string>
#include <memory>
#include <cmath>
#include <cstdlib>

#include "sattools/Assignment.h"
#include "sattools/Bitset.h"
#include "sattools/CNFModel.h"
#include "sattools/CNFStats.h"
#include "sattools/CNFReader.h"
#include "sattools/CNFWriter.h"
#include "sattools/Group.h"
#include "sattools/IntType.h"
#include "sattools/IntegralTypes.h"
#include "sattools/Logging.h"
#include "sattools/StreamBuffer.h"
#include "sattools/Trail.h"
#include "sattools/Saucy1Writer.h"
#include "sattools/SymmetryFinder.h"
#include "sattools/BlissAutomorphismFinder.h"
#include "sattools/LiteralGraphNodeAdaptor.h"
#include "sattools/BreakIDReader.h"
#include "sattools/Order.h"
#include "sattools/BreakerManager.h"

using sat::Assignment;
using sat::Clause;
using sat::BooleanVariable;
using sat::BreakIDReader;
using sat::BreakerManager;
using sat::CNFModel;
using sat::CNFReader;
using sat::CNFWriter;
using sat::Literal;
using sat::Group;
using sat::Order;


int main(int argc, char *argv[]) {
    CNFReader<CNFModel> reader;
    CNFModel model;
    Group group;
    Order order;
    BreakIDReader sym_reader;
    unsigned int num_vars;

    Assignment assignment;

    if (argc < 2)
        LOG(FATAL) << "Need CNF file";


    std::string cnf_filename(argv[1]);
    std::string sym_filename = cnf_filename + ".sym";

    // Load cnf file
    if (!reader.load(cnf_filename, &model))
        LOG(FATAL) << "Cannot load CNF file: " << cnf_filename;

    num_vars = model.numberOfVariables();
    assignment.resize(num_vars);

    // Load symmetry file
    if (!sym_reader.load(sym_filename, num_vars, &group))
        LOG(FATAL) << "Cannot load symmetry file: " << sym_filename;
    LOG(INFO) << "GENERATORS:" << std::endl << group.debugString();

    // Initialize order
    order.createIncreaseOrder(num_vars);
    LOG(INFO) << order.debugString();

    // Initialize BreakerManager
    BreakerManager breaker_manager(group, order, assignment);




    return 0;
}
