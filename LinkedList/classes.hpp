#ifndef _CLASSES_HPP_
#define _CLASSES_HPP_

#include <iostream>
using namespace std;


// This file contains all classes that exist in the project. pre-existing classes from template code were moved here. Also new classes that were created are based from here. 

class Bid {
public:
	Bid();
	double getAmount() const;
	void setAmount(double amount);
	const string& getBidId() const;
	void setBidId(const string& bidId);
	const string& getFund() const;
	void setFund(const string& fund);
	const string& getTitle() const;
	void setTitle(const string& title);

	void display();

private:
	string bidId; // unique identifier
	string title;
	string fund;
	double amount;
};

// BidHandler class
class BidHandler {
public:
	
	Bid getBid();
	void loadBids(string csvPath);
	const LinkedList& getBids() const;
	void setBids(const LinkedList& bids);

private:
	LinkedList bids;
};

// LinkedList class
class LinkedList {
public:
	LinkedList();
	virtual ~LinkedList();
	void Append(Bid bid);
	void Prepend(Bid bid);
	void PrintList();
	void Remove(std::string bidId);
	Bid Search(std::string bidId);
	int Size();

private:
	//Internal structure for list entries, housekeeping variables
	struct Node {
		Bid bid;
		Node* next;

		// default constructor
		Node() {
			next = nullptr;
		}

		// initialize with a bid
		Node(Bid aBid) {
			bid = aBid;
			next = nullptr;
		}
	};

	Node* head;
	Node* tail;
	int size = 0;

	bool keyExists(string key);
};

#endif /*!__CLASSES_HPP_*/