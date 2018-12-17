// Copyright 2017 Hakan Metin

#ifndef SATTOOLS_CNFREADER_H_
#define SATTOOLS_CNFREADER_H_

#include <algorithm>
#include <vector>
#include <string>
#include <utility>

#include "sattools/CNFModel.h"
#include "sattools/Solver.h"
#include "sattools/Literal.h"
#include "sattools/Logging.h"
#include "sattools/StreamBuffer.h"

namespace sat {

template<typename Model>
class CNFReader {
 public:
    CNFReader() {}
    ~CNFReader() {}

    bool load(const std::string &filename, Model *model);

 private:
    DISALLOW_COPY_AND_ASSIGN(CNFReader);
};


template<typename Model>
inline bool CNFReader<Model>::load(const std::string &filename, Model *model) {
    StreamBuffer in(filename, kRead);
    unsigned int expected_num_vars = 0, expected_num_clauses = 0;
    unsigned int num_vars = 0, num_clauses = 0;

    int read_int;

    std::vector<Literal> literals;

    while (*in != '\0') {
        if (*in == 'c') {
            in.skipLine();
        } else if (*in == 'p') {
            ++in; CHECK_EQ(*in, ' ');
            ++in; CHECK_EQ(*in, 'c');
            ++in; CHECK_EQ(*in, 'n');
            ++in; CHECK_EQ(*in, 'f');
            ++in;

            expected_num_vars = in.readInt();
            expected_num_clauses = in.readInt();
            in.skipLine();
        } else {
            literals.clear();
            do {
                read_int = in.readInt();
                if (read_int != 0) {
                    Literal lit(read_int);
                    literals.push_back(lit);
                    num_vars = std::max<int>(num_vars, std::abs(read_int));
                }
            } while (read_int != 0);
            model->addClause(&literals);
            num_clauses++;
            in.skipLine();
        }
        in.skipWhiteSpaces();
    }

    if (expected_num_vars != num_vars) {
        LOG(ERROR) << "Expected " << expected_num_vars <<
            " variables: found " <<  num_vars;
        return false;
    }

    if (expected_num_clauses != num_clauses) {
        LOG(ERROR) << "Expected " << expected_num_clauses <<
            " clauses: found " <<  num_clauses;
        return false;
    }
    return true;
}


}  // namespace sat

#endif  // SATTOOLS_CNFREADER_H_

/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * End:
 */
