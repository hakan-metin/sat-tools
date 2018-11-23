// Copyright 2017 Hakan Metin - LIP6

#ifndef SATTOOLS_WATCHER_H_
#define SATTOOLS_WATCHER_H_

#include <unordered_map>
#include <vector>

namespace sat {

template<typename Key, typename Value>
class Watcher {
 public:
    Watcher() {}
    ~Watcher() {}

    void store(Key key, Value value) { _watchers[key].push_back(value);  }
    struct Iterator;
    Iterator watch(Key key) const {
        return Iterator(_watchers.at(key).begin(), _watchers.at(key).end());
    }

 private:
    std::unordered_map<Key, std::vector<Value>> _watchers;
};

template<typename Key, typename Value>
struct Watcher<Key, Value>::Iterator {
    Iterator() {}
    Iterator(const typename std::vector<Value>::const_iterator& b,
             const typename std::vector<Value>::const_iterator& e) :
        _begin(b), _end(e) {}

    const typename std::vector<Value>::const_iterator _begin;
    const typename std::vector<Value>::const_iterator _end;

    typename std::vector<Value>::const_iterator begin() const { return _begin; }
    typename std::vector<Value>::const_iterator end() const { return _end; }

    int size() const { return _end - _begin; }
};


}  // namespace sat


#endif  // SATTOOLS_WATCHER_H_
/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * End:
 */
