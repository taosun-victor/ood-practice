/*******************************************************************************************************
This is a simple and naive implementation of Elevator system.

Elevator class:
The Elevator class is designed to fulfill two kinds of request at each floor
    1. A user outside the elevator at this floor pressed UP/DOWN button
    2. A user inside the elevator wants to get out at this floor	
    
Controller class:
The Controller class is used to assign requests to elevators


The code is revised according to the comments from Zhao LaoShi.
Comment 1: if destFloorOut tracks the destination floors, we're assuming passengers provide their destination 
           floors while outside of the elevator. Then for simulation, we may not need destFloorIn any more 
		   to avoid redundant information.
Answer 1: The destFloorIn is removed. The revised design uses two hashMap to track the users' requests. 
		  a. "destRequests" tracks all requests assigned to the elevator
		  b. "destProcess" tracks the requests under processing. 
		  c. When transfering from "destRequests" to "destProcess", the number of people inside the elevator 
		     should not go beyond capacity (numOfPeople < Capacity). See Answer 3.
            
Comment 2: in function step(), do we only check the first request element in the queue?
Answer 2: The revised design checks all outside requests and inside requests to determine the next direction
          a. If currently moving UP and there is no request for upper levels, change next direction to DOWN
          b. If currently moving DOWN and there is no request for lower levels, change next direction to UP
          
Comment 3: what's the difference between capacity and maxRequests? Looks they share the same semantic meaning.
Answer 3: The revised design has different meanings for "Capacity" and "maxRequests"
		  a. "maxRequests" tells whether the elevator could still accept request assignments.
		  b. "Capacity" is the max number of people could be in the elevator.
		  c. Imagine an elevator has "maxRequests = 10" and "Capacity = 3". At floor 2 the elevator has 2 people
		  	 inside. Multiple requests are at Floor 2 such as (2,3), (2,5), (2,7), (2,1). Then the elevator
			 could only load one request at floor 2 due to capacity limit. The elevator could fulfill the rest
			 requests when it comes back to floor 2 later on with more space.

Comment 4: no need to have numOfElevators, since we can get the size from Controller#elevators.
Answer 4: The revised design still has "numOfElevators", it is used at the Controller constructor to create 
	      those number of elevator objects and push them into "elevators" vector.
		  
Comment 5: Think more about Elevator#isAvailable. In real life an elevator system usually does not consider 
           elevator's load while scheduling.
Answer 5: The revised design only considers "maxRequests" for scheduling. The elevator's load is considered
          when loading people in at the current floor. See Answer 3.
		  
Comment 6: The strategy in assignRequests is too simple. We can consider the distance between an elevator and 
           the outside requests to determine assignment.
Answer 6: The revised design adopts the following strategy for scheduling.
		  a. find all the elevators currently moveing up (uplist) and down (downlist)
		  b. If the user wants to go to upper level floor, such as (2,6) --> user at floor 2 goes to floor 6
		     (1). iterate through uplist, find the closest available elevator __BELOW__ the outFloor 
		     (2). if not possible, iterate through downlist, find the closest available elevator
		     (3). if still not possible, iterate through the whole elevator list and find the first available one
		     (4). if still still not possible, return error.
	      c. If the user wants to go to lower level floor, such as (5,1) --> user at floor 5 goes to floor 1,
		     follow similar process	
*********************************************************************************************************/
#include <iostream>
#include <limits.h>
#include <random>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <utility>


using namespace std;

enum ElevatorDirection {
	UP, DOWN
};

class Elevator {
private:
	int maxRequests;
	int maxFloor, minFloor;
	
	int numOfPeople;
	int Capacity;
	
	int currFloor;
	ElevatorDirection currMoveDir;
	
	/* use a hashmap to track the user requests
	   key: which floor the users are at --> outside the elevator
	   value: a hashset for floors users' destinations --> inside the elevator
	   e.g. {2 : [3, 6, 1]}
	   several users at floor 2 want to go to floor 3, 6 and 1.  */        
	unordered_map<int, unordered_set<int>> destRequests; // requested and not processed yet
	unordered_map<int, unordered_set<int>> destProcess;  // under processing

public:
	Elevator(int max_requests, int max_floor, int min_floor, int cap){
		maxRequests = max_requests;
		maxFloor = max_floor;
		minFloor = min_floor;
		
		numOfPeople = 0;
		Capacity = cap;
		
		currFloor = min_floor + rand() % (max_floor - min_floor);
				
		double randNum = (double)rand() / (RAND_MAX + 1.0);
		currMoveDir = (randNum > 0.5 ? UP : DOWN);
	}
	
