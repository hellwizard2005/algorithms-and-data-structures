## Overview

This header file provides a template-based implementation of a **B-Tree**, a balanced search tree widely used in databases and filesystems.
It defines two main classes: a node class that stores keys and child pointers, and a tree class that manages operations such as search, insertion, deletion, and traversal.

## Features

* **Generic implementation**: works with any data type that supports comparison operations.
* **Core operations supported**: searching for a key, inserting new keys, deleting existing keys, and traversing the entire tree in sorted order.
* **Self-balancing**: the B-Tree automatically splits or merges nodes as needed to maintain its structural properties.
* **Configurable degree**: when the tree is created, you specify its minimum degree, which defines how many keys each node can hold and how balanced the tree remains.

## Structure

* The **node class** manages the keys stored in a node and pointers to its child nodes. It contains helper methods for handling cases such as inserting into a non-full node, splitting a full node, borrowing keys from siblings, or merging nodes during deletion.
* The **tree class** maintains a reference to the root node and provides the public interface. It is responsible for initiating insertions, deletions, searches, and traversals.

## Memory Management

* Each node allocates its arrays of keys and children dynamically.
* The tree ensures that all allocated memory is released properly in its destructor.
* Copying of the tree is intentionally disabled to prevent issues with ownership of dynamically allocated nodes.

## Usage

* When creating a tree, you must specify the minimum degree. This value controls the maximum and minimum number of keys a node can store.
* The traversal method allows you to inspect the keys in sorted order.
* Searching returns the node where the key is found, or indicates that the key is not present.
* Insertions automatically rebalance the tree by splitting nodes if they overflow.
* Deletions handle all cases, including removing from leaves, removing from internal nodes, and redistributing or merging nodes when necessary.

## Complexity

* Searching, inserting, and deleting are all logarithmic in the number of keys.
* Traversal visits every key exactly once, making it linear in the size of the tree.

## Limitations

* The implementation uses raw pointers for node management, not modern C++ smart pointers.
* It is not thread-safe; synchronization must be handled externally if used in concurrent environments.
* Copying is disabled, so a tree cannot be duplicated directly.
* The implementation focuses on clarity and correctness rather than performance optimizations.

## Applications

B-Trees are particularly useful when working with large datasets, especially when stored on disk, because they minimize the number of disk accesses. They are the basis for many database indexing systems and file storage solutions.

How Insertion Works

When you insert a new key into a B-Tree:

Find the right node — the algorithm descends from the root, choosing child pointers just like in a binary search tree, until it reaches a leaf node.

Check for space — if the node has room for another key, the new key is simply placed in order.

Split if full — if the node is already full, it is split into two nodes:

The middle key is moved up into the parent.

The keys to the left and right of it form two separate nodes.

If the parent is also full, this splitting process continues upward, possibly creating a new root.

Balance maintained — thanks to splitting, the tree always stays balanced and shallow, keeping search time logarithmic.

Example in words: Imagine you’re trying to fit books on a shelf, but the shelf is full. Instead of cramming, you split the shelf in two and move one book up to the “main catalog.” That catalog entry ensures you can still quickly find the right shelf.

How Deletion Works

Deletion is trickier because we need to maintain the balance rules of the tree. There are several cases:

Deleting from a leaf node — if the key is in a leaf that has more than the minimum number of keys, just remove it. Easy case.

Deleting from an internal node — if the key is in a non-leaf:

If the left child has enough keys, replace the key with its predecessor (the largest key from the left subtree) and then remove that predecessor.

Otherwise, if the right child has enough keys, replace the key with its successor (the smallest key from the right subtree) and remove that successor.

If neither child has extra keys, merge the key and both children into a single node, and then continue deletion there.

Rebalancing when a child has too few keys — before descending into a child during deletion, the algorithm ensures the child has at least the minimum number of keys. If not, it fixes it by either:

Borrowing a key from a sibling that has extra keys (rotating keys through the parent), or

Merging the child with a sibling and moving a key down from the parent.

Example in words: Imagine you’re removing a book from a shelf, but the shelf would become too empty. You can either borrow a book from the neighboring shelf to keep things balanced, or merge two half-empty shelves into one fuller shelf.

Why This Matters

These rules guarantee that:

The tree never grows too tall.

Every node (except possibly the root) always has at least the minimum number of keys.

Searching, inserting, and deleting all remain efficient, even as the dataset grows.


# Visual Examples of B-Tree Operations

### Insertion with Split

Suppose we insert the numbers **10, 20, 5, 6** into a B-Tree of minimum degree 2.

1. Start with an empty tree:

```
[ ]
```

2. Insert 10:

```
[10]
```

3. Insert 20:

```
[10 20]
```

4. Insert 5 (keys stay sorted):

```
[5 10 20]
```

5. Insert 6 → node is full, so it splits:

* Middle key `10` moves up.
* Keys `[5 6]` go left, `[20]` goes right.

Result:

```
     [10]
    /    \
 [5 6]   [20]
```

---

### Deletion with Borrowing

Now suppose we remove **6** from the tree above.

1. Current tree:

```
     [10]
    /    \
 [5 6]   [20]
```

2. Remove 6 from the left child:

```
     [10]
    /    \
  [5]    [20]
```

No rebalancing needed, because the left node still has at least the minimum number of keys.

---

### Deletion with Merging

Now suppose we remove **20**.

1. Current tree:

```
     [10]
    /    \
  [5]    [20]
```

2. After removing 20, the right node becomes empty.
   So we **merge** the right node with its sibling and pull down `10` from the root:

```
   [5 10]
```

Now the root itself is just a single node again.

---

These diagrams show how B-Trees grow and shrink smoothly while staying balanced.
