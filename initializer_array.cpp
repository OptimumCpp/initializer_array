// initializer_array.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <list>
//#include <utility>
#include <vector>
#include <iostream>
#include <stdexcept>
#include "initializer_array.hpp"

typedef lib_fm::replicate_initializer<std::vector<int>> vector;
typedef lib_fm::replicate_initializer<std::list<int>> list;

void test(lib_fm::initializer_array<int> tmp) {
    endl(endl(endl(std::cout) << __func__) << "size=" << tmp.size());
    for (auto&& x:tmp)
        endl(std::cout << "value:" << x);
    endl(std::cout);
};

int main()
{
    {
        test({{ 11,55,88 }});

        endl(std::cout);

        lib_fm::initializer_array<int> tmp{{1,5,8,6,4}};

        endl(endl(endl(std::cout) << __func__) << "size=" << tmp.size());
        if (tmp.size())
        {
            endl(endl(std::cerr) << "Ooooch: Our workaround failed");
            return EXIT_FAILURE;
        }

        for (auto&& x : tmp)
            endl(std::cout << "value:" << x);
    };

    {
        endl(endl(endl(std::cout) << "Temporary size:") << lib_fm::initializer_array<int>{ { 333, 777, 786, 601 } }.size());

        endl(std::cout);
    };

    {
        vector vct{ {{ 1234,5678,9101,2131,4151 }} };

        endl(endl(endl(std::cout) << "vector size:") << vct.size());

        for (auto&& x : vct)
            endl(std::cout << "value:" << x);

        endl(std::cout);
    };

    {
        list lst = { {{12345,67891,01112,13141,51617}} };

        endl(endl(endl(std::cout) << "list size:") << lst.size());

        for (auto&& x : lst)
            endl(std::cout << "value:" << x);
    };

    return EXIT_SUCCESS;
};
