//============================================================================
// Name        : HashTable.cpp
// Author      : JJ Newell
// Version     : 1.0
// Copyright   : Copyright © 2023 SNHU COCE
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
    string bidId; // unique identifier
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

/**
 * Define a class containing data members and methods to
 * implement a hash table with chaining.
 */
class HashTable {

private:
    // Define structures to hold bids
    struct Node {
        Bid bid;
        unsigned int key;
        Node *next;

        // used in the remove function
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

/**
 * Default constructor
 */
HashTable::HashTable() {
    // FIXME (1): Initialize the structures used to hold bids
    
    // Initalize node structure by resizing tableSize
    nodes.resize(tableSize);
}

/**
 * Constructor for specifying size of the table
 * Use to improve efficiency of hashing algorithm
 * by reducing collisions without wasting memory.
 */
HashTable::HashTable(unsigned int size) {
    // invoke local tableSize to size with this->
    tableSize = size;

    // resize nodes size
    nodes.resize(tableSize);
}


/**
 * Destructor
 */
HashTable::~HashTable() {
    // FIXME (2): Implement logic to free storage when class is destroyed
    
    // erase nodes beginning
    nodes.clear();
}

/**
 * Calculate the hash value of a given key.
 * Note that key is specifically defined as
 * unsigned int to prevent undefined results
 * of a negative list index.
 *
 * @param key The key to hash
 * @return The calculated hash
 */
unsigned int HashTable::hash(int key) {
    // FIXME (3): Implement logic to calculate a hash value
    // return key tableSize
    return key % tableSize;
}

/**
 * Insert a bid
 *
 * @param bid The bid to insert
 */
void HashTable::Insert(Bid bid) {
    // FIXME (4): Implement logic to insert a bid
    // create the key for the given bid
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

        // passing old node key to UNIT_MAX, set to key, set old node to bid and old node next to null pointer
        while (current->next != nullptr) {
            current = current->next;
        }

        // else find the next open node
        // add new newNode to end
        current->next = newNode;
    }        
}

/**
 * Print all bids
 */
void HashTable::PrintAll() {
    // FIXME (5): Implement logic to print all bids
    // for node begin to end iterate
    for (unsigned int i = 0; i < nodes.size(); i++) {
        Node* node = &nodes[i];

        // while node not equal to nullptr and key not equal to UNIT_MAX
        while (node != nullptr && node->key != UINT_MAX) {

            // output key, bidID, title, amount and fund
            // node is equal to next node
            cout << "Key: " << node->key << ", Bid ID: " << node->bid.bidId
                << ", Title: " << node->bid.title
                << ", Amount: " << node->bid.amount
                << ", Fund: " << node->bid.fund << endl;
            node = node->next;
        }
    }
}

/**
 * Remove a bid
 *
 * @param bidId The bid id to search for
 */
void HashTable::Remove(string bidId) {
    // FIXME (6): Implement logic to remove a bid
    // set key equal to hash stoi bidID cstring
    unsigned int key = hash(stoi(bidId));
    Node* current = &nodes[key];
    
    // if current key is equal to UNIT_MAX, return and end function
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
    else {

        // Traverse the list to find the bidId and remove it
        Node* prev = nullptr;

        // loop through linked list to find the node with the matching bidId
        while (current != nullptr && current->bid.bidId != bidId) {
            prev = current;
            current = current->next;
        }

        // checks if the bidId was not found
        if (current == nullptr) {
            cout << "No bid found with ID: " << bidId << endl;
            return;
        }

        // mark current as deleted
        current->deleted = true;

        // update the next pointer of the previous node to bypass current node
        prev->next = current->next;
        
        // delete current node to deallocate memory
        delete current;

        // output message that verifies the bid was deleted
        cout << "Bid " << bidId << " has been deleted." << endl;
    }
}

/**
 * Search for the specified bidId
 *
 * @param bidId The bid id to search for
 */
Bid HashTable::Search(string bidId) {
    Bid bid;

    // FIXME (7): Implement logic to search for and return a bid

    // create the key for the given bid
    unsigned int key = hash(stoi(bidId));
    Node* current = &nodes[key];

    // check if current node is equal to UINT_MAX. this was added as detection in preventing a deleted bid from appearing when searched for after deletion.
    // this if statement is a bit redundant as the issue being faced earlier was solved, however it doesn't harm anything by still being here
    if (current->key == UINT_MAX) {
        cout << "No bid found with ID: " << bidId << endl;
        return bid;
        
    }
    
    // loop over linked list to find the specified bidId
    while (current != nullptr) {

        // if current node equals the specified bidId
        if (current->bid.bidId == bidId) {

            // check if this current node has been marked as deleted. this also might be redundant as this was created when solving the earlier issue of a deleted bid
            // persisting after deletion. However it is fine with it still being here
            if (current->deleted) {
                cout << "Bid with ID: " << bidId << "has been deleted" << endl;
                return current->bid;  
            }

            // else output the bid found
            else {
                cout << "Bid found: " << current->bid.bidId << endl;
                return current->bid;
            }
        }

        // move to the next node in the list to continue searching
        current = current->next;
    }
}

//============================================================================
// Static methods used for testing
//============================================================================

/**
 * Display the bid information to the console (std::out)
 *
 * @param bid struct containing the bid info
 */
void displayBid(Bid bid) {
    cout << bid.bidId << ": " << bid.title << " | " << bid.amount << " | "
            << bid.fund << endl;
    return;
}

/**
 * Load a CSV file containing bids into a container
 *
 * @param csvPath the path to the CSV file to load
 * @return a container holding all the bids read
 */
void loadBids(string csvPath, HashTable* hashTable) {
    cout << "Loading CSV file " << csvPath << endl;

    // initialize the CSV Parser using the given path
    csv::Parser file = csv::Parser(csvPath);

    // read and display header row - optional
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

/**
 * Simple C function to convert a string to a double
 * after stripping out unwanted char
 *
 * credit: http://stackoverflow.com/a/24875936
 *
 * @param ch The character to strip out
 */
double strToDouble(string str, char ch) {
    str.erase(remove(str.begin(), str.end(), ch), str.end());
    return atof(str.c_str());
}

/**
 * The one and only main() method
 */
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

    // Define a hash table to hold all the bids
    HashTable* bidTable;

    Bid bid;
    bidTable = new HashTable();
    
    int choice = 0;
    while (choice != 9) {

        // added newlines for a bit of whitespace to make the menu more readable during program execution
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

            // Complete the method call to load the bids
            loadBids(csvPath, bidTable);

            // Calculate elapsed time and display result
            ticks = clock() - ticks; // current clock ticks minus starting clock ticks
            cout << "time: " << ticks << " clock ticks" << endl;
            cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;
            break;

        case 2:
            bidTable->PrintAll();
            break;

        case 3:
            ticks = clock();

            bid = bidTable->Search(bidKey);

            ticks = clock() - ticks; // current clock ticks minus starting clock ticks


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
