# B-Tree (C++ Template Implementation)

This project provides a header-only C++ implementation of a **B-Tree**, a balanced search tree commonly used in databases and filesystems.

---

## What is a B-Tree?

A B-Tree is a multi-way search tree that:

* Keeps data sorted.
* Automatically balances itself.
* Guarantees logarithmic time for search, insertion, and deletion.

---

## Features

* Template-based: works with integers, strings, or any comparable type.
* Core operations: insert, search, remove, traverse.
* Self-balancing with node splits (on insert) and merges/borrows (on delete).
* Configurable minimum degree `t` (controls branching).
* Header-only implementation.

---

## Quick Start

1. Create a tree by specifying its minimum degree.
2. Insert keys one by one.
3. Traverse the tree to see keys in sorted order.
4. Remove keys and observe how the tree restructures.

---

## Visual Intuition (ASCII Examples)

### Insertion

Insert the numbers: 10, 20, 5, 6

Step 1: start empty

```
[ ]
```

Step 2: insert 10

```
[10]
```

Step 3: insert 20

```
[10 20]
```

Step 4: insert 5

```
[5 10 20]
```

Step 5: insert 6 → split occurs

```
     [10]
    /    \
 [5 6]   [20]
```

---

### Deletion

Remove 6 from the tree above:

```
     [10]
    /    \
  [5]    [20]
```

Remove 20 → right child becomes empty → merge occurs:

```
[5 10]
```

---

### Internal Node Deletion

If you remove a key that lives in an internal node:

* Replace it with the predecessor (largest in left subtree) or successor (smallest in right subtree).
* If neither child has spare keys, merge them and continue the deletion.

---

## Complexity

* Search: O(log n)
* Insert: O(log n)
* Delete: O(log n)
* Traverse: O(n)

---

## Notes

* Copying a tree is disabled.
* Implementation uses raw pointers.
* Not thread-safe.
* Focused on learning and clarity.

---

## Applications

* Teaching and learning balanced trees.
* Basis for experimenting with database indexing.
* Use in file structures or other storage systems.
