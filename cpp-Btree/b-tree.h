#pragma once

#include <iostream>
#include <algorithm>

// Forward declaration to allow friend declaration inside BTreeNode
template <typename T>
class BTree;

// B-Tree node
template <typename T>
class BTreeNode {
public:
    T* keys;                  // array of keys
    int t;                    // minimum degree (defines range for number of keys)
    BTreeNode<T>** C;         // array of child pointers
    int n;                    // current number of keys
    bool leaf;                // is true when node is leaf

    BTreeNode(int _t, bool _leaf);

    // Traversal and search
    void traverse(std::ostream& os = std::cout) const; // inorder traversal
    BTreeNode<T>* search(const T& k);                   // search key in subtree

    // Insertion helpers
    void insertNonFull(const T& k);
    void splitChild(int i, BTreeNode<T>* y);

    // Deletion helpers (CLRS-style)
    int  findKey(const T& k) const; // index of first key >= k
    void remove(const T& k);
    void removeFromLeaf(int idx);
    void removeFromNonLeaf(int idx);
    T    getPred(int idx); // predecessor of keys[idx]
    T    getSucc(int idx); // successor of keys[idx]
    void fill(int idx);    // ensure child C[idx] has at least t-1 keys
    void borrowFromPrev(int idx);
    void borrowFromNext(int idx);
    void merge(int idx);   // merge C[idx] and C[idx+1] with keys[idx] in between

    friend class BTree<T>;
};

// B-Tree
template <typename T>
class BTree {
    BTreeNode<T>* root; // pointer to root node
    int t;              // minimum degree
public:
    explicit BTree(int _t) : root(nullptr), t(_t) {
        if (t < 2) throw std::invalid_argument("B-Tree minimum degree t must be >= 2");
    }

    // disable copy to keep ownership simple (can be added if needed)
    BTree(const BTree&) = delete;
    BTree& operator=(const BTree&) = delete;

    ~BTree() { clear(root); }

    void traverse(std::ostream& os = std::cout) const {
        if (root != nullptr) root->traverse(os);
    }

    BTreeNode<T>* search(const T& k) {
        return (root == nullptr) ? nullptr : root->search(k);
    }

    void insert(const T& k);

    void remove(const T& k) {
        if (!root) return;

        root->remove(k);

        // If the root has 0 keys, make its first child the new root (if it has a child),
        // otherwise set root to nullptr if it was a leaf.
        if (root->n == 0) {
            BTreeNode<T>* tmp = root;
            if (root->leaf) {
                // Tree becomes empty
                delete[] root->keys;
                delete[] root->C;
                delete root;
                root = nullptr;
            }
            else {
                root = root->C[0];
                // free old empty node 'tmp'
                delete[] tmp->keys;
                delete[] tmp->C;
                delete tmp;
            }
        }
    }

private:
    void clear(BTreeNode<T>* node) {
        if (!node) return;
        if (!node->leaf) {
            for (int i = 0; i <= node->n; ++i) {
                clear(node->C[i]);
            }
        }
        delete[] node->keys;
        delete[] node->C;
        delete node;
    }
};

// ---------------- NODE IMPLEMENTATION ----------------

template <typename T>
BTreeNode<T>::BTreeNode(int t1, bool leaf1)
    : t(t1), n(0), leaf(leaf1) {
    keys = new T[2 * t - 1];
    C = new BTreeNode<T>*[2 * t];
    for (int i = 0; i < 2 * t; ++i) C[i] = nullptr;
}

template <typename T>
void BTreeNode<T>::traverse(std::ostream& os) const {
    int i = 0;
    for (i = 0; i < n; i++) {
        if (!leaf) C[i]->traverse(os);
        os << keys[i] << ' ';
    }
    if (!leaf) C[i]->traverse(os);
}

template <typename T>
BTreeNode<T>* BTreeNode<T>::search(const T& k) {
    int i = 0;
    while (i < n && k > keys[i]) i++;
    if (i < n && keys[i] == k) return this;
    if (leaf) return nullptr;
    return C[i]->search(k);
}

template <typename T>
void BTree<T>::insert(const T& k) {
    if (root == nullptr) {
        root = new BTreeNode<T>(t, true);
        root->keys[0] = k;
        root->n = 1;
        return;
    }

    if (root->n == 2 * t - 1) {
        // Root is full, grow in height
        BTreeNode<T>* s = new BTreeNode<T>(t, false);
        s->C[0] = root;
        s->splitChild(0, root);

        int i = 0;
        if (s->keys[0] < k) i++;
        s->C[i]->insertNonFull(k);
        root = s;
    }
    else {
        root->insertNonFull(k);
    }
}

template <typename T>
void BTreeNode<T>::insertNonFull(const T& k) {
    int i = n - 1;

    if (leaf) {
        // Shift keys to insert in sorted order
        while (i >= 0 && keys[i] > k) {
            keys[i + 1] = keys[i];
            i--;
        }
        keys[i + 1] = k;
        n++;
    }
    else {
        // Find child to descend
        while (i >= 0 && keys[i] > k) i--;
        i++;
        // If the child is full, split it first
        if (C[i]->n == 2 * t - 1) {
            splitChild(i, C[i]);
            if (keys[i] < k) i++;
        }
        C[i]->insertNonFull(k);
    }
}

template <typename T>
void BTreeNode<T>::splitChild(int i, BTreeNode<T>* y) {
    // Create a new node to store the last (t-1) keys of y
    BTreeNode<T>* z = new BTreeNode<T>(y->t, y->leaf);
    z->n = t - 1;

    for (int j = 0; j < t - 1; j++)
        z->keys[j] = y->keys[j + t];

    if (!y->leaf) {
        for (int j = 0; j < t; j++)
            z->C[j] = y->C[j + t];
    }

    // Reduce number of keys in y
    y->n = t - 1;

    // Create space for new child
    for (int j = n; j >= i + 1; j--)
        C[j + 1] = C[j];

    C[i + 1] = z;

    // Move a key from y up to this node
    for (int j = n - 1; j >= i; j--)
        keys[j + 1] = keys[j];

    keys[i] = y->keys[t - 1];
    n++;
}

