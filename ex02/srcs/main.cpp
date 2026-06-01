#include "PmergeMe.hpp"

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        std::cout << "You should pass a sequence of numbers" << std::endl
        << "exemple = ./PmergeMe \"1 56 29 3 7\"" << std::endl;
        return 1;
    }
    PmergeMe merge_calculator;

    merge_calculator.insert(argv[1]);
    merge_calculator.merge();
    return 0;
}