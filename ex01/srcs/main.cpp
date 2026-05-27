#include "RPN.hpp"

int main(int argc, char **argv)
{
    long int result;
    RPN calculator;

    if (argc != 2)
        return 1;
    calculator.setExpression(argv[1]);
    calculator.calcul();
    result = calculator.getResult();
    std::cout << "result = " << result << std::endl;
    return 0;
}
