// Copyright 2017 Hakan Metin

#ifndef SATTOOLS_BASE_RANGEITERATOR_H_
#define SATTOOLS_BASE_RANGEITERATOR_H_

#include <initializer_list>
#include <vector>

namespace sattools {

template <class T>
struct RangeIterator {
    RangeIterator(std::initializer_list<const std::vector<T>*> list) :
        _range(list) {}

    struct Iterator {
        explicit
        Iterator(const std::vector<const std::vector<T>*>& c, unsigned int i)
            : _c(c), _i(i), _j(0) {
            if (_i < _c.size() && _j >= _c[_i]->size())
                ++*this;
        }

        const T& operator*() const { return _c[_i]->at(_j); }
        Iterator& operator++() {
            _j++;
            while (_i < _c.size() && _j >= _c[_i]->size()) {
                _j = 0;
                _i++;
            }
            return *this;
        }

        bool operator!=(const Iterator& other) const {
            return other._i != _i;
        }

     private:
        const typename std::vector<const std::vector<T>*>& _c;
        unsigned int _i, _j;
    };

    Iterator begin() { return Iterator(_range, 0); }
    Iterator end()   { return Iterator(_range, _range.size()); }

 private:
    std::vector<const std::vector<T>*> _range;
};

}  // namespace sattools

#endif  // SATTOOLS_BASE_RANGEITERATOR_H_
/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * End:
 */
