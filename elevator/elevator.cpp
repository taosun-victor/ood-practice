/*******************************************************************************************************
This is a simple and naive implementation of Elevator system.

Elevator class:
The Elevator class is designed to fulfill two kinds of request at each floor
    1. A user outside the elevator at this floor pressed UP/DOWN button
    2. A user inside the elevator wants to get out at this floor
    
Controller class:
The Controller class is used to assign requests (a user at some floor wants to go to some other floor)
to the Elevator. It also performs the simulation for the Elevator to execute.

The modeling for the two kinds of requests as the following. 
    a. use a pair {floor1, floor2} to represents a user at floor1 outside the elevator wants to go
       to floor2.
    b. The request pair is assigned to a specific elevator by the controller
    c. The elevator has two deques: 
			deque<pair<int, int>> destFloorOut
			deque<int> destFloorIn;
		At each floor, the elevator 
		    firstly checks if anyone outside wants to get in by popping the front of "destFloorOut", 
			then it loads the user inside and push the target floor to "destFloorIn".
			
			secondly checks if anyone inside wants to get out by popping the front of "destFloorIn", 
			then it unloads the user.
	d. After necessary loading/unloading, the elevator determines the next direction to move


Potential drawback of the design by using deque: 
	The requests are processed one by one from front to back. 
	
	Suppose the elevator has requests of {1,3}{3,4}. At floor1 it will load the 1st user. Then at 
	floor3 it will unload the first user and load the 2nd user.
	
	Suppose the elevator has requests of {1,3}{4,2}{3,4}. It loads the 1st user at floor1 and unload
	at floor3. Then load the 2nd user at floor4 and unload at floor2. After that again it goes to 
	floor3 to load the 3rd user. The way how it fulfill the requests are really inconvenient.
	
*********************************************************************************************************/



#include <iostream>
#include <random>
#include <vector>
#include <queue>
#include <utility>

using namespace std;

enum ElevatorDirection {
	UP, DOWN
};

class Elevator {
private:
	int capacity;
	int currFloor;
	int currNumPeople;
	int maxRequests;
	
	int maxFloor, minFloor;
	
	ElevatorDirection currMoveDir;
	
	// .first: which floor is the user at --> outside the elevator
	// .second: which floor the user wants to go to --> inside the elevator
	deque<pair<int, int>> destFloorOut;
	
	deque<int> destFloorIn;

public:
	Elevator(int cap, int max_floor, int min_floor){
		capacity = cap;
		maxFloor = max_floor;
		minFloor = min_floor;
		maxRequests = cap;
		
		currFloor = min_floor + rand() % (max_floor - min_floor);
		currNumPeople = 0;
		
		double randNum = (double)rand() / (RAND_MAX + 1.0);
		currMoveDir = (randNum > 0.5 ? UP : DOWN);
	}
	
	int getCurrFloor(){
		return currFloor;
	}
	
	ElevatorDirection getCurrMoveDir() {
		return currMoveDir;
	}
	
	int isAvailable(){
		return currNumPeople < capacity && 
			   destFloorOut.size() < maxRequests &&
			   destFloorIn.size() < maxRequests;
	}
	
	int finish() {
		return currNumPeople == 0 && 
			   destFloorOut.empty() &&
			   destFloorIn.empty();		
	}
	
	void addNewDestOut(pair<int, int> requestOut){
		destFloorOut.push_back(requestOut);
	}
	
	void addNewDestIn(int requestIn){
		destFloorIn.push_back(requestIn);
	}
	
	void moveUp(){
		currFloor++;
	}
	
	void moveDown() {
		currFloor--;
	}
	
	void nextDirection(){
		if (currFloor == maxFloor)
			currMoveDir = DOWN;
		else if (currFloor == minFloor)
			currMoveDir = UP;
		else{
			if (!destFloorOut.empty()){
				if (destFloorOut.front().first > currFloor)
					currMoveDir = UP;
				else
					currMoveDir = DOWN;	
			}
			else if (!destFloorIn.empty()){
				if (destFloorIn.front() > currFloor)
					currMoveDir = UP;
				else
					currMoveDir = DOWN;					
			}
		}
	}
	
	
	void step(){
		// let the outside person in and mark the person's destination floor
		while (!destFloorOut.empty() && destFloorOut.front().first == currFloor){
			addNewDestIn(destFloorOut.front().second);
			destFloorOut.pop_front();
			currNumPeople++;			
		}
		
		// let the inside person out and unload the person
		while (!destFloorIn.empty() && destFloorIn.front() == currFloor){
			destFloorIn.pop_front();
			currNumPeople--;			
		}
		
		// decide which direction for next move
		nextDirection();
		
		if (currMoveDir == UP)
			moveUp();
		else
			moveDown();
	}
};


class Controller {
private:
	int numOfElevators;
	int numOfFloors;
	int elevatorCapacity;
	vector<Elevator*> elevators;
	deque<pair<int, int>> requests;
	
public:
	Controller(int num_elevators, int num_floors, int cap_elevator){
		numOfElevators = num_elevators;
		numOfFloors = num_floors;
		elevatorCapacity = cap_elevator;
		
		for (int i = 0; i < numOfElevators; i++)
			elevators.push_back(new Elevator(elevatorCapacity, num_floors, 1));
	}
	
	void loadRequests(vector<pair<int, int>>& req){
		for (auto r : req)
			requests.push_back(r);
	}
	
	void assignRequests(){
		while(!requests.empty()){
			bool assignSuccess = false;
			for (int i = 0; i < numOfElevators; i++){
				if (elevators[i]->isAvailable()){
					assignSuccess = true;
					elevators[i]->addNewDestOut(requests.front());
					requests.pop_front();
				}
			}
			if (!assignSuccess){
				cout << "No available elevators!" << endl;
				break;
			}
		}	
	}
	
	bool allFinish(){
		for (int i = 0; i < numOfElevators; i++){
			if (!elevators[i]->finish())
				return false;
		}
		return true;
	}
	
	void run(){
		while(!allFinish()){
			for (int i = 0; i < numOfElevators; i++){
				cout << "elevator " << i << " is at floor " << elevators[i]->getCurrFloor() << endl;
				elevators[i]->step();
			}
			cout << "----------------------" << endl;
		}
		cout << "finish all" << endl;
	}	
};


int main() {
	Controller controller(3, 8, 4);
	vector<pair<int, int>> reqs{{1, 3}, {3, 5}, {8, 2}, {7, 4}, 
								{2, 6}, {6, 3}, {6, 8}, {4, 5}, 
								{5, 7}, {6, 5}, {8, 4}, {1, 7}};
	controller.loadRequests(reqs);
	controller.assignRequests();
	controller.run();
	
	return 0;
}

