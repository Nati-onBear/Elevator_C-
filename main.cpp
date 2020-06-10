#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

#define MAX_PASS 10
#define FLOOR_NUM 7
#define ELEVATOR_NUM 2

using namespace std;

enum class Direction {
	IDLE, // 0
	DOWN, // 1
	UP    // 2
};

enum class Place {
	OUTER, // 0
	INNER  // 1
};

struct Request {
	int floor;
	Direction direction;
	Place place;

	string toString() { 
		string dir = (direction == Direction::UP) ? "UP" : ((direction == Direction::DOWN) ? "DOWN" : "IDLE");
		string pla = (place == Place::OUTER) ? "OUTER" : "INNER";
		return "Floor: " + to_string(floor) + ", Direction: " + dir + ", Place: " + pla + "\n";
	}

	/*bool operator<(const Request& a) const
	{
		return direction == Direction::UP ? floor < a.floor : floor > a.floor;
	}*/
};

struct Elevator {
	int id;
	int currentFloor = 0;
	int currentPasser = 0;
	enum::Direction direction = Direction::IDLE;
	vector<int> outerReq = {};
	vector<int> innerReq = {};

	int getCurrentFloor() { return currentFloor; }
	int getCurrentPasser() { return currentPasser; }
	Direction getCurrentDirection() { return direction; }

	void printRequests() {
		cout << "Requests of elevator " << id << ": ";
		vector<int>::iterator it;
		for (it = outerReq.begin(); it != outerReq.end(); it++)
			cout << to_string(*it);
		cout << endl;
	}

	void printInfo() {
		string dir = (direction == Direction::UP) ? "UP" : ((direction == Direction::DOWN) ? "DOWN" : "IDLE");
		cout << "================================================\n";
		cout << "Elevator #" << id << endl;
		cout << "Elevator is currently at floor " << currentFloor <<endl;
		cout << "Elevator's current number of people is " << currentPasser << endl;
		cout << "Elevator's current direction is " << dir << endl;
		cout << "================================================\n";
	}

	// Calculate score for elevator
	int scoreValidOuterReq(int floor) {
		// Max capacity reached, stop taking requests
		if (currentPasser == MAX_PASS) return -1;
		// Idle elevator: best score is 0
		else if (direction == Direction::IDLE) return abs(currentFloor - floor);
		// Moving elevator: best score is 1 
		else if (
			(direction == Direction::UP && floor > currentFloor + 1)
			||
			(direction == Direction::DOWN && floor < currentFloor - 1)
		) return abs(currentFloor - floor);
		else return -1;
	}

	bool isValidInnerReq(int floor) {
		if (
			(direction == Direction::UP && floor > currentFloor)
			|| 
			(direction == Direction::DOWN && floor < currentFloor)
		) return true;
		return false;
	}
};

struct WaitingArea {
	vector<Elevator*> elevators;
	int maxPassenger = MAX_PASS;
	int numFloor = FLOOR_NUM;
	int numElevator = ELEVATOR_NUM; // = elevators.size()
	vector<Request> queueRequests = {};
};

void operateElevators();

Request getOuterRequest();

int getInnerRequest();

void delegateRequest(Request r);

void rideElevators(Elevator* e);

void exit();

// Init global data
Elevator e1 = { 1 };
Elevator e2 = { 2 };
vector<Elevator *> elevators{ &e1, &e2 };
WaitingArea waitArea = { elevators };

int main(int argc, char** argv) {
	cout << "Building's number of floor is: " << waitArea.numFloor << endl;
	cout << "A waiting area's number of elevators is: " << waitArea.numElevator << endl;
	cout << "Elevator's number of maximum passengers is: " << waitArea.maxPassenger << endl;

	e1.currentFloor = 5; // for test

	e1.printInfo();
	e2.printInfo();
	
	operateElevators();
	exit();
	return 0;
}

