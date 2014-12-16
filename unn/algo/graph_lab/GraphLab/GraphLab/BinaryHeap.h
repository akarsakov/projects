#ifndef __BINARY_HEAP__
#define __BINARY_HEAP__

#include <utility>
#include <algorithm>

class BinaryHeap {
public:
    BinaryHeap(int sz): capacity(sz), size(0) {
        heap = new std::pair<int, double>[capacity];
    };
    
    virtual ~BinaryHeap() {
        delete[] heap;
    }
    
    bool isEmpty() {
        return size == 0;
    }
    
    bool isFull() {
        return size == capacity;
    }
    
    void insert(std::pair<int, double> elem) {
        if (isFull())
            return;
            
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
        if (size > 0) {
            std::swap(heap[0], heap[size]);
            dive(0);
        }
    }

    void increaseKey(int val, double newKey) {
        int i=0;
        for (; i<size; i++) {
            if (heap[i].first == val)
                break;
        }
        if (i<size) {
            heap[i].second = newKey;
            emersion(i);
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

    void emersion(int index) {
        if (index == 0)
            return;
        int p = parent(index);
        if (heap[p].second > heap[index].second) {
            std::swap(heap[p], heap[index]);
            emersion(p);
        }
    }
    
    void dive(int index) {
        double left_elem = left(index) < size ? heap[left(index)].second : DBL_MAX;
        double right_elem = right(index) < size ? heap[right(index)].second : DBL_MAX;
        
        double min_elem = std::min(left_elem, right_elem);
        
        if (heap[index].second > min_elem) {
            if (left_elem == min_elem) {
                swap(heap[index], heap[left(index)]);
                dive(left(index));
            } else {
                swap(heap[index], heap[right(index)]);
                dive(right(index));
            }
        }
    }

    int capacity;
    int size;
    std::pair<int, double>* heap;
};

#endif /* __BINARY_HEAP__ */