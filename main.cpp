#include <boost/program_options.hpp>
#include <iostream>
#include "fmt/color.h"

#include "addressbook.pb.h"

using namespace boost::program_options;

void on_age(int age)
{
    std::cout << "On age: " << age << '\n';
}

int main(int argc, const char *argv[])
{
    fmt::print(fg(fmt::terminal_color::cyan), "Hello!\n");

    try
    {
        options_description desc{"Options"};
        desc.add_options()
            ("help,h", "Help screen")
            ("pi", value<float>()->default_value(3.14f), "Pi")
            ("age", value<int>()->notifier(on_age), "Age");

        variables_map vm;
        store(parse_command_line(argc, argv, desc), vm);
        notify(vm);

        if (vm.count("help"))
            std::cout << desc << '\n';
        else if (vm.count("age"))
            std::cout << "Age: " << vm["age"].as<int>() << '\n';
        else if (vm.count("pi"))
            std::cout << "Pi: " << vm["pi"].as<float>() << '\n';
    }
    catch (const error &ex)
    {
        std::cerr << ex.what() << '\n';
    }

    tutorial::Person p;
    p.set_id(21);
    p.set_name("conan-center-index");
    p.set_email("info@conan.io");

    std::cout << p.SerializeAsString() << "\n";
}
