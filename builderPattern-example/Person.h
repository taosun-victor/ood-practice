#pragma once
#include <iostream>
#include <string>

using namespace std;

class PersonBuilder;

class Person{
private:
    const string name;
    
	string street_address;
	string post_code;
	string city;  // Personal Detail
    
	string company_name;
	string position;
	string annual_income;    // Employment Detail

    Person(string name) : name(name) {
	
	}

public:
    friend class PersonBuilder;
    static PersonBuilder create(string name);
    
    string getName();
    string getAddress();
    string getWork();
};
