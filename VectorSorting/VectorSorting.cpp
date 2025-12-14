//============================================================================
// Name        : VectorSorting.cpp
// Author      : JJ Newell
// Version     : 1.0
// Copyright   : Copyright � 2023 SNHU COCE
// Description : Vector Sorting Algorithms
//============================================================================
#include <algorithm>
#include <iostream>
#include <time.h>
#include "CSVparser.hpp"
#include "CSVparser.cpp"

using namespace std;

//============================================================================
// Global definitions visible to all methods and classes
//============================================================================

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
// Static methods used for testing
//============================================================================

// display bid info to console
void displayBid(Bid bid) {
    cout << bid.bidId << ": " << bid.title << " | " << bid.amount << " | "
            << bid.fund << endl;
    return;
}

// get bid information from user
Bid getBid() {
    Bid bid;

    cout << "Enter Id: ";
    cin.ignore();
    getline(cin, bid.bidId);

    cout << "Enter title: ";
    getline(cin, bid.title);

    cout << "Enter fund: ";
    cin >> bid.fund;

    cout << "Enter amount: ";
    cin.ignore();
    string strAmount;
    getline(cin, strAmount);
    bid.amount = strToDouble(strAmount, '$');

    return bid;
}

// load bids from csv file
vector<Bid> loadBids(string csvPath) {
    cout << "Loading CSV file " << csvPath << endl;

    vector<Bid> bids;

    // initialize the CSV Parser using the given path
    csv::Parser file = csv::Parser(csvPath);

    try {
        // loop to read rows of a CSV file
        for (int i = 0; i < file.rowCount(); i++) {
            Bid bid;
            bid.bidId = file[i][1];
            bid.title = file[i][0];
            bid.fund = file[i][8];
            bid.amount = strToDouble(file[i][4], '$');

            //cout << "Item: " << bid.title << ", Fund: " << bid.fund << ", Amount: " << bid.amount << endl;

            // push this bid to the end
            bids.push_back(bid);
        }
    } catch (csv::Error &e) {
        std::cerr << e.what() << std::endl;
    }
    return bids;
}

// partition vector into low and high parts
int partition(vector<Bid>& bids, int begin, int end) {
    int low = begin;
    int high = end;

    // Calculate the middle element as midpoint (int)
    int midpoint = (begin + end) / 2;
    // Set Pivot as midpoint element title to compare (string)
    Bid pivot = bids.at(midpoint);

    bool done = false;
    while (!done) {
        while (bids.at(low).title < pivot.title) {
            ++low;
        }
        while (pivot.title < bids.at(high).title) {
            --high;
        }
        if (low >= high) {
            done = true;
        }
        else {
            swap(bids.at(low), bids.at(high));
            ++low;
            --high;
        }
    }
    return high;
}

// perform quick sort on bid title
void quickSort(vector<Bid>&bids, int begin, int end) {
    int mid = 0;

    /* Base case: If there are 1 or zero bids to sort,
     partition is already sorted otherwise if begin is greater
     than or equal to end then return*/
    if (begin >= end) {
        return;
    }

    /* Partition bids into low and high such that
     midpoint is location of last element in low */
    mid = partition(bids, begin, end);
     
    // recursively sort low partition (begin to mid)
    quickSort(bids, begin, mid);

    // recursively sort high partition (mid+1 to end)
    quickSort(bids, mid + 1, end);

    return;
}

// perform a selection sort on bid title
void selectionSort(vector<Bid>& bids) {
    int smallest = 0;
    int largest = bids.size();

    for (unsigned int place = 0; place < largest; ++place) {
        smallest = place;
        for (unsigned j = place + 1; j < largest; ++j) {
            if (bids[j].title.compare(bids[smallest].title) < 0) {
                smallest = j;
            }
        }
        if (smallest != place) {
            swap(bids[place], bids[smallest]);
        }

    }
}

// C function to convert a string to a double after stripping out unwanted char
// credit: http://stackoverflow.com/a/24875936: 
double strToDouble(string str, char ch) {
    str.erase(remove(str.begin(), str.end(), ch), str.end());
    return atof(str.c_str());
}

int main(int argc, char* argv[]) {
    string csvPath;
    switch (argc) {
    case 2:
        csvPath = argv[1];
        break;
    default:
        csvPath = "eBid_Monthly_Sales.csv";
    }

    vector<Bid> bids;

    // Define a timer variable
    clock_t ticks;

    int choice = 0;
    while (choice != 9) {
        cout << "Menu:" << endl;
        cout << "  1. Load Bids" << endl;
        cout << "  2. Display All Bids" << endl;
        cout << "  3. Selection Sort All Bids" << endl;
        cout << "  4. Quick Sort All Bids" << endl;
        cout << "  9. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {

        case 1:
            ticks = clock();
            bids = loadBids(csvPath);
            cout << bids.size() << " bids read" << endl;

            // Calculate elapsed time and display result
            ticks = clock() - ticks; 
            cout << "time: " << ticks << " clock ticks" << endl;
            cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;
            break;
        case 2:
            // Loop and display the bids read
            for (int i = 0; i < bids.size(); ++i) {
                displayBid(bids[i]);
            }
            cout << endl;

            break;
        case 3: 
            ticks = clock();
            selectionSort(bids);

            cout << bids.size() << " bids sorted" << endl;
            ticks = clock() - ticks; 
            cout << "time: " << ticks << " clock ticks" << endl;
            cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;

            break;
        case 4: 
            ticks = clock();
            quickSort(bids, 0, bids.size() - 1);

            ticks = clock() - ticks; 
            cout << "time: " << ticks << " clock ticks" << endl;
            cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;

            break;
        }
    }
    cout << "Good bye." << endl;
    return 0;
}