// ---------------- Deletion helpers ----------------

template <typename T>
int BTreeNode<T>::findKey(const T& k) const {
    int idx = 0;
    while (idx < n && keys[idx] < k) ++idx;
    return idx;
}

template <typename T>
void BTreeNode<T>::remove(const T& k) {
    int idx = findKey(k);

    if (idx < n && keys[idx] == k) {
        if (leaf)
            removeFromLeaf(idx);
        else
            removeFromNonLeaf(idx);
    }
    else {
        if (leaf) {
            // Key not present in tree
            return;
        }

        bool flag = (idx == n); // last child?

        if (C[idx]->n < t)
            fill(idx);

        // If we merged the last child with its sibling, the key might now be in C[idx-1]
        if (flag && idx > n) {
            C[idx - 1]->remove(k);
        }
        else {
            C[idx]->remove(k);
        }
    }
}

template <typename T>
void BTreeNode<T>::removeFromLeaf(int idx) {
    // Shift keys to fill the gap
    for (int i = idx + 1; i < n; ++i)
        keys[i - 1] = keys[i];
    n--;
}

template <typename T>
void BTreeNode<T>::removeFromNonLeaf(int idx) {
    T k = keys[idx];

    // Case 1: predecessor child has >= t keys
    if (C[idx]->n >= t) {
        T pred = getPred(idx);
        keys[idx] = pred;
        C[idx]->remove(pred);
    }
    // Case 2: successor child has >= t keys
    else if (C[idx + 1]->n >= t) {
        T succ = getSucc(idx);
        keys[idx] = succ;
        C[idx + 1]->remove(succ);
    }
    // Case 3: both children have t-1 keys, merge them
    else {
        merge(idx);
        C[idx]->remove(k);
    }
}

template <typename T>
T BTreeNode<T>::getPred(int idx) {
    // Move to the rightmost key of left subtree
    BTreeNode<T>* cur = C[idx];
    while (!cur->leaf)
        cur = cur->C[cur->n];
    return cur->keys[cur->n - 1];
}

template <typename T>
T BTreeNode<T>::getSucc(int idx) {
    // Move to the leftmost key of right subtree
    BTreeNode<T>* cur = C[idx + 1];
    while (!cur->leaf)
        cur = cur->C[0];
    return cur->keys[0];
}

template <typename T>
void BTreeNode<T>::fill(int idx) {
    // If the previous sibling has >= t keys, borrow from it
    if (idx != 0 && C[idx - 1]->n >= t)
        borrowFromPrev(idx);
    // Else if the next sibling has >= t keys, borrow from it
    else if (idx != n && C[idx + 1]->n >= t)
        borrowFromNext(idx);
    // Else merge with a sibling
    else {
        if (idx != n)
            merge(idx);
        else
            merge(idx - 1);
    }
}

template <typename T>
void BTreeNode<T>::borrowFromPrev(int idx) {
    BTreeNode<T>* child = C[idx];
    BTreeNode<T>* sibling = C[idx - 1];

    // Shift child's keys and children right by one to make room
    for (int i = child->n - 1; i >= 0; --i)
        child->keys[i + 1] = child->keys[i];

    if (!child->leaf) {
        for (int i = child->n; i >= 0; --i)
            child->C[i + 1] = child->C[i];
    }

    // Move a key from current node down to child
    child->keys[0] = keys[idx - 1];

    if (!child->leaf)
        child->C[0] = sibling->C[sibling->n];

    // Move sibling's last key up to current node
    keys[idx - 1] = sibling->keys[sibling->n - 1];

    child->n += 1;
    sibling->n -= 1;
}

template <typename T>
void BTreeNode<T>::borrowFromNext(int idx) {
    BTreeNode<T>* child = C[idx];
    BTreeNode<T>* sibling = C[idx + 1];

    // Bring down a key from current node to child's end
    child->keys[child->n] = keys[idx];

    if (!child->leaf)
        child->C[child->n + 1] = sibling->C[0];

    // Move sibling's first key up to current node
    keys[idx] = sibling->keys[0];

    // Shift sibling's keys and children left by one
    for (int i = 1; i < sibling->n; ++i)
        sibling->keys[i - 1] = sibling->keys[i];

    if (!sibling->leaf) {
        for (int i = 1; i <= sibling->n; ++i)
            sibling->C[i - 1] = sibling->C[i];
    }

    child->n += 1;
    sibling->n -= 1;
}

template <typename T>
void BTreeNode<T>::merge(int idx) {
    BTreeNode<T>* child = C[idx];
    BTreeNode<T>* sibling = C[idx + 1];

    // Pull down keys[idx] into child
    child->keys[t - 1] = keys[idx];

    // Copy sibling's keys and children into child
    for (int i = 0; i < sibling->n; ++i)
        child->keys[i + t] = sibling->keys[i];

    if (!child->leaf) {
        for (int i = 0; i <= sibling->n; ++i)
            child->C[i + t] = sibling->C[i];
    }

    // Shift keys and children in current node to close the gap
    for (int i = idx + 1; i < n; ++i)
        keys[i - 1] = keys[i];

    for (int i = idx + 2; i <= n; ++i)
        C[i - 1] = C[i];

    child->n += sibling->n + 1;
    n--;

    // Free memory of sibling
    delete[] sibling->keys;
    delete[] sibling->C;
    delete sibling;
}
