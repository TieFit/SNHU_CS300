//============================================================================
// Name        : BinarySearchTree.cpp
// Author      : JJ Newell
// Version     : 1.0
// Copyright   : Copyright � 2023 SNHU COCE
// Description : Lab 5-2 Binary Search Tree
//============================================================================

#include <iostream>
#include <time.h>
#include <algorithm>
#include <cstdlib>
#include "CSVparser.hpp"

using namespace std;

//============================================================================
// Global definitions visible to all methods and classes
//============================================================================

// forward declarations
double strToDouble(string str, char ch);

// define a structure to hold bid information
struct Bid {
    string bidId; // unique identifier
    string title;
    string fund;
    double amount;
    Bid() {
        amount = 0.0;
    }
};

// Internal structure for tree node
struct Node {
    Bid bid;
    Node *left;
    Node *right;

    // default constructor
    Node() {
        left = nullptr;
        right = nullptr;
    }

    // initialize with a bid
    Node(const Bid& aBid) :
        Node() {
        bid = aBid;
    }
};

//============================================================================
// Binary Search Tree class definition
//============================================================================

class BinarySearchTree {

private:
    Node* root;

    void addNode(Node* node, const Bid& bid);
    void inOrder(Node* node);
    void postOrder(Node* node);
    void preOrder(Node* node);
    Node* removeNode(Node* node, string bidId);
    Node* minNode(Node* node);
    void deleteNode(Node* node);

public:
    BinarySearchTree();
    virtual ~BinarySearchTree();
    void InOrder();
    void PostOrder();
    void PreOrder();
    void Insert(const Bid& bid);
    void Remove(const string& bidId);
    Bid Search(const string& bidId);
};

// default constructor
BinarySearchTree::BinarySearchTree() {
    root = nullptr;
}

// destructor
BinarySearchTree::~BinarySearchTree() {
    deleteNode(root);
}

// function to recursively delete nodes in the tree
void BinarySearchTree::deleteNode(Node* node) {
    if (node != nullptr) {
        // recursively deleted left and right subtrees
        deleteNode(node->left);
        deleteNode(node->right);

        delete node;
    }
}

// traverse tree in order
void BinarySearchTree::InOrder() {
    inOrder(root);
}

void BinarySearchTree::inOrder(Node* node) {
    if (node != nullptr) {
        // traverse left subtree
        inOrder(node->left);

        // display bidID, title, amount, fund
        displayBid(node->bid);

        // traverse right subtree
        inOrder(node->right);
    }
}

// traverse tree in post order
void BinarySearchTree::PostOrder() {
    postOrder(root);
}

void BinarySearchTree::postOrder(Node* node) {
    if (node != nullptr) {
        // traverse left subtree
        postOrder(node->left);

        // traverse right subtree
        postOrder(node->right);

        // display bidID, title, amount, fund
        displayBid(node->bid);

    }
}

// traverse tree in pre order
void BinarySearchTree::PreOrder() {
    preOrder(root);
}

void BinarySearchTree::preOrder(Node* node) {
    if (node != nullptr) {
        // display bidID, title, amount, fund
        displayBid(node->bid);

        // traverse left subtree
        preOrder(node->left);

        // traverse right subtree
        preOrder(node->right);
    }
}



// inserting a bid
void BinarySearchTree::Insert(const Bid& bid) {
    if (root == nullptr) {
        root = new Node(bid);
    }
    else {
        // insert bid into the tree, starting from the root
        addNode(root, bid);
    }
}

void BinarySearchTree::addNode(Node* node, const Bid& bid) {
    // if node is larger, then add to left
    if (bid.bidId < node->bid.bidId) {

        // If bidId is smaller, insert into the left subtree
        if (node->left == nullptr) {

            // this node becomes left
            node->left = new Node(bid);
        }

        // else recurse down the left node
        else {
            addNode(node->left, bid);
        }
    }

    else {
        // If bidId is larger or equal, insert into the right subtree
        if (node->right == nullptr) {

            // this node becomes right
            node->right = new Node(bid);
        }

        else {
            // recurse down the left node
            addNode(node->right, bid);
        }
    }
}

// removing a bid
void BinarySearchTree::Remove(const string& bidId) {
    root = removeNode(root, bidId);
}


