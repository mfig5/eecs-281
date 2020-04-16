// Project identifier: 0E04A31E0D60C01986ACB20081C9D8722A1899B6

#ifndef BINARYPQ_H
#define BINARYPQ_H


#include <algorithm>
#include "Eecs281PQ.h"
#include <vector>
#include <utility>  

// A specialized version of the 'heap' ADT implemented as a binary heap.
template<typename TYPE, typename COMP_FUNCTOR = std::less<TYPE>>
class BinaryPQ : public Eecs281PQ<TYPE, COMP_FUNCTOR> {
    // This is a way to refer to the base class object.
    using BaseClass = Eecs281PQ<TYPE, COMP_FUNCTOR>;

public:
    // Description: Construct an empty heap with an optional comparison functor.
    // Runtime: O(1)
    explicit BinaryPQ(COMP_FUNCTOR comp = COMP_FUNCTOR()) :
        BaseClass{ comp } {
        // TODO: Implement this function.
            
    
    } // BinaryPQ


    // Description: Construct a heap out of an iterator range with an optional
    //              comparison functor.
    // Runtime: O(n) where n is number of elements in range.
    // TODO: when you implement this function, uncomment the parameter names.
    template<typename InputIterator>
    BinaryPQ(InputIterator start, InputIterator end, COMP_FUNCTOR comp = COMP_FUNCTOR()) :
        BaseClass{ comp }, data{start,end} {
            updatePriorities();
            
    } // BinaryPQ


    // Description: Destructor doesn't need any code, the data vector will
    //              be destroyed automaticslly.
    virtual ~BinaryPQ() {
    } // ~BinaryPQ()


    // Description: Assumes that all elements inside the heap are out of order and
    //              'rebuilds' the heap by fixing the heap invariant.
    // Runtime: O(n)
    virtual void updatePriorities() {
        for (int i = (int)(data.size() - 1); i >= 0; --i) {
            fixDown(i);
        }
    } // updatePriorities()


    // Description: Add a new element to the heap.
    // Runtime: O(log(n))
    // TODO: when you implement this function, uncomment the parameter names.
    virtual void push(const TYPE & val) {
        data.push_back(val);
        fixUp((int)(data.size()-1));
    } // push()


    // Description: Remove the most extreme (defined by 'compare') element from
    //              the heap.
    // Note: We will not run tests on your code that would require it to pop an
    // element when the heap is empty. Though you are welcome to if you are
    // familiar with them, you do not need to use exceptions in this project.
    // Runtime: O(log(n))
    virtual void pop() {
        // TODO: Implement this function
        std::swap(data.front(), data.back());
        data.pop_back();
        fixDown(0);
    } // pop()


    // Description: Return the most extreme (defined by 'compare') element of
    //              the heap.  This should be a reference for speed.  It MUST be
    //              const because we cannot allow it to be modified, as that
    //              might make it no longer be the most extreme element.
    // Runtime: O(1)
    virtual const TYPE & top() const {
        // TODO: Implement this function.
        return data.front();
        
    } // top()


    // Description: Get the number of elements in the heap.
    // Runtime: O(1)
    virtual std::size_t size() const {
        // TODO: Implement this function.  Might be very simple,
        // depending on your implementation.
        return data.size();
    } // size()


    // Description: Return true if the heap is empty.
    // Runtime: O(1)
    virtual bool empty() const {
        // TODO: Implement this function.  Might be very simple,
        // depending on your implementation.
        return data.empty();
    } // empty()


private:
    // Note: This vector *must* be used your heap implementation.
    std::vector<TYPE> data;
//    size_t = size;

    // TODO: Add any additional member functions or data you require here.
    // For instance, you might add fixUp() and fixDown().

    void fixUp(int k) {
        while (k > 0 && this->compare(data[(k-1)/2], data[k])) {
            std::swap(data[k], data[(k-1)/2]);
            k = (k-1)/2;
        }
    }
    void fixDown(int k) {
        while ((2 * k + 1) < (int)(data.size())) {
            int j = (2 * k) + 1;
            if ((j < (int)data.size()-1) && this->compare(data[j], data[j+1])) ++j;
            if (this->compare(data[j], data[k])) break;
            std::swap(data[k],data[j]);
            k = j;
        }
    }
    
}; // BinaryPQ


#endif // BINARYPQ_H
