#include <iostream>
#include "Person.h"
#include "PersonBuilder.h"
using namespace std;

int main()
{
    Person p = Person::create("John")
                                .lives()
                                    .at("123 London Road")
                                    .with_postcode("SW1 1GB")
                                    .in("London")
                                .works()
                                    .with("PragmaSoft")
                                    .as_a("Consultant")
                                    .earning("10e6");

	cout << p.getName() << endl;
    cout << p.getAddress() << endl;
    cout << p.getWork() << endl;
    
    return EXIT_SUCCESS;
}