// recursively remove a bid from a node
Node* BinarySearchTree::removeNode(Node* node, const string& bidId) {
    if (node == nullptr) {
        return node;
    }
    // recurse down the left subtree
    if (bidId < node->bid.bidId) {
        // check for match and if equal, remove left node using recursive call 
        node->left = removeNode(node->left, bidId);
    }

    // recurse down the right subtree
    else if (bidId > node->bid.bidId) {
        // check for match and if equal, remove right node using recursive call
        node->right = removeNode(node->right, bidId);
    }

    // else, no children so node is a leaf node
    else {
        if (node->left == nullptr && node->right == nullptr) {
            delete node;  // Node is a leaf, delete it
            return nullptr;
        }

        // check one child to the left
        else if (node->left == nullptr) {
            Node* temp = node->right;
            delete node;  // Node has only right child
            return temp;
        }
        else if (node->right == nullptr) {
            Node* temp = node->left;
            delete node;  // Node has only left child
            return temp;
        }
        else {
            // Node has two children: find the in-order successor
            Node* temp = minNode(node->right);  // Find the smallest node in the right subtree

            // make node bid (right) equal to temp bid (left)
            node->bid = temp->bid;

            // remove right node using recursive call
            node->right = removeNode(node->right, temp->bid.bidId);
        }
    }
    return node;
}

// search for bid
Bid BinarySearchTree::Search(string bidId) {
    Node* current = root;

    // keep looping downwards until bottom reached or matching bidId found
    while (current != nullptr) {

        // if match found, return current bid
        if (bidId == current->bid.bidId) {
            return current->bid;
        }

        // if bid is smaller than current node then traverse left
        // else larger so traverse right
        current = (bidId < current->bid.bidId) ? current->left : current->right;
    }

    // return empty bid if not found   
    Bid bid;
    return bid;
}

Node* BinarySearchTree::minNode(Node* node) {
    while (node && node->left != nullptr) {
        node = node->left;  // Move to the leftmost node
    }
    return node;
}

//============================================================================
// Static methods used for testing
//============================================================================

// function to display bid information
void displayBid(const Bid& bid) {
    cout << bid.bidId << ": " << bid.title << " | " << bid.amount << " | "
        << bid.fund << '\n';
    return;
}

/**
 * Load a CSV file containing bids into a container
 *
 * @param csvPath the path to the CSV file to load
 * @return a container holding all the bids read
 */
// load a CSV file containing bids into a container, csvPath being the filepath for the CSV
void loadBids(string csvPath, BinarySearchTree* bst) {
    cout << "Loading CSV file " << csvPath << endl;

    csv::Parser file = csv::Parser(csvPath);

    // read and display header row 
    vector<string> header = file.getHeader();
    for (auto const& c : header) {
        cout << c << " | ";
    }
    cout << '\n';

    try {
        // loop to read rows of a CSV file
        for (unsigned int i = 0; i < file.rowCount(); i++) {

            // Create a data structure and add to the collection of bids
            Bid bid;
            bid.bidId = file[i][1];
            bid.title = file[i][0];
            bid.fund = file[i][8];
            bid.amount = strToDouble(file[i][4], '$');

            //cout << "Item: " << bid.title << ", Fund: " << bid.fund << ", Amount: " << bid.amount << endl;

            // push this bid to the end
            bst->Insert(bid);
        }
    }
    catch (csv::Error& e) {
        std::cerr << e.what() << std::endl;
    }
}

// C function to convert a string to a double after stripping out unwanted char
// credit: http://stackoverflow.com/a/24875936: 
double strToDouble(string str, char ch) {
    str.erase(remove(str.begin(), str.end(), ch), str.end());
    return atof(str.c_str());
}

int main(int argc, char* argv[]) {

    // process command line arguments
    string csvPath, bidKey;
    switch (argc) {
    case 2:
        csvPath = argv[1];
        bidKey = "98223";
        break;
    case 3:
        csvPath = argv[1];
        bidKey = argv[2];
        break;
    default:
        // loads much quicker with smaller csv file, better for debugging
        csvPath = "eBid_Monthly_Sales_Dec_2016.csv";
        bidKey = "98223";
    }

    // Define a timer variable
    clock_t ticks;

    // Define a binary search tree to hold all bids
    BinarySearchTree* bst;
    bst = new BinarySearchTree();
    Bid bid;

    int choice = 0;
    while (choice != 9) {
        cout << "Menu:" << endl;
        cout << "  1. Load Bids" << endl;
        cout << "  2. Display All Bids" << endl;
        cout << "  3. Find Bid" << endl;
        cout << "  4. Remove Bid" << endl;
        cout << "  9. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {

        case 1:
            // Initialize a timer variable before loading bids
            ticks = clock();

            loadBids(csvPath, bst);

            //cout << bst->Size() << " bids read" << endl;

            // Calculate elapsed time and display result
            ticks = clock() - ticks; 
            cout << "time: " << ticks << " clock ticks" << endl;
            cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;
            break;

        case 2:
            bst->InOrder();
            break;

        case 3:
            ticks = clock();
            bid = bst->Search(bidKey);
            ticks = clock() - ticks;

            if (!bid.bidId.empty()) {
                displayBid(bid);
            }
            else {
                cout << "Bid Id " << bidKey << " not found." << endl;
            }

            cout << "time: " << ticks << " clock ticks" << endl;
            cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;
            break;

        case 4:
            bst->Remove(bidKey);
            break;
        }
    }
    cout << "Good bye." << endl;
    return 0;
}