void operateElevators() {
	bool working = true;
	while (working) {
		int isReq;
		cout << "\nGive me a request (-1 to exit, 1 to request, 2 to start riding): ";
		cin >> isReq;
		// TODO check correct isReq

		if (isReq == -1) working = false;
		else if (isReq == 1) {
			Request req = getOuterRequest();
			delegateRequest(req);

			// for test
			e1.printRequests();
			e2.printRequests();
		}
		else if (isReq == 2) {
			//rideElevators(&e1);
			rideElevators(&e2);
		}
		else cout << "Wrong input for request!!!" << endl;
	}
}

Request getOuterRequest() {
	int atFloor;
	int direction;
	Request req;

	cout << "At which floor are you? ";
	cin >> atFloor;
	// TODO check condition for atFloor

	cout << "Which direction do you go? (1 for down, 2 for up) ";
	cin >> direction;
	while (direction != 1 && direction != 2) {
		cout << "Wrong input! Which direction do you go? (1 for down, 2 for up) ";
		cin >> direction;
	}
	req.floor = atFloor;

	if (direction == 2) // up
		req.direction = Direction::UP;
	else req.direction = Direction::DOWN;

	req.place = Place::OUTER;

	return req;
}

int getInnerRequest() {
	int toFloor;

	cout << "To which floor do you want? ";
	cin >> toFloor;
	// TODO check condition for toFloor

	return toFloor;
}

// Delegate request to one of elevators 
void delegateRequest(Request r) {
	// Lambda function for delegate request and increment passer
	auto deleReq = [](Elevator *e, Request r) {
		e->outerReq.push_back(r.floor);
		if (e->direction == Direction::IDLE) e->direction = r.direction;
	};

	int e1Score = e1.scoreValidOuterReq(r.floor);
	int e2Score = e2.scoreValidOuterReq(r.floor);

	if (e1Score == -1) {
		if (e2Score == -1) {
			//cout << "Both elevators are full!\nOr in different direction. Putting request in queue..." << endl;
			cout << "Both elevators are full!\nOr in different direction. Required input again after riding..." << endl;
			
			// TODO implement queueRequests
			//waitArea.queueRequests.push_back(r);
			return;
		}
		else deleReq(&e2, r);
	}
	// Else e1Score != -1 && e2Score == -1
	else if (e2Score == -1) deleReq(&e1, r);
	// Smallest score wins!
	else e1Score < e2Score ? deleReq(&e1, r) : deleReq(&e2, r);
	/*
	// TODO: check for direction...
	// Case: Full for both elevators
	// TODO implement these as isValidReq
	if (e1.currentPasser == MAX_PASS && e2.currentPasser == MAX_PASS) {
		//cout << "Both elevators are full!\nOr in different direction. Putting request in queue..." << endl;
		cout << "Both elevators are full!\nOr in different direction. Required input again after riding..." << endl;

		waitArea.queueRequests.push_back(r);
		// TODO implement queueRequests
		return;
	}
	// Case: Full for one elevator
	else if (e1.currentPasser < MAX_PASS && e2.currentPasser == MAX_PASS) deleReq(&e1, r);
	else if (e1.currentPasser == MAX_PASS && e2.currentPasser > MAX_PASS) deleReq(&e2, r);
	// Case: Both elevators are idle
	else if (e1.direction == e2.direction && e1.direction == IDLE) {
		if (abs(e1.currentFloor - r.floor) < abs(e2.currentFloor - r.floor))
			deleReq(&e1, r);
		else deleReq(&e2, r);
	}
	// Case: Both elevators are going in the same direction as request
	else if (e1.direction == e2.direction && e1.direction == r.direction) {
		if (e1.direction == DOWN) {
			if (e1.currentFloor > r.floor && e2.currentFloor > r.floor) {
				if (e1.currentFloor < e2.currentFloor)
					deleReq(&e1, r);
				else deleReq(&e2, r);
			} 
			else if (e1.currentFloor < r.floor)
				deleReq(&e2, r);
			else deleReq(&e1, r);
		}
		else if (e1.direction == UP) {
			if (e1.currentFloor < r.floor && e2.currentFloor < r.floor) {
				if (e1.currentFloor > e2.currentFloor)
					deleReq(&e1, r);
				else deleReq(&e2, r);
			}
			else if (e1.currentFloor > r.floor)
				deleReq(&e2, r);
			else deleReq(&e1, r);
		}
	}
	// Case: Two elevators has different directions
	else if (e1.direction != e2.direction && e1.direction != r.direction) {
		deleReq(&e2, r);
	} else deleReq(&e1, r);
	*/
}

