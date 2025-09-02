#include "AVLTree.h"
#include <iostream>

int main() {
    AVLTree tree;

    // Insert values
    std::cout << "Inserting values: 10, 20, 30, 40, 50, 25\n";
    tree.insert(10);
    tree.insert(20);
    tree.insert(30);
    tree.insert(40);
    tree.insert(50);
    tree.insert(25);

    // Print different traversals
    tree.printAsList("inorder");
    tree.printAsList("preorder");
    tree.printAsList("postorder");
    tree.printAsList("levelorder");

    // Remove a value
    std::cout << "\nRemoving value 30...\n";
    tree.remove(30);
    tree.printAsList("inorder");

    // Check if values exist
    std::cout << "\nTree contains 25? " << (tree.contains(25) ? "Yes" : "No") << std::endl;
    std::cout << "Tree contains 30? " << (tree.contains(30) ? "Yes" : "No") << std::endl;

    return 0;
}