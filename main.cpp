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
	bool outerReq[FLOOR_NUM + 1] = { false, false, false, false, false, false, false, false };
	bool toFloor[FLOOR_NUM + 1] = { false, false, false, false, false, false, false, false };
};

Elevator initializeElevator(int);

Request initRequest();

void delegateRequest(Elevator*, Elevator*, Request);

void rideElevators();

int main(int argc, char** argv) {
	cout << "Init building successfully!" << endl;
	cout << "Building's number of floor is: " << MAX_PASS << endl;
	cout << "Building's number of elevators is: " << ELEVATOR_NUM << endl;
	cout << "Building's elevator's number of maximum passengers is: " << FLOOR_NUM << endl;

	Elevator e1 = initializeElevator(1);
	Elevator e2 = initializeElevator(2);

	e1.currentFloor = 5; // for test

	bool working = true;
	while (working) {
		int isReq;
		cout << "\nGive me a request (-1 to exit, 1 to request, 2 to start riding): ";
		cin >> isReq;
		// TODO check correct isReq
		if (isReq == -1) working = false;
		else if (isReq == 1) {
			Request req = initRequest();
			delegateRequest(&e1, &e2, req);

			for (int i = 0; i < 8; i++) {
				cout << e1.outerReq[i] << " ";
			}
			cout << endl;
			for (int i = 0; i < 8; i++) {
				cout << e2.outerReq[i] << " ";
			}
			cout << endl;
		}
		else if (isReq == 2) {
			rideElevators();
		}
		else cout << "Wrong input for request!!!" << endl;
	}

	cout << "\nThank you for using Bear Elevators. Have a good night <3" << endl;
	return 0;
}

Elevator initializeElevator(int id) {
	Elevator e;
	e.id = id;
	e.currentFloor = 0; // Ground floor
	e.currentPasser = 0;
	e.direction = IDLE;
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
void delegateRequest(Elevator *e1, Elevator *e2, Request r) {
	// Case: Both elevators are idle
	if (e1->direction == e2->direction && e1->direction == IDLE) {
		if (abs(e1->currentFloor - r.fromFloor) < abs(e2->currentFloor - r.fromFloor))
			e1->outerReq[r.fromFloor] = true;
		else e2->outerReq[r.fromFloor] = true;
	}
	// Case: Both elevators are going in the same direction as request
	else if (e1->direction == e2->direction && e1->direction == r.direction) {
		if (e1->direction == DOWN) {
			if (e1->currentFloor > r.fromFloor && e2->currentFloor > r.fromFloor) {
				if (e1->currentFloor < e2->currentFloor)
					e1->outerReq[r.fromFloor] = true;
				else e2->outerReq[r.fromFloor] = true;
			} 
			else if (e1->currentFloor < r.fromFloor)
				e2->outerReq[r.fromFloor] = true;
			else e1->outerReq[r.fromFloor] = true;
		}
		else if (e1->direction == UP) {
			if (e1->currentFloor < r.fromFloor && e2->currentFloor < r.fromFloor) {
				if (e1->currentFloor > e2->currentFloor)
					e1->outerReq[r.fromFloor] = true;
				else e2->outerReq[r.fromFloor] = true;
			}
			else if (e1->currentFloor > r.fromFloor)
				e2->outerReq[r.fromFloor] = true;
			else e1->outerReq[r.fromFloor] = true;
		}
	}
	// Case: Two elevators has different directions
	else if (e1->direction != e2->direction && e1->direction != r.direction) {
		e2->outerReq[r.fromFloor] = true;
	} else e1->outerReq[r.fromFloor] = true;
}

void rideElevators()
{
}

