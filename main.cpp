#include <iostream>

#include "addressbook.pb.h"

int main(int argc, const char *argv[])
{
    tutorial::Person p;
    p.set_id(21);
    p.set_name("conan-center-index");
    p.set_email("info@conan.io");

    std::cout << p.SerializeAsString() << "\n";
}
