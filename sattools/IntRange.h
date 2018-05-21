// Copyright 2017 Hakan Metin

#ifndef SATTOOLS_INTRANGE_H_
#define SATTOOLS_INTRANGE_H_

struct IntRange {
    IntRange(int begin, int end) : _begin(begin), _end(end) {}

    struct Iterator {
        explicit Iterator(int index) : _index(index) {}
        Iterator(const Iterator& other) : _index(other._index) {}

        int operator*() { return _index; }
        Iterator& operator++() { ++_index; return *this; }
        bool operator!=(const Iterator& other) { return _index < other._index; }

     private:
        int _index;
    };

    Iterator begin() { return Iterator(_begin); }
    Iterator end() { return Iterator(_end); }
 private:
    int _begin;
    int _end;
};

#endif  // SATTOOLS_INTRANGE_H_
