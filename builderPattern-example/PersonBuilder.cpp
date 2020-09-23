#include "PersonBuilder.h"

PersonBuilder&  PersonBuilder::lives() { return *this; }

PersonBuilder&  PersonBuilder::works() { return *this; }

PersonBuilder&  PersonBuilder::with(string company_name) {
    person.company_name = company_name; 
    return *this;
}

PersonBuilder&  PersonBuilder::as_a(string position) {
    person.position = position; 
    return *this;
}

PersonBuilder&  PersonBuilder::earning(string annual_income) {
    person.annual_income = annual_income; 
    return *this;
}

PersonBuilder&  PersonBuilder::at(std::string street_address) {
    person.street_address = street_address; 
    return *this;
}

PersonBuilder&  PersonBuilder::with_postcode(std::string post_code) {
    person.post_code = post_code; 
    return *this;
}

PersonBuilder&  PersonBuilder::in(std::string city) {
    person.city = city; 
    return *this;
}
