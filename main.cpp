#include <iostream>
#include <vector>
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
};

struct Elevator {
	int id;
	int currentFloor = 0;
	int currentPasser = 0;
	enum::direction direction = IDLE;
	vector<int> outerReq;
	vector<int> innerReq;

	int getCurrentFloor() { return currentFloor; }
	int getCurrentPasser() { return currentPasser; }
	enum::direction getCurrentDirection() { return direction; }

	void printOuterReq() {
		cout << "Outer req of e" << id << ": " << endl;
		vector<int>::iterator it;
		for (it = outerReq.begin(); it != outerReq.end(); it++)
			cout << *it << (it == outerReq.end() ? NULL : ", ");
		cout << endl;
	}

	void printInnerReq() {
		cout << "Inner req of e" << id << ": " << endl;
		vector<int>::iterator it;
		for (it = innerReq.begin(); it != innerReq.end(); it++)
			cout << *it << (it == outerReq.end() ? NULL : ", ");
		cout << endl;
	}
};

Request getRequest();

bool isValidReq(int floor);

void delegateRequest(Request r);

void rideElevators();

void exit();

// Init data
Elevator e1 = { 1 };
Elevator e2 = { 2 };

int main(int argc, char** argv) {
	cout << "Init building successfully!" << endl;
	cout << "Building's number of floor is: " << MAX_PASS << endl;
	cout << "Building's number of elevators is: " << ELEVATOR_NUM << endl;
	cout << "Building's elevator's number of maximum passengers is: " << FLOOR_NUM << endl;

	e1.currentFloor = 5; // for test

	bool working = true;
	while (working) {
		int isReq;
		cout << "\nGive me a request (-1 to exit, 1 to request, 2 to start riding): ";
		cin >> isReq;
		// TODO check correct isReq

		if (isReq == -1) working = false;
		else if (isReq == 1) {
			Request req = getRequest();
			delegateRequest(req);

			// for test
			e1.printOuterReq();
			e2.printOuterReq();
		}
		else if (isReq == 2) {
			rideElevators();
		}
		else cout << "Wrong input for request!!!" << endl;
	}
	exit();
	return 0;
}

Request getRequest() {
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

bool isValidReq(int floor)
{
	return false;
}

// Delegate request to one of elevators 
void delegateRequest(Request r) {
	// Lambda function for delegate request and increment passer
	auto deleReq = [](Elevator *e, Request r) {
		e->outerReq.push_back(r.fromFloor);
		e->currentPasser++;
		if (e->direction == IDLE) e->direction = r.direction;
	};

	// Case: Full for both elevators
	if (e1.currentPasser == MAX_PASS && e2.currentPasser == MAX_PASS) {
		cout << "Both elevators are full!" << endl;
		return;
	}
	// Case: Full for one elevator
	else if (e1.currentPasser < MAX_PASS && e2.currentPasser == MAX_PASS) deleReq(&e1, r);
	else if (e1.currentPasser == MAX_PASS && e2.currentPasser > MAX_PASS) deleReq(&e2, r);
	// Case: Both elevators are idle
	else if (e1.direction == e2.direction && e1.direction == IDLE) {
		if (abs(e1.currentFloor - r.fromFloor) < abs(e2.currentFloor - r.fromFloor))
			deleReq(&e1, r);
		else deleReq(&e2, r);
	}
	// Case: Both elevators are going in the same direction as request
	else if (e1.direction == e2.direction && e1.direction == r.direction) {
		if (e1.direction == DOWN) {
			if (e1.currentFloor > r.fromFloor && e2.currentFloor > r.fromFloor) {
				if (e1.currentFloor < e2.currentFloor)
					deleReq(&e1, r);
				else deleReq(&e2, r);
			} 
			else if (e1.currentFloor < r.fromFloor)
				deleReq(&e2, r);
			else deleReq(&e1, r);
		}
		else if (e1.direction == UP) {
			if (e1.currentFloor < r.fromFloor && e2.currentFloor < r.fromFloor) {
				if (e1.currentFloor > e2.currentFloor)
					deleReq(&e1, r);
				else deleReq(&e2, r);
			}
			else if (e1.currentFloor > r.fromFloor)
				deleReq(&e2, r);
			else deleReq(&e1, r);
		}
	}
	// Case: Two elevators has different directions
	else if (e1.direction != e2.direction && e1.direction != r.direction) {
		deleReq(&e2, r);
	} else deleReq(&e1, r);
}

void rideElevators()
{

}

void exit()
{
	e1 = { 1 }; e2 = { 2 };
	cout << "\nThank you for using Bear Elevators. Have a good night <3" << endl;
}
