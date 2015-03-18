#ifndef __BINARY_HEAP__
#define __BINARY_HEAP__

#include <utility>
#include <algorithm>
#include <vector>

class BinaryHeap {
public:
    BinaryHeap(size_t sz): capacity(sz), size(0) {
        heap.resize(capacity);
        indexes.resize(capacity, -1);
    };
    
    virtual ~BinaryHeap() { }
    
    bool isEmpty() const {
        return size == 0;
    }
    
    bool isFull() const {
        return size == capacity;
    }
    
    void insert(std::pair<size_t, double> elem) {
        if (isFull())
            return;
        
        indexes[elem.first] = size;
        heap[size++] = elem;
        emersion(size-1);
    }
    
    std::pair<size_t, double> top() const {
        if (isEmpty())
            return std::make_pair(-1, -1);
        else
            return heap[0];
    }
    
    void pop() {
        if (isEmpty())
            return;
        size--;

        indexes[heap[0].first] = -1;
            
        if (size > 0) {
            heap[0] = heap[size];
            indexes[heap[0].first] = 0;
            dive(0);
        }
    }

    void increaseKey(std::pair<size_t, double> newKey) {
        size_t index = indexes[newKey.first];
        if (index >= 0) {
            if (newKey.second < heap[index].second) {
                heap[index].second = newKey.second;
                emersion(index);
            }
        } else {
            insert(newKey);
        }
    }

private:
    inline static size_t parent(size_t index) {
        return index >> 1;
    }
    
    inline static size_t left(size_t index) {
        return index << 1;
    }
    
    inline static size_t right(size_t index) {
        return (index << 1) + 1;
    }

    inline void swapElements(size_t i1, size_t i2) {
        std::swap(indexes[heap[i1].first], indexes[heap[i2].first]);
        std::swap(heap[i1], heap[i2]);
    }

    void emersion(size_t index) {
        if (index == 0)
            return;
        size_t p = parent(index);
        if (heap[p].second > heap[index].second) {
            swapElements(p, index);
            emersion(p);
        }
    }
    
    void dive(size_t index) {
        double left_elem = left(index) < size ? heap[left(index)].second : DBL_MAX;
        double right_elem = right(index) < size ? heap[right(index)].second : DBL_MAX;
        
        double min_elem = std::min(left_elem, right_elem);
        
        if (heap[index].second > min_elem) {
            if (left_elem == min_elem) {
                swapElements(index, left(index));
                dive(left(index));
            } else {
                swapElements(index, right(index));
                dive(right(index));
            }
        }
    }

    const size_t capacity;
    size_t size;
    std::vector<std::pair<size_t, double>> heap;
    std::vector<size_t> indexes;
};

#endif /* __BINARY_HEAP__ */