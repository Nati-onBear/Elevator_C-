#include <iostream>
#include <set>
#define MAX_PASS 10
#define FLOOR_NUM 7
#define ELEVATOR_NUM 2

using namespace std;

enum direction {
	IDLE, // 0
	DOWN, // 1
	UP    // 2
};

struct Request {
	int fromFloor;
	direction direction;

	// compare for order
	bool operator <(const Request& req) const {
		if (direction == 2) // 2 is up
			return fromFloor < req.fromFloor;
		return fromFloor > req.fromFloor;
	}
};

struct Elevator {
	int id;
	int currentFloor;
	int currentPasser;
	direction direction;
	set<int> outerReq;
	set<int> toFloor;
};

Elevator initializeElevator(int);

Request initRequest();

void delegateRequest(Elevator, Elevator, Request);

int main(int argc, char** argv) {
	cout << "Init building successfully!";
	cout << "\nBuilding's number of floor is: " << MAX_PASS;
	cout << "\nBuilding's number of elevators is: " << ELEVATOR_NUM;
	cout << "\nBuilding's elevator's number of maximum passengers is: " << FLOOR_NUM;

	Elevator e1 = initializeElevator(1);
	Elevator e2 = initializeElevator(2);

	e1.currentFloor = 5;

	bool working = true;
	while (working) {
		int isReq;
		cout << "\nGive me a request (-1 to exit, 1 to request): ";
		cin >> isReq;

		if (isReq == -1) working = false;
		else {
			Request req = initRequest();
			delegateRequest(e1, e2, req);
		}
	}

	cout << "\nThank you for using Bear Elevators. Have a good night <3";
	return 0;
}

Elevator initializeElevator(int id) {
	Elevator e;
	e.id = id;
	e.currentFloor = 0; // Ground floor
	e.currentPasser = 0;
	e.direction = IDLE;
	//e.outerReq = set<Request>;
	//e.toFloor = set<int>;
	return e;
}

Request initRequest() {
	int atFloor;
	int direction;
	Request req;

	cout << "\nAt which floor are you? ";
	cin >> atFloor;
	// TODO check condition for atFloor

	cout << "\nWhich direction do you go? (1 for down, 2 for up) ";
	cin >> direction;
	while (direction != 1 && direction != 2) {
		cout << "\nWrong input! Which direction do you go? (1 for down, 2 for up) ";
		cin >> direction;
	}
	req.fromFloor = atFloor;
	if (direction == 2) // up
		req.direction = UP;
	else req.direction = DOWN;

	return req;
}

// Delegate request to one of elevators 
void delegateRequest(Elevator e1, Elevator e2, Request r) {
	if (e1.direction == r.direction || e1.direction == IDLE) {
		e1.outerReq.insert(r.fromFloor);
	}
}
