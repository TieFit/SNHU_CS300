//============================================================================
// Name        : LinkedList.cpp
// Author      : JJ Newell
// Version     : 1.0
// Copyright   : Copyright © 2023,  portions Copyright © 2017 SNHU COCE
// Description : Lab 3-2 Lists and Searching
//============================================================================

#include "CSVparser.hpp"
#include "classes.hpp"

using namespace std;


// call default constructor and sets amount to 0
Bid::Bid() {
	amount = 0.0;
}

double Bid::getAmount() const {
	return amount;
}

void Bid::setAmount(double amount) {
	this->amount = amount;
}

const string& Bid::getBidId() const {
	return bidId;
}

void Bid::setBidId(const string& bidId) {
	this->bidId = bidId;
}

const string& Bid::getFund() const {
	return fund;
}

void Bid::setFund(const string& fund) {
	this->fund = fund;
}

const string& Bid::getTitle() const {
	return title;
}

void Bid::setTitle(const string& title) {
	this->title = title;
}

void Bid::display() {
	cout << this->getBidId() << ": " << this->getTitle() << " | " << this->getAmount()
		<< " | " << this->getFund() << endl;
}

/**
 * Default constructor
 */
LinkedList::LinkedList() {
	// FIXME (1): Initialize housekeeping variables
	//set head and tail equal to null
	head = nullptr;
	tail = nullptr;
}

/**
 * Destructor
 */
LinkedList::~LinkedList() {
	// start at the head
	Node* current = head;
	Node* temp;

	// loop over each node, detach from list then delete
	while (current != nullptr) {
		temp = current; // hang on to current node
		current = current->next; // make current the next node
		delete temp; // delete the orphan node
	}
}

/**
 * Append a new bid to the end of the list
 */
void LinkedList::Append(Bid bid) {
	// FIXME (2): Implement append logic

	// this conditional will check if the key entered already exists, acting as input validation to prevent duplicates
	if (this->keyExists(bid.getBidId())) {
		cout << "Duplicate key in BidId Enter another bid" << endl;
		return;
	}

	//Create new node
	Node* new_node = new Node(bid);

	//if there is nothing at the head...
	if (head == nullptr) {
		// new node becomes the head and the tail
		head = new_node;
		tail = new_node;
	}

	//else
	else {
		// make current tail node point to the new node
		tail->next = new_node;

		// and tail becomes the new node
		tail = new_node;
	}
	//increase size count
	++size;
}

/**
 * Prepend a new bid to the start of the list
 */
void LinkedList::Prepend(Bid bid) {
	// FIXME (3): Implement prepend logic
	// this conditional will check if the key entered already exists, acting as input validation to prevent duplicates
	if (this->keyExists(bid.getBidId())) {
		std::cout << "Duplicate key entered, please enter a different bid\n";
		return;
	}

	// Create new node
	Node* new_node = new Node(bid);

	// if there is already something at the head...
	if (head != nullptr) {
		// new node points to current head as its next node
		new_node->next = head;
	}

	// head now becomes the new node
	head = new_node;

	//increase size count
	++size;
}

/**
 * Simple output of all bids in the list
 */
void LinkedList::PrintList() {
	// FIXME (4): Implement print logic
	// start at the head
	Node* current = head;

	// while loop over each node looking for a match
	while (current != nullptr) {
		//output current bidID, title, amount and fund
		current->bid.display();

		//set current equal to next
		current = current->next;
	}
}

/**
 * Remove a specified bid
 *
 * @param bidId The bid id to remove from the list
 */
void LinkedList::Remove(std::string bidId) {
	// FIXME (5): Implement remove logic
	// special case if matching node is the head
	if (head->bid.getBidId() == bidId) {
		// make head point to the next node in the list
		head = head->next;

		//decrease size count
		--size;

		cout << bidId << "  removed." << endl;

		//return
		return;
	}

	// start at the head
	Node* current = head;

	// while loop over each node looking for a match
	while (current != nullptr) {

		// conditional to break loop if the node is null
		if (current->next == nullptr) {
			cout << bidId << "  not found." << endl;
			return;
		}

		// if the next node bidID is equal to the current bidID
		if (current->next->bid.getBidId() == bidId) {
			// hold onto the next node temporarily
			Node* temp = current->next;

			// make current node point beyond the next node
			current->next = temp->next;

			// now free up memory held by temp
			temp = nullptr;

			// decrease size count
			--size;

			// output message saying that the bid was removed
			cout << bidId << "  removed." << endl;

			//return
			return;
		}
		// current node is equal to next node
		current = current->next;
	}

}

/**
 * Search for the specified bidId
 *
 * @param bidId The bid id to search for
 */
Bid LinkedList::Search(std::string bidId) {
	// FIXME (6): Implement search logic
	if (head->bid.getBidId() == bidId) {

		// returns bid from the head node
		return head->bid;
	}

	// start at the head of the list
	Node* current = head;

	// keep searching until end reached with while loop (current != nullptr)
	while (current != nullptr) {
		// if the current node matches, return it
		if (current->bid.getBidId() == bidId) {
			return current->bid;
		}
		// else current node is equal to next node
		else {
			current = current->next;
		}
	}
	//(the next two statements will only execute if search item is not found)
	//create new empty bid
	Bid bid = Bid();

	//return empty bid
	return bid;
}

/**
 * Returns the current size (number of elements) in the list
 */
int LinkedList::Size() {
	return size;
}

