#include <iostream>
#include "Person.h"
#include "PersonBuilder.h"

PersonBuilder Person::create(string name) { 
	return PersonBuilder(name); 
}

string Person::getAddress(){
	return street_address + ", " + 
	       city + ", " + 
		   post_code;
}

string Person::getName(){
	return name;
}

string Person::getWork(){
	return company_name + ", " + 
	       position + ", " + 
		   annual_income;
}

