// Copyright 2017 Hakan Metin - LIP6

#include "sattools/Orbits.h"


namespace sat {

void Orbits::assign(const std::vector<Permutation*>& permutations) {
    std::unordered_set<Literal> used;

    _disjointSets.Clear();
    _symmetrics.clear();
    _orbits.clear();

    for (const Permutation* permutation : permutations) {
        for (unsigned int c = 0; c < permutation->numberOfCycles(); ++c) {
            Literal element = permutation->lastElementInCycle(c);
            _disjointSets.Add(element.index().value());

            for (const Literal& image : permutation->cycle(c)) {
                if (used.insert(element).second)
                    _symmetrics.push_back(element);

                _disjointSets.Add(image.index().value());
                _disjointSets.Union(element.index().value(),
                                   image.index().value());

                element = image;
            }
        }
    }

    std::unordered_map<int, std::vector<Literal>> m_orbits;
    for (const Literal& element : _symmetrics) {
        int representant = _disjointSets.Find(element.index().value());
        m_orbits[representant].push_back(element);
    }

    for (const auto& orbit : m_orbits)
        _orbits.emplace_back(std::move(orbit.second));
}

void Orbits::assign(const Group& group) {
    std::unordered_set<Literal> used;

    _disjointSets.Clear();
    _symmetrics.clear();
    _orbits.clear();

    for (const std::unique_ptr<Permutation>& perm : group.permutations()) {
        for (unsigned int c = 0; c < perm->numberOfCycles(); ++c) {
            Literal element = perm->lastElementInCycle(c);
            _disjointSets.Add(element.index().value());

            for (const Literal& image : perm->cycle(c)) {
                if (used.insert(element).second)
                    _symmetrics.push_back(element);

                _disjointSets.Add(image.index().value());
                _disjointSets.Union(element.index().value(),
                                   image.index().value());

                element = image;
            }
        }
    }

    std::unordered_map<int, std::vector<Literal>> m_orbits;
    for (const Literal& element : _symmetrics) {
        int representant = _disjointSets.Find(element.index().value());
        m_orbits[representant].push_back(element);
    }

    for (const auto& orbit : m_orbits)
        _orbits.emplace_back(std::move(orbit.second));
}


bool Orbits::isInSameOrbit(Literal a, Literal b) {
    int ra = _disjointSets.Find(a.index().value());
    int rb = _disjointSets.Find(b.index().value());
    return ra == rb;
}


std::string Orbits::debugString() const {
    std::stringstream ss;
    unsigned int i = 0;
    ss << "Orbits:" << std::endl;
    for (std::vector<Literal> orbit : _orbits) {  // make copy
        std::sort(orbit.begin(), orbit.end());
        std::string orbit_str;
        for (const Literal& literal : orbit) {
            if (!orbit_str.empty())
                orbit_str.append(" ");
            orbit_str.append(literal.debugString());
        }
        ss << "[" << i++ << "]: " << orbit_str << std::endl;
    }

    return ss.str();
}


}  // namespace sat
