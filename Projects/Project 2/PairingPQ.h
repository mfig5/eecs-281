// Project identifier: 0E04A31E0D60C01986ACB20081C9D8722A1899B6

#ifndef PAIRINGPQ_H
#define PAIRINGPQ_H

#include "Eecs281PQ.h"
#include <deque>
#include <utility>

// A specialized version of the 'priority_queue' ADT implemented as a pairing heap.
template<typename TYPE, typename COMP_FUNCTOR = std::less<TYPE>>
class PairingPQ : public Eecs281PQ<TYPE, COMP_FUNCTOR> {
    // This is a way to refer to the base class object.
    using BaseClass = Eecs281PQ<TYPE, COMP_FUNCTOR>;

public:
    // Each node within the pairing heap
    class Node {
        public:
            // TODO: After you add add one extra pointer (see below), be sure to
            // initialize it here.
            explicit Node(const TYPE &val)
        : elt{ val }, child{ nullptr }, sibling{ nullptr } , prev{nullptr}
            {}

            // Description: Allows access to the element at that Node's position.
			// There are two versions, getElt() and a dereference operator, use
			// whichever one seems more natural to you.
            // Runtime: O(1) - this has been provided for you.
            const TYPE &getElt() const { return elt; }
            const TYPE &operator*() const { return elt; }

            // The following line allows you to access any private data members of this
            // Node class from within the PairingPQ class. (ie: myNode.elt is a legal
            // statement in PairingPQ's add_node() function).
            friend PairingPQ;

        private:
            TYPE elt;
            Node *child;
            Node *sibling;
            Node *prev;
            // TODO: Add one extra pointer (parent or previous) as desired.
    }; // Node


    // Description: Construct an empty priority_queue with an optional comparison functor.
    // Runtime: O(1)
    explicit PairingPQ(COMP_FUNCTOR comp = COMP_FUNCTOR()) :
    BaseClass{ comp }, root{ nullptr }, tree_size{ 0 }{
        // TODO: Implement this function.
    } // PairingPQ()


    // Description: Construct a priority_queue out of an iterator range with an optional
    //              comparison functor.
    // Runtime: O(n) where n is number of elements in range.
    // TODO: when you implement this function, uncomment the parameter names.
    template<typename InputIterator>
    PairingPQ(InputIterator start, InputIterator end, COMP_FUNCTOR comp = COMP_FUNCTOR()) :
    BaseClass{ comp }, root{ nullptr }, tree_size{ 0 }{
            while (start != end) {
                this->push(*start);
                ++start;
            }
        // TODO: Implement this function.
    } // PairingPQ()


    // Description: Copy constructor.
    // Runtime: O(n)
    PairingPQ(const PairingPQ& other) :
        BaseClass{ other.compare }, root{ nullptr }, tree_size{ 0 } {
        // TODO: Implement this function.
            collection.push_back(other.root);
            while (!collection.empty()) {
                if (collection[0]->sibling) {
                    collection.push_back(collection[0]->sibling);
                }
                if (collection[0]->child) {
                    collection.push_back(collection[0]->child);
                }
                this->addNode(collection[0]->elt);
                collection.pop_front();
           //     addNode(collection.front()->elt);
            }
    } // PairingPQ()


    // Description: Copy assignment operator.
    // Runtime: O(n)
    // TODO: when you implement this function, uncomment the parameter names.
    PairingPQ& operator=(const PairingPQ& rhs) {
        // TODO: Implement this function.
        PairingPQ temp(rhs);
        
        std::swap(root, temp.root);
        std::swap(tree_size, temp.tree_size);
        std::swap(this->compare, temp.compare);
        
        return *this;
    } // operator=()


    // Description: Destructor
    // Runtime: O(n)
    ~PairingPQ() {
        // TODO: Implement this function.
        
        if (!this->root) {
            return;
        }
        collection.push_back(this->root);
        this->root = nullptr;
        Node *temp;
        
        while (!collection.empty()) {
            if (collection[0]->sibling) {
                collection.push_back( collection[0]->sibling);
            }
            if (collection[0]->child) {
                collection.push_back( collection[0]->child);
            }
            temp = collection.front();
            temp->child = nullptr;
            temp->prev = nullptr;
            temp->sibling = nullptr;
            collection.pop_front();
            delete temp;
        }
    } // ~PairingPQ()


    // Description: Assumes that all elements inside the priority_queue are out of order and
    //              'rebuilds' the priority_queue by fixing the priority_queue invariant.
    // Runtime: O(n)
    virtual void updatePriorities() {
        // TODO: Implement this function.
        Node *temp;
        collection.push_back(this->root);
        this->root = nullptr;
        while (!collection.empty()) {
            if (collection[0]->sibling) {
                collection.push_back(collection[0]->sibling);
            //    temp->sibling = nullptr;
            }
            if (collection[0]->child) {
                collection.push_back(collection[0]->child);
               // temp->child = nullptr;
            }
            temp = collection.front();
            temp->child = nullptr;
            temp->prev = nullptr;
            temp->sibling = nullptr;
            collection.pop_front();
            this->root = meld(this->root, temp);
        }
    } // updatePriorities()


