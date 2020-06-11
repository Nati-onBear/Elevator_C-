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
		if (currentPasser >= MAX_PASS) return -1;
		// Idle elevator: best score is 0 (request at stay by floor)
		else if (direction == Direction::IDLE) return abs(currentFloor - floor);
		// Moving elevator: best score is 1 (for same direction but differ by 2 floors)
		else if (direction == Direction::UP && floor > currentFloor + 1) return floor - currentFloor - 1;
		else if (direction == Direction::DOWN && floor < currentFloor - 1) return currentFloor - floor - 1;
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

	/*e1.printInfo();
	e2.printInfo();*/
	
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

		if (isReq == -1) working = false;
		else if (isReq == 1) {
			Request req = getOuterRequest();
			delegateRequest(req);

			// for test
			e1.printRequests();
			e2.printRequests();
		}
		else if (isReq == 2) {
			// TODO implement concurrent here
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
	return toFloor;
}

// Delegate request to one of elevators 
void delegateRequest(Request r) {
	// Lambda function for delegate request and increment passer
	auto deleReq = [](Elevator *pE, Request r) {
		pE->outerReq.insert(r);
		pE->currentPasser++;
		//if (pE->direction == Direction::IDLE) pE->direction = r.direction;
	};

	int e1Score = e1.scoreValidOuterReq(r.floor);
	int e2Score = e2.scoreValidOuterReq(r.floor);
	//cout << to_string(e1Score) << " & " << to_string(e2Score) << endl;

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

void rideElevators(Elevator* pE) {
	vector<int> requests = {};
	
	for (set<Request>::iterator it = pE->outerReq.begin(); it != pE->outerReq.end(); ++it) {
		requests.push_back((*it).floor);
	}
	for (set<Request>::iterator it = pE->innerReq.begin(); it != pE->innerReq.end(); ++it) {
		requests.push_back((*it).floor);
	}

	if (pE->direction == Direction::IDLE) pE->direction = (*(pE->outerReq.begin())).direction;
	
	int counter = requests.size();

	cout << to_string(counter);

	while (!requests.empty()) {
		bool isUp = pE->direction == Direction::UP;

		pE->currentFloor = requests.empty() ? pE->currentFloor : requests[0];
		
		vector<int>::iterator currReq = find(requests.begin(), requests.end(), pE->currentFloor);

		while (currReq != requests.end()) {
			//currPass++;
			// Removing current requests
			requests.erase(currReq);
			currReq = find(requests.begin(), requests.end(), pE->currentFloor);
			pE->printInfo();
		}
		
		if (counter <= 0) break;

		// Input inner requests
		int toFloor = getInnerRequest();
		Request nextReq = Request{ toFloor, pE->direction, Place::INNER };
		// Delegete outer request
		if (pE->isValidInnerReq(nextReq.floor)) {
			pE->innerReq.insert(nextReq);
			counter--;
		}

		for (set<Request>::iterator it = pE->innerReq.begin(); it != pE->innerReq.end(); ++it)
			requests.push_back((*it).floor);
	}

	pE->outerReq = {};
	pE->innerReq = {};
	pE->direction = Direction::IDLE;
	pE->currentPasser = 0;
	cout << "Elevator " << pE->id << "has finished all request. Status: IDLE!" << endl;
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
