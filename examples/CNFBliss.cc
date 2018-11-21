#include <iostream>

#include "sattools/io/CNFReader.h"
#include "sattools/io/Saucy1Writer.h"
#include "sattools/sym/BlissAutomorphismFinder.h"
#include "sattools/sym/SymmetryFinder.h"

int main(int argc, char *argv[]) {
    sattools::CNFReader reader;
    sattools::CNFModel model;
    sattools::Group group;

    if (argc < 2)
        LOG(FATAL) << "Need CNF file";

    std::string cnf_filename(argv[1]);

    if (!reader.load(cnf_filename, &model))
        LOG(FATAL) << "Cannot load CNF file: " << cnf_filename;

    sattools::SymmetryFinder<sattools::BlissAutomorphismFinder,
                             sattools::DoubleLiteralGraphNodeAdaptor> finder;

    finder.findAutomorphisms(model, &group);

    sattools::Saucy1Writer::dump(std::cout, group);

    return 0;
}
