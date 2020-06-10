#include <iostream>
#include <vector>
#include <set>
#include <algorithm> // for std::sort
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

	string toString() const { 
		string dir = (direction == Direction::UP) ? "UP" : ((direction == Direction::DOWN) ? "DOWN" : "IDLE");
		string pla = (place == Place::OUTER) ? "OUTER" : "INNER";
		return "\n\tFloor: " + to_string(floor) + ", Direction: " + dir + ", Place: " + pla;
	}

	bool operator<(const Request & a) const
	{
		return (direction == Direction::UP && direction == a.direction) ? floor < a.floor : floor > a.floor;
	}
};

struct Elevator {
	int id;
	int currentFloor = 0;
	int currentPasser = 0;
	enum::Direction direction = Direction::IDLE;
	set<Request> outerReq = {};
	set<Request> innerReq = {};

	int getCurrentFloor() { return currentFloor; }
	int getCurrentPasser() { return currentPasser; }
	Direction getCurrentDirection() { return direction; }

	void printRequests() {
		cout << "Requests of elevator " << id << ": ";
		set<Request>::iterator it;
		for (it = outerReq.begin(); it != outerReq.end(); it++)
			cout << (*it).toString();
		cout << endl;
	}

	void printInfo() {
		string dir = (direction == Direction::UP) ? "UP" : ((direction == Direction::DOWN) ? "DOWN" : "IDLE");
		cout << "________________________________________________\n";
		cout << "Elevator #" << id << endl;
		cout << "Elevator is currently at floor " << currentFloor <<endl;
		cout << "Elevator's current est number of people is " << currentPasser << endl;
		cout << "Elevator's current direction is " << dir << endl;
		cout << "________________________________________________\n";
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

	e1.currentFloor = 7; // for test

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
		e->outerReq.insert(r);
		e->currentPasser++;
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
}

//bool sortByDirection (const Request& l, const Request& r) {
//	return l.direction == Direction::UP ? l.floor < r.floor : l.floor > r.floor;
//}

void rideElevators(Elevator* pE) {
	vector<int> outerReq = {};
	vector<int> innerReq = {};
	for (set<Request>::iterator it = pE->outerReq.begin(); it != pE->outerReq.end(); ++it) {
		outerReq.push_back((*it).floor);
	}
	for (set<Request>::iterator it = pE->innerReq.begin(); it != pE->innerReq.end(); ++it) {
		innerReq.push_back((*it).floor);
	}
	
	/*sort(outerReq.begin(), outerReq.end());
	sort(innerReq.begin(), innerReq.end());*/

	//pE->printInfo();

	while (!outerReq.empty() || !innerReq.empty()) {
		bool isFromOuter = outerReq.empty() ? false : (innerReq.empty() ? true : NULL);
		bool isUp = pE->direction == Direction::UP;

		//isFromOuter = outerReq[0] < innerReq[0];
		pE->currentFloor = isFromOuter ? outerReq[0] : innerReq[0];
		
		int currFloor = pE->currentFloor;
		int currPass = pE->currentPasser;

		vector<int>::iterator currReq = isFromOuter ? find(outerReq.begin(), outerReq.end(), pE->currentFloor) 
			: find(innerReq.begin(), innerReq.end(), pE->currentFloor);

		while (currReq != outerReq.end() /*|| currReq != innerReq.end()*/) {
			if (isFromOuter) {
				while (currReq != outerReq.end()) {
					//currPass++;
					// Removing current requests
					outerReq.erase(currReq);

					isFromOuter = outerReq.empty();
					currReq = isFromOuter
						? find(outerReq.begin(), outerReq.end(), pE->currentFloor)
						: find(innerReq.begin(), innerReq.end(), pE->currentFloor);
				}
			}
			//else if (!isFromOuter) {
			//	while (currReq != innerReq.end()) {
			//		// Increment for passenger based on where the request is from
			//		currPass--;
			//		innerReq.erase(currReq);
			//		isFromOuter = !innerReq.empty();
			//		currReq = isFromOuter
			//			? find(outerReq.begin(), outerReq.end(), pE->currentFloor)
			//			: find(innerReq.begin(), innerReq.end(), pE->currentFloor);
			//	}
			//}
		}
		
		pE->currentPasser = currPass;

		//pE->printInfo();

		/*if (currFloor == FLOOR_NUM) pE->direction = DOWN;
		else if (currFloor == 1) pE->direction = UP;*/

		// Input inner requests
		if (currPass < MAX_PASS) {
			int toFloor = getInnerRequest();
			Request nextReq = Request{ toFloor, pE->direction, Place::INNER };
			// Delegete outer request
			if (pE->isValidInnerReq(nextReq.floor))
				pE->innerReq.insert(nextReq);

			// Problem: can't reach e2, need concurrent function
			// Can hard code this...
		}

		// sort here or in delegate
		/*sort(outerReq.begin(), outerReq.end());
		sort(innerReq.begin(), innerReq.end());*/
		/*for (set<Request>::iterator it = pE->outerReq.begin(); it != pE->outerReq.end(); ++it) {
			outerReq.push_back((*it).floor);
		}
		for (set<Request>::iterator it = pE->innerReq.begin(); it != pE->innerReq.end(); ++it) {
			innerReq.push_back((*it).floor);
		}*/
	}

	pE->outerReq = {};
	pE->innerReq = {};
	pE->direction = Direction::IDLE;
}

void exit()
{
	waitArea.elevators.clear();
	e1 = { 1 }; e2 = { 2 };
	waitArea.elevators.push_back(&e1);
	waitArea.elevators.push_back(&e2);
	e1.printInfo();
	e2.printInfo();
	cout << "\nThank you for using Bear Elevators. Have a good night <3" << endl;
}
