#include "smart_pointers.h"
#include <iostream>
#include <memory>


void testSmartPointers() {
    std::auto_ptr<int> api(new int (5));
    std::cout << *api << std::endl;
    std::auto_ptr<int> api2(api);
    std::cout << api.operator ->() << std::endl;
    std::cout << *api2 << std::endl;
}
