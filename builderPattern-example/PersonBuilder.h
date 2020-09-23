#pragma once
#include "Person.h"
#include <string>

class PersonBuilder{
private:
    Person person;

public:
    PersonBuilder(string name) : person(name) {}

    operator Person() const { 
		return move(person); 
	}

    PersonBuilder&  lives();
    PersonBuilder&  at(string street_address);
    PersonBuilder&  with_postcode(string post_code);
    PersonBuilder&  in(string city);
    PersonBuilder&  works();
    PersonBuilder&  with(string company_name);
    PersonBuilder&  as_a(string position);
    PersonBuilder&  earning(string annual_income);
};
