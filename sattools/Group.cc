// Copyright 2017 Hakan Metin - LIP6

#include "sattools/Group.h"

namespace sat {

Group::Group() {
}

Group::~Group() {
}

bool Group::addPermutation(std::unique_ptr<Permutation>&& permutation) {
    CHECK_NOTNULL(permutation.get());

    const unsigned int permutation_index = _permutations.size();
    const unsigned int num_cycles = permutation->numberOfCycles();

    if (num_cycles == 0)
        return false;

    if (permutation->isSpurious())
        return false;

    if (permutation->size() > _watchers.size())
        _watchers.resize(permutation->size());

    for (unsigned int c = 0; c < num_cycles; ++c) {
        Literal element = permutation->lastElementInCycle(c);

        for (const Literal& image : permutation->cycle(c)) {
            const int index = image.variable().value();
            _watchers[index].insert(permutation_index);

            const BooleanVariable variable = image.variable();
            _symmetric.insert(variable);

            if (element == image.negated())
                _inverting.insert(variable);

            element = image;
        }
    }

    _permutations.emplace_back(permutation.release());

    return true;
}

Group::Iterator Group::watch(BooleanVariable variable) const {
    const int index = variable.value();
    return Iterator(_watchers[index].begin(), _watchers[index].end());
}

std::string Group::debugString() const {
    std::string str;

    unsigned int i = 0;
    for (const std::unique_ptr<Permutation>& permutation : _permutations) {
        if (!str.empty())
            str += '\n';
        str += "[" + std::to_string(i++) + "]: " + permutation->debugString();
    }

    return str;
}

void Group::debugPrint() const {
    std::cout << debugString() << std::endl;
}

}  // namespace sat


/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * End:
 */
