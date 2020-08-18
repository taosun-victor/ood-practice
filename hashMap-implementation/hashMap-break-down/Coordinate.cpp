#include <iostream>
#include <vector>
#include "Coordinate.h"

using namespace std;

Coordinate::Coordinate(){
	x = 0;
	y = 0;
}

Coordinate::Coordinate(int x, int y){
	this->x = x;
	this->y = y;
}

int Coordinate::getX(){
	return this->x;
}

int Coordinate::getY(){
	return this->y;
}

Coordinate& Coordinate::operator=(const Coordinate& other){
	this->x = other.x;
	this->y = other.y;
	return *this;
}

bool Coordinate::operator==(Coordinate other){
	return other.x== this->x && other.y== this->y;
}

bool Coordinate::operator!=(Coordinate other){
	return other.x != this->x || other.y != this->y;
}

namespace std{
	template<>
	struct hash<Coordinate>{
		size_t operator() (Coordinate coord) const {
			return hash<int>()(coord.getX()) * 101 + hash<int>()(coord.getY());
		}
	};
}
