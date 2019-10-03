// Copyright 2017 Hakan Metin - LIP6

#ifndef SATTOOLS_INTEGERPRIORITYQUEUE_H_
#define SATTOOLS_INTEGERPRIORITYQUEUE_H_

#include <vector>
#include <string>
#include <sstream>
#include <functional>

#include "sattools/Macros.h"
#include "sattools/Logging.h"

namespace sat {

template <typename Element, class Compare = std::less<Element>>
class IntegerPriorityQueue {
 public:
    explicit IntegerPriorityQueue(int n = 0, Compare comp = Compare()) :
        _size(0), _less(comp) { reserve(n); }
    ~IntegerPriorityQueue() {}

    unsigned int size() const { return _size; }
    unsigned int capacity() const { return _position.size(); }

    bool empty() const { return _size == 0; }
    bool contains(int index) const { return _position[index] != 0; }

    void reserve(unsigned int n);
    void add(Element element);
    Element top() const { return _heap[1]; }
    void pop();
    void remove(int index);

    void increasePriority(Element element);
    void decreasePriority(Element element);
    void changePriority(Element element);

    void clear();

    std::string debugString() const;

 private:
    unsigned int _size;
    Compare _less;
    std::vector<Element> _heap;
    std::vector<unsigned int> _position;

    void set(unsigned int i, const Element element);
    void setAndIncreasePriority(unsigned int i, const Element element);
    void setAndDecreasePriority(unsigned int i, const Element element);
};


template <typename Element, class Compare>
void IntegerPriorityQueue<Element, Compare>::reserve(unsigned int n) {
    _heap.resize(n + 1);
    _position.resize(n, 0);
}

template <typename Element, class Compare>
void IntegerPriorityQueue<Element, Compare>::add(Element element) {
    DCHECK(!contains(element.index()));
    setAndIncreasePriority(++_size, element);
}

template <typename Element, class Compare>
void IntegerPriorityQueue<Element, Compare>::pop() {
    DCHECK(!empty());
    _position[top().index()] = 0;
    const unsigned int old_size = _size--;
    if (old_size > 1)
        setAndDecreasePriority(1, _heap[old_size]);
}

template <typename Element, class Compare>
void IntegerPriorityQueue<Element, Compare>::remove(int index) {
    DCHECK(contains(index));
    const unsigned int to_replace = _position[index];
    _position[index] = 0;
    const unsigned int old_size = _size--;
    if (to_replace == old_size)
        return;
    const Element element = _heap[old_size];
    if (_less(element, _heap[to_replace])) {
        setAndDecreasePriority(to_replace, element);
    } else {
        setAndIncreasePriority(to_replace, element);
    }
}

template <typename Element, class Compare>
void IntegerPriorityQueue<Element, Compare>::increasePriority(Element element) {
    setAndIncreasePriority(_position[element.index()], element);
}

template <typename Element, class Compare>
void IntegerPriorityQueue<Element, Compare>::decreasePriority(Element element) {
    setAndDecreasePriority(_position[element.index()], element);
}

template <typename Element, class Compare>
void IntegerPriorityQueue<Element, Compare>::changePriority(Element element) {
    DCHECK(contains(element.index()));
    const unsigned int i = _position[element.index()];
    if (i > 1 && _less(_heap[i >> 1], element)) {
        setAndIncreasePriority(i, element);
    } else {
        setAndDecreasePriority(i, element);
    }
}

template <typename Element, class Compare>
void IntegerPriorityQueue<Element, Compare>::
set(unsigned int i, const Element element) {
    _heap[i] = element;
    _position[element.index()] = i;
}

template <typename Element, class Compare>
void IntegerPriorityQueue<Element, Compare>::
setAndIncreasePriority(unsigned int i, const Element element) {
    while (i > 1) {
        const int parent = i >> 1;
        const Element parent_element = _heap[parent];
        if (!_less(parent_element, element))
            break;
        set(i, parent_element);
        i = parent;
    }
    set(i, element);
}

template <typename Element, class Compare>
void IntegerPriorityQueue<Element, Compare>::
setAndDecreasePriority(unsigned int i, const Element element) {
    const unsigned int size = _size;
    while (true) {
        const unsigned int left = i * 2;
        const unsigned int right = left + 1;
        if (right > size) {
            if (left > size)
                break;
            const Element left_element = _heap[left];
            if (!_less(element, left_element))
                break;
            set(i, left_element);
            i = left;
            break;
        }
        const Element left_element = _heap[left];
        const Element right_element = _heap[right];
        if (_less(left_element, right_element)) {
            if (!_less(element, right_element))
                break;
            set(i, right_element);
            i = right;
        } else {
            if (!_less(element, left_element))
                break;
            set(i, left_element);
            i = left;
        }
    }
    set(i, element);
}

template <typename Element, class Compare>
void IntegerPriorityQueue<Element, Compare>::clear() {
    _size = 0;
    _position.assign(_position.size(), 0);
}


template <typename Element, class Compare>
std::string IntegerPriorityQueue<Element, Compare>::debugString() const {
    std::stringstream ss;
    for (const Element& element : _heap) {
        ss << element << " ";
  }
  return ss.str();
}

}  // namespace sat


#endif  // SATTOOLS_INTEGERPRIORITYQUEUE_H_
/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * End:
 */