	int getCurrFloor(){
		return currFloor;
	}
	
	ElevatorDirection getCurrMoveDir() {
		return currMoveDir;
	}
	
	int getNumRequests(){
		int count = 0;
		for (auto m : destRequests)
			count += m.second.size();
		return count;
	}
	
	int isAvailable(){
		return getNumRequests() < maxRequests;
	}
	
	bool finish() {
		return destRequests.empty() && destProcess.empty();
	}
	
	void addNewRequest(int outFloor, int inFloor){
		destRequests[outFloor].insert(inFloor);
	}
	
	void printRequests(){
		for (auto m : destRequests){
			cout << "outside " << m.first << ": ";
			for (int n : m.second)
				cout << n << ", ";
			cout << endl;
		}
		cout << endl;
	}
	
	void printProcess(){
		for (auto m : destProcess){
			cout << "outside " << m.first << ": ";
			for (int n : m.second)
				cout << n << ", ";
			cout << endl;
		}
		cout << endl;
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
			// if currMoveDir is UP and there is no requests for upper levels
			// then change to move DOWN
			if (currMoveDir == UP){
				int nextUpperFloor = minFloor;
				// first check outside requesting floor
				for (auto m : destRequests)
					nextUpperFloor = max(nextUpperFloor, m.first);
				// then check inside requesting destinations
				for (auto m : destProcess){
					for (int n : m.second)
						nextUpperFloor = max(nextUpperFloor, n);
				}
					
				if (nextUpperFloor < currFloor)
					currMoveDir = DOWN;
			}
			
			// if currMoveDir is DOWN and there is no requests for lower levels
			// then change to move UP
			else {
				int nextLowerFloor = maxFloor;
				// first check outside requesting floor
				for (auto m : destRequests)
					nextLowerFloor = min(nextLowerFloor, m.first);
				// then check inside requesting destinations
				for (auto m : destProcess){
					for (int n : m.second)
						nextLowerFloor = min(nextLowerFloor, n);
				}
				if (nextLowerFloor > currFloor)
					currMoveDir = UP;				
			}
		}
	}
	
	
	void step(){
		// load the outside requests for current floor and let outside people in
		// should not go beyond elevator capacity
		if (destRequests.count(currFloor)){
			while(numOfPeople < Capacity && !destRequests[currFloor].empty()){
				destProcess[currFloor].insert(*destRequests[currFloor].begin());
				destRequests[currFloor].erase(destRequests[currFloor].begin());
				numOfPeople++;
			}
			if (destRequests[currFloor].empty())
				destRequests.erase(currFloor);
		}
		
		// process inside requests targeting currFloor and let inside person out
		vector<int> emptyIdx;
		for (auto& request : destProcess){
			if (request.second.count(currFloor)){
				request.second.erase(currFloor);
				numOfPeople--;
				if (request.second.empty())
					emptyIdx.push_back(request.first);
			}
		}
		for (int idx : emptyIdx)
			destProcess.erase(idx);
		
		// decide which direction for next move
		nextDirection();
		
		// move next
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
	int maxElevatorRequests;
	int elevatorCapacity;
	
	vector<Elevator*> elevators;
	vector<pair<int, int>> requests;
	
public:
	Controller(int num_elevators, int num_floors, int max_elevator_requests, int elevator_cap){
		numOfElevators = num_elevators;
		numOfFloors = num_floors;
		maxElevatorRequests = max_elevator_requests;
		elevatorCapacity = elevator_cap;
		
		for (int i = 0; i < numOfElevators; i++)
			elevators.push_back(new Elevator(maxElevatorRequests, numOfFloors, 1, elevatorCapacity));
	}
	
	~Controller(){
		for (int i = 0; i < numOfElevators; i++)
			delete elevators[i];
		cout << "cleared the controller!" << endl;
	}
	
	void loadRequests(vector<pair<int, int>>& req){
		for (auto r : req)
			requests.push_back(r);
	}
	
	
	/* the strategy to assign a request to an elevator
	1. find all the elevators currently moveing up (uplist) and down (downlist)
	2. If the user wants to go to upper level floor
		a. iterate through uplist, find the closest available elevator BELOW the outFloor 
		b. if not possible, iterate through downlist, find the closest available elevator
		c. if still not possible, iterate through the whole elevator list and find the first available one
		d. if still still not possible, return error.
	3. If the user wants to go to lower level floor, follow similar process */	
	void assignRequests(){
		vector<int> upList, downList;
		for (int i = 0; i < numOfElevators; i++){
			if (elevators[i]->getCurrMoveDir() == UP)
				upList.push_back(i);
			else
				downList.push_back(i);
		}
		
		for (auto r : requests){
			int outFloor = r.first, inFloor = r.second;
			bool success = assignUserRequest(upList, downList, outFloor, inFloor);
			if (!success){
				cout << "unable to assign request (outFloor: " << outFloor << ", inFloor: " << inFloor << ")" << endl;
				return;
			}
		}
		cout << "assign request successful" << endl;
		printRequestsAssignment();
		cout << "-------------------------" << endl;
	}
	
	
	bool assignUserRequest(vector<int>& upList, vector<int>& downList, int outFloor, int inFloor){
		bool UserWantsUp = (outFloor < inFloor);
		int minDist = INT_MAX, minDistIdx = -1;
		if (UserWantsUp){
			// wants to go up, try to assign to some elevator in uplist
			for (int up : upList){
				if (elevators[up]->getCurrFloor() < outFloor &&
					elevators[up]->isAvailable() &&
				    outFloor - elevators[up]->getCurrFloor() < minDist){
				    minDist = outFloor - elevators[up]->getCurrFloor();
				    minDistIdx = up;
				}
			}			
		} 
		else {
			// wants to go down, try to assign to some elevator in downlist
			for (int down : downList){
				if (elevators[down]->getCurrFloor() > outFloor &&
					elevators[down]->isAvailable() &&
				    elevators[down]->getCurrFloor() - outFloor < minDist){
				    minDist = elevators[down]->getCurrFloor() - outFloor;
				    minDistIdx = down;
				}
			}			
		}
		if (minDistIdx != -1){
			elevators[minDistIdx]->addNewRequest(outFloor, inFloor);
			return true;
		}
		
		//if not successful, try the other moving direction, find the closest
		vector<int>& secondPrimaryList = (UserWantsUp ? downList : upList);
		for (int idx : secondPrimaryList){
			if (elevators[idx]->isAvailable() && 
			    abs(elevators[idx]->getCurrFloor() - outFloor) < minDist){
				minDist = abs(elevators[idx]->getCurrFloor() - outFloor);
				minDistIdx = idx;
			}
		}
		if (minDistIdx != -1){
			elevators[minDistIdx]->addNewRequest(outFloor, inFloor);
			return true;
		}
		
		//if still not successful, try iterate through all elevator and find the first available
		for (int i = 0; i < numOfElevators; i++){
			if (elevators[i]->isAvailable()){
				elevators[i]->addNewRequest(outFloor, inFloor);
				return true;
			}
		}
		
		//assign failure
		return false;				
	}
	
	void printRequestsAssignment(){
		for (int i = 0; i < numOfElevators; i++){
			cout << "print requests for elevator " << i << endl;
			elevators[i]->printRequests();
		}
	}
	
	void printRequestsProcess(){
		for (int i = 0; i < numOfElevators; i++){
			cout << "print process for elevator " << i << endl;
			elevators[i]->printProcess();
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
			printRequestsAssignment();
			printRequestsProcess();
			cout << "----------------------" << endl;
		}
		cout << "finish all" << endl;
	}	
};


int main() {
	int num_elevators = 3;
	int num_floors = 8;
	int max_elevator_requests = 4;
	int elevator_cap = 1;
	
	Controller controller(num_elevators, num_floors, max_elevator_requests, elevator_cap);
	vector<pair<int, int>> reqs{{1, 3}, {3, 5}, {8, 2}, {7, 4}, 
								{2, 6}, {6, 3}, {6, 8}, {4, 5}, 
								{5, 7}, {6, 5}, {8, 4}, {1, 7}};
	controller.loadRequests(reqs);
	controller.assignRequests();
	controller.run();
	
	return 0;
}

