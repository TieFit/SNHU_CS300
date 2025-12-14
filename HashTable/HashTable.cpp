//============================================================================
// Name        : HashTable.cpp
// Author      : JJ Newell
// Version     : 1.0
// Copyright   : Copyright � 2023 SNHU COCE
// Description : Lab 4-2 Hash Table
//============================================================================

#include <algorithm>
#include <climits>
#include <iostream>
#include <time.h>

#include "CSVparser.hpp"

using namespace std;

//============================================================================
// Global definitions visible to all methods and classes
//============================================================================

const unsigned int DEFAULT_SIZE = 179;

// forward declarations
double strToDouble(string str, char ch);

// define a structure to hold bid information
struct Bid {
    string bidId;
    string title;
    string fund;
    double amount;
    Bid() {
        amount = 0.0;
    }
};

//============================================================================
// Hash Table class definition
//============================================================================

class HashTable {

private:
    struct Node {
        Bid bid;
        unsigned int key;
        Node *next;

        // used in the remove function as a deletion flag
        bool deleted;

        // default constructor
        Node() {
            key = UINT_MAX;
            next = nullptr;
        }

        // initialize with a bid
        Node(Bid aBid) : Node() {
            bid = aBid;
        }

        // initialize with a bid and a key
        Node(Bid aBid, unsigned int aKey) : Node(aBid) {
            key = aKey;
        }
    };

    vector<Node> nodes;
    unsigned int tableSize = DEFAULT_SIZE;
    unsigned int hash(int key);

public:
    HashTable();
    HashTable(unsigned int size);
    virtual ~HashTable();
    void Insert(Bid bid);
    void PrintAll();
    void Remove(string bidId);
    Bid Search(string bidId);
    size_t Size();
    bool deleted;
};

// default constructor
HashTable::HashTable() {
    nodes.resize(tableSize);
}

// constructor for specifying size of the table
// use to improve efficiency of hashing algorithm by reducing collisions without wasting memory
HashTable::HashTable(unsigned int size) {
    tableSize = size;
    nodes.resize(tableSize);
}

// destructor to free storage when class is cleared
HashTable::~HashTable() {
    nodes.clear();
}

// calculate hash value of a given key. Key is defined as unsigned int to prevent
// undefined results of a negative list index
unsigned int HashTable::hash(int key) {
    return key % tableSize;
}


 // insert a bid
void HashTable::Insert(Bid bid) {
    unsigned int key = hash(stoi(bid.bidId));

    // retrieve node using key
    Node* newNode = new Node(bid, key);
    Node* current = &nodes[key];

    // sets this value to false as it is important later on in regards to the remove and search functions
    newNode->deleted = false;

    // if no entry found for the key
    if (current->key == UINT_MAX) {

        // assign this node to the key position
        nodes[key] = *newNode;
    }

    // else if node is not used
    else {
        while (current->next != nullptr) {
            current = current->next;
        }

        // else find the next open node
        // add new newNode to end
        current->next = newNode;
    }        
}

// print all bids
void HashTable::PrintAll() {
    for (unsigned int i = 0; i < nodes.size(); i++) {
        Node* node = &nodes[i];

        // iterate over all nodes in the table
        while (node != nullptr && node->key != UINT_MAX) {

            // output key, bidID, title, amount and fund of each bid/node
            cout << "Key: " << node->key << ", Bid ID: " << node->bid.bidId
                << ", Title: " << node->bid.title
                << ", Amount: " << node->bid.amount
                << ", Fund: " << node->bid.fund << endl;
            node = node->next;
        }
    }
}

// remove a bid
void HashTable::Remove(string bidId) {
    unsigned int key = hash(stoi(bidId));
    Node* current = &nodes[key];
    
    if (current->key == UINT_MAX) {
        cout << "No bid found with ID: " << bidId << endl;
        return;
    }

    // check if the first node is the one to delete
    if (current->bid.bidId == bidId) {

        current->deleted = true;  // flag the node as deleted
        current->key = UINT_MAX;  // mark the current slot as empty
        current->next = nullptr;  // disconnect the next node
        cout << "Bid " << bidId << " has been deleted." << endl;
    }

    else { // traverse the list to find the bidId and delete it

        Node* prev = nullptr;

        while (current != nullptr && current->bid.bidId != bidId) {
            prev = current;
            current = current->next;
        }

        if (current == nullptr) {
            cout << "No bid found with ID: " << bidId << endl;
            return;
        }

        current->deleted = true;

        // update the next pointer of the previous node to bypass current node
        prev->next = current->next;
        
        // delete current node to deallocate memory
        delete current;

        cout << "Bid " << bidId << " has been deleted." << endl;
    }
}

// search for bid
Bid HashTable::Search(string bidId) {
    Bid bid;
    unsigned int key = hash(stoi(bidId));
    Node* current = &nodes[key];
    
    // loop over linked list to find the specified bidId
    while (current != nullptr) {
        if (current->bid.bidId == bidId) {

            // check if this current node has been marked as deleted. 
            // this also might be redundant as this was created when solving the earlier issue of a deleted bid
            // persisting after deletion. However it is fine with it still being here
            if (current->deleted) {
                cout << "Bid with ID: " << bidId << "has been deleted" << endl;
                return current->bid;  
            }
            else {
                cout << "Bid found: " << current->bid.bidId << endl;
                return current->bid;
            }
        }

        // move to the next node in the list to continue searching
        current = current->next;
    }
}

// display bid information to the console
void displayBid(Bid bid) {
    cout << bid.bidId << ": " << bid.title << " | " << bid.amount << " | "
            << bid.fund << endl;
    return;
}

// load CSV file containing bids into container, csvPath is the pathway for this file
void loadBids(string csvPath, HashTable* hashTable) {
    cout << "Loading CSV file " << csvPath << endl;

    // initialize the CSV Parser using the given path
    csv::Parser file = csv::Parser(csvPath);

    // read and display header row
    vector<string> header = file.getHeader();
    for (auto const& c : header) {
        cout << c << " | ";
    }
    cout << "" << endl;

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
            hashTable->Insert(bid);
        }
    } catch (csv::Error &e) {
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
        csvPath = "eBid_Monthly_Sales_Dec_2016.csv";
        bidKey = "98223";
    }

    // Define a timer variable
    clock_t ticks;
    HashTable* bidTable;

    Bid bid;
    bidTable = new HashTable();
    
    int choice = 0;
    while (choice != 9) {
        cout << "\nMenu:" << endl;
        cout << "  1. Load Bids" << endl;
        cout << "  2. Display All Bids" << endl;
        cout << "  3. Find Bid" << endl;
        cout << "  4. Remove Bid" << endl;
        cout << "  9. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;
        cout << "\n";

        switch (choice) {

        case 1:
            
            // Initialize a timer variable before loading bids
            ticks = clock();

            loadBids(csvPath, bidTable);

            // Calculate elapsed time and display result
            ticks = clock() - ticks; 
            cout << "time: " << ticks << " clock ticks" << endl;
            cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;
            break;

        case 2:
            bidTable->PrintAll();
            break;

        case 3:
            ticks = clock();

            bid = bidTable->Search(bidKey);

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
            bidTable->Remove(bidKey);
            break;
        }
    }
    cout << "Good bye." << endl;
    return 0;
}