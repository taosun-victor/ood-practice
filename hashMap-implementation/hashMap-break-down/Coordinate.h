#ifndef COORDINATE_H
#define COORDINATE_H

#include <iostream>
#include <vector>

using namespace std;

// ============== custom class, test purpose ==================
class Coordinate {
private:
	int x;
	int y;
	
public:
	Coordinate();
	
	Coordinate(int x, int y);
	
	int getX();
	
	int getY();
	
	Coordinate& operator=(const Coordinate& other);
	
	bool operator==(Coordinate other);
	
	bool operator!=(Coordinate other);
};

#endif
