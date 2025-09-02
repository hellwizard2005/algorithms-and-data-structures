#ifndef AVLTREE_H
#define AVLTREE_H

#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
#include <string>

class AVLTree {
private:
    // Node structure for AVL Tree
    struct Node {
        int value;       // Value stored in the node
        Node* left;      // Pointer to left child
        Node* right;     // Pointer to right child
        int height;      // Height of the node

        // Constructor
        Node(int val) : value(val), left(nullptr), right(nullptr), height(1) {}
    };

    Node* root;     // Root of the AVL tree

    // Helper function to get height of a node
    int getHeight(Node* node) {
        return node ? node->height : 0;
    }

    // Calculate balance factor of a node
    int getBalanceFactor(Node* node) {
        return node ? getHeight(node->left) - getHeight(node->right) : 0;
    }

    // Update height of a node based on children's heights
    void updateHeight(Node* node) {
        if (node) {
            node->height = 1 + std::max(getHeight(node->left), getHeight(node->right));
        }
    }

    // Right rotation to fix left-heavy subtree
    Node* rotateRight(Node* y) {
        Node* x = y->left;
        Node* T2 = x->right;

        // Perform rotation
        x->right = y;
        y->left = T2;

        // Update heights
        updateHeight(y);
        updateHeight(x);

        return x;  // New root after rotation
    }

    // Left rotation to fix right-heavy subtree
    Node* rotateLeft(Node* x) {
        Node* y = x->right;
        Node* T2 = y->left;

        // Perform rotation
        y->left = x;
        x->right = T2;

        // Update heights
        updateHeight(x);
        updateHeight(y);

        return y;  // New root after rotation
    }

    // Balance the node if it's unbalanced
    Node* balance(Node* node) {
        if (!node) return nullptr;

        updateHeight(node);
        int balanceFactor = getBalanceFactor(node);

        // Left Left Case (Single right rotation)
        if (balanceFactor > 1 && getBalanceFactor(node->left) >= 0) {
            return rotateRight(node);
        }

        // Left Right Case (Left rotation then right rotation)
        if (balanceFactor > 1 && getBalanceFactor(node->left) < 0) {
            node->left = rotateLeft(node->left);
            return rotateRight(node);
        }

        // Right Right Case (Single left rotation)
        if (balanceFactor < -1 && getBalanceFactor(node->right) <= 0) {
            return rotateLeft(node);
        }

        // Right Left Case (Right rotation then left rotation)
        if (balanceFactor < -1 && getBalanceFactor(node->right) > 0) {
            node->right = rotateRight(node->right);
            return rotateLeft(node);
        }

        return node;  // Node is already balanced
    }

    // Insert a value into the subtree rooted at 'node'
    Node* insert(Node* node, int value) {
        // Base case: create new node
        if (!node) return new Node(value);

        // Recursive insertion
        if (value < node->value) {
            node->left = insert(node->left, value);
        } else if (value > node->value) {
            node->right = insert(node->right, value);
        } else {
            return node;  // Duplicate values not allowed
        }

        return balance(node);  // Balance the tree after insertion
    }

    // Find node with minimum value in a subtree
    Node* findMin(Node* node) {
        while (node && node->left) {
            node = node->left;
        }
        return node;
    }

    // Remove a value from the subtree rooted at 'node'
    Node* remove(Node* node, int value) {
        if (!node) return nullptr;

        // Search for the node to delete
        if (value < node->value) {
            node->left = remove(node->left, value);
        } else if (value > node->value) {
            node->right = remove(node->right, value);
        } else {
            // Node found - handle deletion based on number of children
            if (!node->left || !node->right) {
                Node* temp = node->left ? node->left : node->right;
                
                // No child case
                if (!temp) {
                    temp = node;
                    node = nullptr;
                } else {
                    // One child case
                    *node = *temp;  // Copy contents
                }
                
                delete temp;
            } else {
                // Two children case
                Node* temp = findMin(node->right);
                node->value = temp->value;
                node->right = remove(node->right, temp->value);
            }
        }

        if (!node) return nullptr;

        return balance(node);  // Balance the tree after deletion
    }

    // Tree traversal methods (store results in a vector)
    void inOrderTraversal(Node* node, std::vector<int>& result) {
        if (node) {
            inOrderTraversal(node->left, result);
            result.push_back(node->value);
            inOrderTraversal(node->right, result);
        }
    }

    void preOrderTraversal(Node* node, std::vector<int>& result) {
        if (node) {
            result.push_back(node->value);
            preOrderTraversal(node->left, result);
            preOrderTraversal(node->right, result);
        }
    }

    void postOrderTraversal(Node* node, std::vector<int>& result) {
        if (node) {
            postOrderTraversal(node->left, result);
            postOrderTraversal(node->right, result);
            result.push_back(node->value);
        }
    }

    void levelOrderTraversal(Node* node, std::vector<int>& result) {
        if (!node) return;

        std::queue<Node*> q;
        q.push(node);

        while (!q.empty()) {
            Node* current = q.front();
            q.pop();
            result.push_back(current->value);

            if (current->left) q.push(current->left);
            if (current->right) q.push(current->right);
        }
    }

public:
    // Constructor
    AVLTree() : root(nullptr) {}

    // Destructor
    ~AVLTree() {
        clear(root);
    }

    // Helper function for destructor
    void clear(Node* node) {
        if (node) {
            clear(node->left);
            clear(node->right);
            delete node;
        }
    }

    // Public interface for insertion
    void insert(int value) {
        root = insert(root, value);
    }

    // Public interface for deletion
    void remove(int value) {
        root = remove(root, value);
    }

    // Check if value exists in the tree
    bool contains(int value) {
        Node* current = root;
        while (current) {
            if (value == current->value) return true;
            current = value < current->value ? current->left : current->right;
        }
        return false;
    }

    // Traversal methods that return vectors
    std::vector<int> toInOrderList() {
        std::vector<int> result;
        inOrderTraversal(root, result);
        return result;
    }

    std::vector<int> toPreOrderList() {
        std::vector<int> result;
        preOrderTraversal(root, result);
        return result;
    }

    std::vector<int> toPostOrderList() {
        std::vector<int> result;
        postOrderTraversal(root, result);
        return result;
    }

    std::vector<int> toLevelOrderList() {
        std::vector<int> result;
        levelOrderTraversal(root, result);
        return result;
    }

    // Print tree elements as a list
    void printAsList(const std::string& order = "inorder") {
        std::vector<int> elements;

        if (order == "inorder") {
            elements = toInOrderList();
            std::cout << "In-order traversal (sorted): [";
        } else if (order == "preorder") {
            elements = toPreOrderList();
            std::cout << "Pre-order traversal: [";
        } else if (order == "postorder") {
            elements = toPostOrderList();
            std::cout << "Post-order traversal: [";
        } else if (order == "levelorder") {
            elements = toLevelOrderList();
            std::cout << "Level-order traversal: [";
        } else {
            std::cout << "Unknown traversal type. Using inorder by default.\n";
            elements = toInOrderList();
            std::cout << "In-order traversal (sorted): [";
        }

        for (size_t i = 0; i < elements.size(); ++i) {
            std::cout << elements[i];
            if (i != elements.size() - 1) std::cout << ", ";
        }
        std::cout << "]\n";
    }
};

#endif // AVL_TREE_H