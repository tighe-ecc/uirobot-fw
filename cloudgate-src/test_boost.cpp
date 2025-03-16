#include <boost/shared_ptr.hpp>
#include <iostream>

int main() {
    boost::shared_ptr<int> ptr(new int(10));
    std::cout << "Shared pointer value: " << *ptr << std::endl;
    return 0;
}