bool sortByDirection (const Request& l, const Request& r) {
	return l.direction == Direction::UP ? l.floor < r.floor : l.floor > r.floor;
}

void rideElevators(Elevator* pE) {
	vector<int> outerReq = pE->outerReq;
	vector<int> innerReq = pE->innerReq;
	
	sort(outerReq.begin(), outerReq.end());
	sort(innerReq.begin(), innerReq.end());

	cout << "For elevator " << pE->id 
		<< ", the current floor is " << pE->currentFloor 
		<< ", number of people in it is " << pE->currentPasser << endl;

	while (!outerReq.empty() || !innerReq.empty()) {
		bool isFromOuter = false;
		bool isUp = pE->direction == Direction::UP;
		if (isUp) {
			isFromOuter = outerReq[0] < innerReq[0];
			pE->currentFloor = isFromOuter ? outerReq[0] : innerReq[0];
		}
		else if (pE->direction == Direction::DOWN) {
			isFromOuter = outerReq[outerReq.size() - 1] < innerReq[innerReq.size() - 1];
			pE->currentFloor = isFromOuter ? outerReq[outerReq.size() - 1] : innerReq[innerReq.size() - 1];
		}

		int currFloor = pE->currentFloor;
		int currPass = pE->currentPasser;

		auto currReq = isFromOuter ? find(outerReq.begin(), outerReq.end(), pE->currentFloor) 
			: find(innerReq.begin(), innerReq.end(), pE->currentFloor);
		while (currReq != outerReq.end()) {
			// Increment for passenger based on where the request is from
			(isFromOuter)
				? currPass++
				: currPass--;
			// Removing current requests
			(isFromOuter) 
				? outerReq.erase(currReq)
				: innerReq.erase(currReq);
			isUp 
				? isFromOuter = outerReq[0] < innerReq[0] 
				: isFromOuter = outerReq[outerReq.size() - 1] < innerReq[innerReq.size() - 1];

			currReq = isFromOuter 
				? find(outerReq.begin(), outerReq.end(), pE->currentFloor)
				: find(innerReq.begin(), innerReq.end(), pE->currentFloor);
		}

		pE->currentPasser = currPass;

		pE->printInfo();

		/*if (currFloor == FLOOR_NUM) pE->direction = DOWN;
		else if (currFloor == 1) pE->direction = UP;*/

		// Input inner requests
		if (currPass < MAX_PASS) {
			int toFloor = getInnerRequest();
			Request nextReq = Request{ toFloor, pE->direction, Place::INNER };
			if (pE->isValidInnerReq(nextReq.floor))
				pE->innerReq.push_back(nextReq.floor);

			// Problem: can't reach e2, need concurrent function
			// Can hard code this...
		}

		// sort here or in delegate
		sort(outerReq.begin(), outerReq.end());
		sort(innerReq.begin(), innerReq.end());
	}

	pE->outerReq = outerReq;
	pE->innerReq = innerReq;
	pE->direction = Direction::IDLE;
}

void exit()
{
	e1 = { 1 }; e2 = { 2 };
	e1.printInfo();
	e2.printInfo();
	cout << "\nThank you for using Bear Elevators. Have a good night <3" << endl;
}
