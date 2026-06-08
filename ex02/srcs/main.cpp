#include "PmergeMe.hpp"

int main(int argc, char **argv)
{
    int error = 0;
    PmergeMe merge_calculator;
    
    if (argc != 2)
    {
        std::cout << "You should pass a sequence of numbers" << std::endl
        << "exemple = ./PmergeMe \"1 56 29 3 7\"" << std::endl;
        return 1;
    }
    merge_calculator.insert(argv[1], error);
    if (error == 1)
        return 1;
    merge_calculator.merge();
    return 0;
}