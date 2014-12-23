#ifndef __BINARY_HEAP__
#define __BINARY_HEAP__

#include <utility>
#include <algorithm>
#include <vector>

class BinaryHeap {
public:
    BinaryHeap(int sz): capacity(sz), size(0) {
        heap.resize(capacity);
        indexes.resize(capacity, -1);
    };
    
    virtual ~BinaryHeap() { }
    
    bool isEmpty() {
        return size == 0;
    }
    
    bool isFull() {
        return size == capacity;
    }
    
    void insert(std::pair<int, double> elem) {
        if (isFull())
            return;
        
        indexes[elem.first] = size;
        heap[size++] = elem;
        emersion(size-1);
    }
    
    std::pair<int, double> top() {
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

    void increaseKey(std::pair<int, double> newKey) {
        int index = indexes[newKey.first];
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
    inline int parent(int index) {
        return index >> 1;
    }
    
    inline int left(int index) {
        return index << 1;
    }
    
    inline int right(int index) {
        return (index << 1) + 1;
    }

    inline void swapElements(int i1, int i2) {
        std::swap(indexes[heap[i1].first], indexes[heap[i2].first]);
        std::swap(heap[i1], heap[i2]);
    }

    void emersion(int index) {
        if (index == 0)
            return;
        int p = parent(index);
        if (heap[p].second > heap[index].second) {
            swapElements(p, index);
            emersion(p);
        }
    }
    
    void dive(int index) {
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

    int capacity;
    int size;
    std::vector<std::pair<int, double>> heap;
    std::vector<int> indexes;
};

#endif /* __BINARY_HEAP__ */