    // Description: Add a new element to the priority_queue. This is almost done,
    //              in that you should implement push functionality in the addNode()
    //              function, and this function should call addNode().
    // Runtime: O(1)
    // TODO: when you implement this function, uncomment the parameter names.
    virtual void push(const TYPE & val) {
        // TODO: Implement this function.
        addNode(val);
    } // push()


    // Description: Remove the most extreme (defined by 'compare') element from
    //              the priority_queue.
    // Note: We will not run tests on your code that would require it to pop an
    // element when the priority_queue is empty. Though you are welcome to if you are
    // familiar with them, you do not need to use exceptions in this project.
    // Runtime: Amortized O(log(n))
    virtual void pop() {
        // TODO: Implement this function.
        if (!this->root->child) {
            this->root = nullptr;
            --tree_size;
            delete root;
            return;
        }
        Node* temp;
        temp = this->root->child;
        delete root;
      //  collection.push_back(temp);
        while (temp->sibling) {
            collection.push_back(temp);
            temp = temp->sibling;
        }
        collection.push_back(temp);
    //    Node* new_root = collection.front();
        if (collection.size() <= 1) {
             this->root = collection.front();
            collection.pop_front();
            --tree_size;
            return;
        }
        else {
            while(collection.size() > 1) {
                collection[0]->prev = nullptr;
                collection[0]->sibling = nullptr;
                collection[1]->prev = nullptr;
                collection[1]->sibling = nullptr;
                collection.push_back(meld(collection[0], collection[1]));
                collection.pop_front();
                collection.pop_front();
                //    new_root = meld(temp1, temp2);
            }
        }
        collection.pop_front();
        --tree_size;

    }// pop()


    // Description: Return the most extreme (defined by 'compare') element of
    //              the heap.  This should be a reference for speed.  It MUST be
    //              const because we cannot allow it to be modified, as that
    //              might make it no longer be the most extreme element.
    // Runtime: O(1)
    virtual const TYPE & top() const {
        return this->root->elt;
    } // top()


    // Description: Get the number of elements in the priority_queue.
    // Runtime: O(1)
    virtual std::size_t size() const {
        return tree_size;
    } // size()

    // Description: Return true if the priority_queue is empty.
    // Runtime: O(1)
    virtual bool empty() const {
        if (tree_size == 0) {
            return true;
        }
        return false;
    } // empty()


    // Description: Updates the priority of an element already in the priority_queue by
    //              replacing the element refered to by the Node with new_value.
    //              Must maintain priority_queue invariants.
    //
    // PRECONDITION: The new priority, given by 'new_value' must be more extreme
    //               (as defined by comp) than the old priority.
    //
    // Runtime: As discussed in reading material.
    // TODO: when you implement this function, uncomment the parameter names.
    void updateElt(Node* node, const TYPE & new_value) {
        // TODO: Implement this function
        if (node->prev->child == node) {
            if (node->sibling) {
                node->prev->child = node->sibling;
                node->sibling->prev = node->prev;
            }
            node->sibling = nullptr;
            node->prev = nullptr;
            node->elt = new_value;
            
            meld(node, this->root);
        }
        else if (node->prev && node->sibling) {
            node->prev->sibling = node->sibling;
            node->sibling->prev = node->prev;
            node->prev = nullptr;
            node->sibling = nullptr;
            node->elt = new_value;
            
            meld(node, this->root);
        }
        else if (!node->sibling) {
            node->prev->sibling = node->sibling;
            node->elt = new_value;
            node->prev = nullptr;
            node->sibling = nullptr;
            node->elt = new_value;
            
            meld(node, this->root);
        }
    } // updateElt()


    // Description: Add a new element to the priority_queue. Returns a Node* corresponding
    //              to the newly added element.
    // Runtime: O(1)
    // TODO: when you implement this function, uncomment the parameter names.
    // NOTE: Whenever you create a node, and thus return a Node *, you must be sure to
    //       never move or copy/delete that node in the future, until it is eliminated
    //       by the user calling pop().  Remember this when you implement updateElt() and
    //       updatePriorities().
    Node* addNode(const TYPE & val) {
        // TODO: Implement this function
        Node* temp = new Node(val);
      //  temp->elt = val;
        if (!this->empty()) {
            ++tree_size;
            this->root = meld(temp, this->root);
        }
        else {
            ++tree_size;
            this->root = temp;
            return this->root;
        }
        return temp; // TODO: Delete or change this line
    } // addNode()


private:
    Node* root;
    int tree_size;
    std::deque<Node *>collection;
    Node* meld(Node* l, Node* r) {
        if (!l && r) {
            this->root = r;
            return this->root;
        }
        if (!r && l) {
            this->root = l;
            return this->root;
        }
        else {
            if (this->compare(l->elt, r->elt)) {
                l->sibling = r->child;
                r->child = l;
                if (l->sibling) {
                    l->sibling->prev = l;
                }
                l->prev = r;
                this->root = r;
                return r;
            }
            else {
                r->sibling = l->child;
                l->child = r;
                if (r->sibling) {
                    r->sibling->prev = r;
                }
                r->prev = l;
                this->root = l;
                return l;
            }
        }
    }
    // TODO: Add any additional member functions or data you require here.
    // TODO: We recommend creating a 'meld' function (see the Pairing Heap papers).

};


#endif // PAIRINGPQ_H
