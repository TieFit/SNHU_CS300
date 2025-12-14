//============================================================================
// Name        : LinkedList.cpp
// Author      : JJ Newell
// Version     : 1.0
// Copyright   : Copyright � 2023,  portions Copyright � 2017 SNHU COCE
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

// constructor
LinkedList::LinkedList() {
	head = nullptr;
	tail = nullptr;
}

// destructor
LinkedList::~LinkedList() {
	Node* current = head;
	Node* temp;

	// loop over each node, detach from list then delete
	while (current != nullptr) {
		temp = current; // hang onto current node
		current = current->next; // make current the next node
		delete temp; // delete the orphan node
	}
}

// append bid to end of list (const-ref)
void LinkedList::Append(const Bid& bid) {

	// this conditional will check if the key entered already exists, acting as input validation to prevent duplicates
	if (this->keyExists(bid.getBidId())) {
		cout << "Duplicate key in BidId Enter another bid" << endl;
		return;
	}

	// create new node
	Node* new_node = new Node(bid);
	
	// new node becomes the head and the tail
	if (head == nullptr) {
		head = new_node;
		tail = new_node;
	}

	else {
		// make current tail node point to the new node
		tail->next = new_node;

		// and tail becomes the new node
		tail = new_node;
	}
	++size;
}

// prepend bid to start of list
void LinkedList::Prepend(Bid bid) {
	// this conditional will check if the key entered already exists, acting as input validation to prevent duplicates
	if (this->keyExists(bid.getBidId())) {
		std::cout << "Duplicate key entered, please enter a different bid\n";
		return;
	}

	// Create new node
	Node* new_node = new Node(bid);

	// new node points to current head as its next node
	if (head != nullptr) {
		new_node->next = head;
	}
	head = new_node;
	++size;
}

// print all bids in the list
void LinkedList::PrintList() {
	// start at the head
	Node* current = head;

	while (current != nullptr) {
		// output current bidID, title, amount and fund
		current->bid.display();
		current = current->next;
	}
}

// remove a bid
void LinkedList::Remove(std::string bidId) {

	// special case if matching node is the head
	if (head->bid.getBidId() == bidId) {
		head = head->next;
		--size;
		cout << bidId << "  removed." << endl;
		return;
	}

	// start at the head
	Node* current = head;
	while (current->next != nullptr) {
		if (current->next->bid.getBidId() == bidId) {
			// hold onto the next node temporarily
			Node* temp = current->next;

			// make current node point beyond the next node
			current->next = temp->next;

			// now free up memory held by temp
			temp = nullptr;

			--size;
			cout << bidId << "  removed." << endl;
			return;
		}
		current = current->next;
	}
}

// search for bid
Bid LinkedList::Search(std::string bidId) {
	if (head->bid.getBidId() == bidId) {
		return head->bid;
	}

	// start at the head
	Node* current = head;

	// iterate over list to find the specified bid
	while (current != nullptr) {
		if (current->bid.getBidId() == bidId) {
			return current->bid;
		}
		else {
			current = current->next;
		}
	}
	// executes if search item is not found
	Bid bid = Bid();
	return bid;
}

// returns size of a list
int LinkedList::Size() {
	return size;
}

// used for checking if a key exists, helps to cut down on redundancy throughout the file
bool LinkedList::keyExists(std::string key)
{
	Node* current = head;

	// iterate over entire list
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

// get bid information from user
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

// load bids from csv
void BidHandler::loadBids(std::string csvPath) {
	cout << "Loading CSV file " << csvPath << endl;

	// initialize the CSV Parser
	csv::Parser file = csv::Parser(csvPath);

	try {
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
			bid = handler.getBid();
			cout << "Bid created: ";

			bid.display();
			bidList = handler.getBids();
			bidList.Append(bid);

			// update handler with the updated list
			handler.setBids(bidList);
			break;
		case 2:
			ticks = clock();
			handler.loadBids(csvPath);

			// retrieve list of bids that was loaded into the handler
			bidList = handler.getBids();

			cout << bidList.Size() << " bids read" << endl;

			ticks = clock() - ticks;
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
			bid = bidList.Search(bidKey);
			ticks = clock() - ticks; 

			// if a bid matching what was entered is found, display the bid
			if (!bid.getBidId().empty()) {
				bid.display();
			}
			else {
				cout << "Bid Id " << bidKey << " not found." << endl;
			}

			cout << "time: " << ticks << " clock ticks" << endl;
			cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;
			break;
		case 5:
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