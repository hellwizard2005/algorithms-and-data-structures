Overview

This repository contains a C++ implementation of an AVL tree, a self-balancing binary search tree. The AVL tree maintains its balance by ensuring that the heights of the two child subtrees of any node differ by at most one. This guarantees O(log n) time complexity for insertion, deletion, and search operations.
Features

    Self-balancing: Automatically maintains balance after insertions and deletions

    Standard operations:

        Insertion of values

        Deletion of values

        Search for values

    Multiple traversal methods:

        In-order traversal (returns sorted list)

        Pre-order traversal

        Post-order traversal

        Level-order (breadth-first) traversal

    Clean output: Methods to print the tree in different traversal orders

Implementation Details

The implementation includes:

    Node structure with:

        Integer value storage

        Left and right child pointers

        Height tracking

    Core balancing operations:

        Right rotations (for left-heavy trees)

        Left rotations (for right-heavy trees)

        Double rotations when needed

    Helper functions for:

        Height calculation

        Balance factor calculation

        Height updates
        
Building and Testing

The implementation is header-only (AVLTree.h). Simply include it in your project. A sample test file (test.cpp) is provided that demonstrates basic functionality.

To compile and run the test:

g++ test.cpp -o avl_test
./avl_test

Performance

All major operations have O(log n) time complexity:

    Insertion: O(log n)

    Deletion: O(log n)

    Search: O(log n)

    Traversals: O(n)

Contributing

Contributions are welcome! Please open an issue or submit a pull request for any improvements or bug fixes.
License

This code is provided as-is under the MIT License. See LICENSE file for details.
