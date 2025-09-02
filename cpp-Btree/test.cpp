#include <iostream>
#include "b-tree.h"

int main() {
    // B-Tree of minimum degree 3 (each node: 2..5 keys, except root may have fewer)
    BTree<int> bt(3);

    // Insert a bunch of keys
    int values[] = { 10, 20, 5, 6, 12, 30, 7, 17, 3, 4, 2, 40, 50, 60, 1, 8, 9, 11, 13, 14 };
    for (int v : values) bt.insert(v);

    std::cout << "Traversal after insertions:\n";
    bt.traverse();
    std::cout << "\n";

    // Search tests
    int toFind[] = { 6, 15, 30, 100 };
    for (int x : toFind) {
        auto* node = bt.search(x);
        std::cout << "Search " << x << ": " << (node ? "found" : "not found") << "\n";
    }

    // Deletions with intermediate traversals
    int toDelete[] = { 6, 13, 7, 4, 2, 12, 30, 10, 20, 5, 3, 1, 9, 8, 11, 14, 17, 40, 50, 60 };
    for (int d : toDelete) {
        bt.remove(d);
        std::cout << "After deleting " << d << ":\n";
        bt.traverse();
        std::cout << "\n";
    }

    // Final state
    std::cout << "Final traversal (should be empty tree):\n";
    bt.traverse();
    std::cout << "\n";

    return 0;
}
