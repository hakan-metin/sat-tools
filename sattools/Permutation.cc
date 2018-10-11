// Copyright 2017 Hakan Metin - LIP6

#include "sattools/Permutation.h"

namespace sat {

void Permutation::addToCurrentCycle(Literal x) {
    const int cs = _cycles.size();
    const int back = _cycles_lim.empty() ? 0 : _cycles_lim.back();
    _cycles.push_back(x);

    // Store image and inverse
    if (_cycles.size() > 0 && cs != back) {
        const Literal e = _cycles[cs - 1];
        const Literal i = _cycles[cs];
        _image[e] = i;
        _inverse[i] = e;
    }
}

void Permutation::closeCurrentCycle() {
    const int sz = _cycles.size();
    const int back = _cycles_lim.empty() ? 0 : _cycles_lim.back();

    if (back == sz)
        return;

    DCHECK_GE(sz - back, 2);
    _cycles_lim.push_back(sz);

    // Store image and inverse
    const int num_cycle = _cycles_lim.size() - 1;
    const Literal e = lastElementInCycle(num_cycle);
    const Literal i = *(cycle(num_cycle).begin());
    _image[e] = i;
    _inverse[i] = e;
}

Permutation::Iterator Permutation::cycle(unsigned int i) const {
    DCHECK_LT(i, numberOfCycles());

    return Iterator(_cycles.begin() + (i == 0 ? 0 : _cycles_lim[i - 1]),
        _cycles.begin() + _cycles_lim[i]);
}

Literal Permutation::lastElementInCycle(unsigned int i) const {
    DCHECK_LT(i, numberOfCycles());

    return _cycles[_cycles_lim[i] - 1];
}

const Literal Permutation::imageOf(const Literal& element) const {
    return _image.at(element);
}
const Literal Permutation::inverseOf(const Literal& element) const {
    return _inverse.at(element);
}

bool Permutation::isTrivialImage(const Literal& element) const {
    return _image.find(element) == _image.end();
}
bool Permutation::isTrivialInverse(const Literal& element) const {
    return _inverse.find(element) == _inverse.end();
}

bool Permutation::isSpurious() const {
    const unsigned int num_cycles = numberOfCycles();
    for (unsigned int c = 0; c < num_cycles; ++c) {
        for (const Literal& element : cycle(c)) {
            if (isTrivialImage(element))
                return true;
            if (isTrivialImage(element.negated()))
                return true;
            if (imageOf(element).negated() != imageOf(element.negated()))
                return true;
        }
    }

    return false;
}

unsigned int Permutation::order() const {
    unsigned int order = 1;
    for (unsigned int c = 0; c < numberOfCycles(); c++)
        order = std::lcm(order, cycle(c).size());
    return order;
}

std::string Permutation::debugString() const {
    std::string str, str_cycle;

    if (numberOfCycles() == 0)
        return std::string("()");

    for (unsigned int c = 0; c < numberOfCycles(); ++c) {
        str_cycle.clear();
        for (const Literal& element : cycle(c)) {
            if (!str_cycle.empty())
                str_cycle.append(" ");
            str_cycle.append(element.debugString());
        }
        str += "(" + str_cycle + ")";
    }

    return str;
}

void Permutation::debugPrint() const {
    std::cout << debugString() << std::endl;
}

}  // namespace sat

/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * End:
 */
