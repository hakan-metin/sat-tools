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


using sat::Assignment;
using sat::Clause;
using sat::BooleanVariable;
using sat::CNFModel;
using sat::CNFReader;
using sat::CNFWriter;
using sat::Literal;
using sat::Saucy1Writer;
using sat::Group;
using sat::LiteralGraphNodeAdaptor;
using sat::DoubleLiteralGraphNodeAdaptor;

int main(int argc, char *argv[]) {
    CNFReader<CNFModel> reader;
    CNFModel model;
    Group group;

    if (argc < 2)
        LOG(FATAL) << "Need CNF file";

#ifdef USE_GLOG
    google::InitGoogleLogging(argv[0]);
    FLAGS_logtostderr = 1;
#endif

    std::string cnf_filename(argv[1]);

    if (!reader.load(cnf_filename, &model))
        LOG(FATAL) << "Cannot load CNF file: " << cnf_filename;


    const bool verbose = false;
    const bool dump_no_opt = true;

    std::string opt_style = dump_no_opt ? "" : "style=invis";
    std::string no_opt_style = dump_no_opt ? "style=invis" : "";

    std::stringstream ss;

    const unsigned int num_vars = model.numberOfVariables();
    unsigned int num_nodes = num_vars * 2 + model.numberOfClauses() -
        model.numberOfBinaryClauses();
    unsigned int clause_node = num_vars * 2;
    unsigned int x, y;

    //if (dump_no_opt)
        num_nodes += model.numberOfBinaryClauses();

    unsigned int _num_vars = model.numberOfVariables();
    std::unique_ptr<LiteralGraphNodeAdaptor> _adaptor =
        std::make_unique<DoubleLiteralGraphNodeAdaptor>(_num_vars);

    std::vector<bool> seen(num_vars, false);

    std::set<int> forbid;

    std::string lbl;

    ss << "strict graph symmetry_graph {" << std::endl;
    ss << "overlap=false;" << std::endl;
    ss << "splines=true;" << std::endl;

    // Boolean consistency
    for (BooleanVariable var(0); var < num_vars; ++var) {
        Literal l = Literal(var, false);
        x = _adaptor->literalToNode(l) - 1;
        y = _adaptor->literalToNode(l.negated()) - 1;

        if (verbose)
            LOG(INFO) << x << " " << y;

        lbl = "<&#172;x<SUB>" + l.negated().debugString() + "</SUB>>";
        ss << l.debugString() << " [label=" << lbl << "];" << std::endl;
        lbl = "<x<SUB>" + l.negated().debugString() + "</SUB>>";
        ss << l.negated().debugString() << " [label=" << lbl << "];" <<
            std::endl;


        ss << l.debugString() << " -- "  << l.negated().debugString()
           << " [color=\"red\"]" << ";" << std::endl;

    }

    for (Clause *clause : model.clauses()) {
        if (clause->size() == 2) {
            const Literal first = clause->literals()[0];
            const Literal second = clause->literals()[1];

            x = _adaptor->literalToNode(first) - 1;
            y = _adaptor->literalToNode(second) - 1;
            if (verbose)
                LOG(INFO) << x << " " << y;

            ss << first.debugString() << " -- " << second.debugString()
               << "[" << no_opt_style << "];" << std::endl;

            x = first.variable().value();
            y = second.variable().value();

            seen[x] = true;
            seen[y] = true;
        }
            for (const Literal& literal : *clause) {
                x =  _adaptor->literalToNode(literal) - 1;

                if (verbose)
                    LOG(INFO) << x << " " << clause_node;

                if (!dump_no_opt && clause->size() == 2) {
                    forbid.insert(clause_node);
                    // ss << clause_node << "[style=invis];" << std::endl;
                    ss << literal.debugString() << " -- " << clause_node
                       << "[style=invis];" << std::endl;
                } else {
                    ss << literal.debugString() << " -- " << clause_node
                       << "[];" << std::endl;
                }

                x = literal.variable().value();
                seen[x] = true;
            }
            clause_node++;
        }


    // Change color of clauses
    for (clause_node = num_vars * 2; clause_node < num_nodes; clause_node++) {

        std::string style = forbid.find(clause_node) != forbid.end() ?
            "style=invis" : "style=filled";
        ss << clause_node << "[" << style << ", fillcolor=\"#00ff005f\","
           << "shape=square, label=<&#969;<SUB>"
           << (clause_node+1 - (num_vars * 2)) << "</SUB>>];" << std::endl;
    }


    ss << "}";

    std::cout << ss.str() << std::endl;

    return 0;
}
