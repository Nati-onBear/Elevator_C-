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
	vector<int> requests;

	int getCurrentFloor() { return currentFloor; }
	int getCurrentPasser() { return currentPasser; }
	enum::direction getCurrentDirection() { return direction; }

	void printRequests() {
		cout << "Requests of e" << id << ": " << endl;
		vector<int>::iterator it;
		for (it = requests.begin(); it != requests.end(); it++)
			cout << *it << (it == requests.end() ? NULL : ", ");
		cout << endl;
	}

	void printInfo() {
		string dir = (direction == UP) ? "UP" : ((direction == DOWN) ? "DOWN" : "IDLE");
		cout << "================================================\n";
		cout << "Elevator #" << id << endl;
		cout << "Elevator is currently at floor " << currentFloor <<endl;
		cout << "Elevator's current number of people is " << currentPasser << endl;
		cout << "Elevator's current direction is " << dir << endl;
		cout << "================================================\n";
	}

	bool isValidReq(int floor)
	{
		if (currentPasser == MAX_PASS) return false;
		return true;
	}
};

void operateElevators();

Request getRequest();

void delegateRequest(Request r);

void rideElevators(Elevator* e);

void exit();

// Init data
Elevator e1 = { 1 };
Elevator e2 = { 2 };

int main(int argc, char** argv) {
	cout << "Building's number of floor is: " << FLOOR_NUM << endl;
	cout << "A waiting area's number of elevators is: " << ELEVATOR_NUM << endl;
	cout << "Elevator's number of maximum passengers is: " << MAX_PASS << endl;

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
			Request req = getRequest();
			delegateRequest(req);

			// for test
			e1.printRequests();
			e2.printRequests();
		}
		else if (isReq == 2) {
			// rideElevators();
		}
		else cout << "Wrong input for request!!!" << endl;
	}
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

// Delegate request to one of elevators 
void delegateRequest(Request r) {
	// Lambda function for delegate request and increment passer
	auto deleReq = [](Elevator *e, Request r) {
		e->requests.push_back(r.fromFloor);
		e->currentPasser++;
		if (e->direction == IDLE) e->direction = r.direction;
	};

	// TODO: check for direction...
	// Case: Full for both elevators
	// TODO implement these as isValidReq
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

void rideElevators(Elevator* e)
{
	int currFloor = e->currentFloor;
	int currCapa = e->currentPasser;

	cout << "For elevator " << e->id << ", the current floor is " << currFloor << ", number of people in it is " << currCapa << endl;
	// TODO sort req
	
	while (!e->requests.empty()) {
		if (e->direction == UP) {
			e->currentFloor = e->requests[0];
		}
		else if (e->direction == DOWN) {
			e->currentFloor = e->requests[e->requests.size() - 1];
		}

		currFloor = e->currentFloor;
		currCapa = e->currentPasser;

		auto curr_floor_req = find(e->requests.begin(), e->requests.end(), e->currentFloor);
		while (curr_floor_req != e->requests.end())
		{
			e->requests.erase(curr_floor_req); //removing current floor's requests
			currCapa--;
			curr_floor_req = find(e->requests.begin(), e->requests.end(), e->currentFloor);
		}

		e->currentPasser = currCapa;

		// Display info
		e->printInfo();

		if (currFloor == FLOOR_NUM) e->direction = DOWN;
		else if (currFloor == 1) e->direction = UP;

		if (currCapa < MAX_PASS) {
			getRequest();
			// TODO sort here or in delegate
			// Problem: can't reach e2
		}
	}
}

void exit()
{
	e1 = { 1 }; e2 = { 2 };
	e1.printInfo();
	e2.printInfo();
	cout << "\nThank you for using Bear Elevators. Have a good night <3" << endl;
}