// this function can be reused throughout various parts of the project. The purpose of this is input validation as it will prevent duplicates. It does so by checking the current
// bidId against pre-existing bidIds
bool LinkedList::keyExists(std::string key)
{
	Node* current = head;

	// for loops iterates through the list for as many times as there are nodes in the list
	for (int i = 0; i < this->Size(); i++) {

		// Checks if current node in the list is the same as what was entered by user
		if (current->bid.getBidId() == key) {
			// returns true if a match/duplicate is found
			return true;
		}
		current = current->next;
	}

	// returns false if no duplicates/matches were found
	return false;
}

/**
 * Prompt user for bid information
 *
 * @return Bid struct containing the bid info
 */
Bid BidHandler::getBid() {
	Bid bid;

	cout << "Enter Id: ";
	cin.ignore();
	string _bidId;
	getline(cin, _bidId);
	bid.setBidId(_bidId);

	cout << "Enter title: ";
	string _title;
	getline(cin, _title);
	bid.setTitle(_title);

	cout << "Enter fund: ";
	string _fund;
	getline(cin, _fund);
	bid.setFund(_fund);

	cout << "Enter amount: ";
	string strAmount;
	getline(cin, strAmount);
	bid.setAmount(strToDouble(strAmount, '$'));

	return bid;
}

/**
 * Load a CSV file containing bids into a LinkedList
 *
 * @return a LinkedList containing all the bids read
 */
void BidHandler::loadBids(std::string csvPath) {
	cout << "Loading CSV file " << csvPath << endl;

	// initialize the CSV Parser
	csv::Parser file = csv::Parser(csvPath);

	try {
		// loop to read rows of a CSV file
		for (int i = 0; i < file.rowCount(); i++) {

			// initialize a bid using data from current row (i)
			Bid bid;
			bid.setBidId(file[i][1]);
			bid.setTitle(file[i][0]);
			bid.setFund(file[i][8]);
			bid.setAmount(strToDouble(file[i][4], '$'));

			bid.display();

			// add this bid to the end
			bids.Append(bid);
		}
	}
	catch (csv::Error& e) {
		std::cerr << e.what() << std::endl;
	}
}

const LinkedList& BidHandler::getBids() const{
	return bids;
}

void BidHandler::setBids(const LinkedList& bids) {
	this->bids = bids;
}

double strToDouble(std::string str, char ch) {
	str.erase(remove(str.begin(), str.end(), ch), str.end());
	return atof(str.c_str());
};

/**
 * The one and only main() method
 *
 * @param arg[1] path to CSV file to load from (optional)
 * @param arg[2] the bid Id to use when searching the list (optional)
 */
int main(int argc, char* argv[]) {

	// process command line arguments
	string csvPath, bidKey;
	switch (argc) {
	case 2:
		csvPath = argv[1];
		bidKey = "98109";
		break;
	case 3:
		csvPath = argv[1];
		bidKey = argv[2];
		break;
	default:
		// Switched csv file to Dec as it is smaller and loads quicker for debug purposes
		csvPath = "eBid_Monthly_Sales_Dec_2016.csv";

		//csvPath = "eBid_Monthly_Sales.csv";
		bidKey = "98109";
	}

	clock_t ticks;

	BidHandler handler = BidHandler();
	LinkedList bidList;

	Bid bid;

	int choice = 0;
	while (choice != 9) {
		cout << "Menu:" << endl;
		cout << "  1. Enter a Bid" << endl;
		cout << "  2. Load Bids" << endl;
		cout << "  3. Display All Bids" << endl;
		cout << "  4. Find Bid" << endl;
		cout << "  5. Remove Bid" << endl;
		cout << "  9. Exit" << endl;
		cout << "Enter choice: ";
		cin >> choice;

		switch (choice) {
		case 1:
			// get current bid using handler
			bid = handler.getBid();
			cout << "Bid created: ";

			// display details of current bid
			bid.display();

			// retrieve list of existing bids
			bidList = handler.getBids();

			// append new bid to this list
			bidList.Append(bid);

			// update handler with the updated list
			handler.setBids(bidList);


			break;

		case 2:
			ticks = clock();

			// loads bids from the csv file
			handler.loadBids(csvPath);

			// retrieve list of bids that was loaded into the handler
			bidList = handler.getBids();

			// output number bids that were read from csv file
			cout << bidList.Size() << " bids read" << endl;

			ticks = clock() - ticks; // current clock ticks minus starting clock ticks
			cout << "time: " << ticks << " milliseconds" << endl;
			cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;

			break;

		case 3:
			// retrieve list of bids that was loaded into the handler
			bidList = handler.getBids();

			// pass bidList variable into the PrintList function to display all bids
			bidList.PrintList();

			break;

		case 4:
			ticks = clock();

			// retrieve list of bids that was loaded into the handler
			bidList = handler.getBids();

			// Search for a specific bid in the list using the provided bidKey
			bid = bidList.Search(bidKey);

			ticks = clock() - ticks; // current clock ticks minus starting clock ticks

			// if a bid matching what was entered is found, display the bid
			if (!bid.getBidId().empty()) {
				bid.display();
			}

			// else, the bid was not found
			else {
				cout << "Bid Id " << bidKey << " not found." << endl;
			}

			cout << "time: " << ticks << " clock ticks" << endl;
			cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;

			break;

		case 5:
			// retrieve list of bids that was loaded into the handler
			bidList = handler.getBids();

			// remove the bid that was entered through use of the bidKey argument
			bidList.Remove(bidKey);

			// update the list to reflect that this bid has been removed
			handler.setBids(bidList);

			break;
		}
	}
	cout << "Good bye." << endl;
	
	return 0;
}