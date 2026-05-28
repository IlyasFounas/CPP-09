#include "RPN.hpp"

int main(int argc, char **argv)
{
    long int result;
    RPN calculator;

    if (argc != 2)
        return 1;
    calculator.setExpression(argv[1]);
    if (!calculator.calcul())
        return 1;
    result = calculator.getResult();
    std::cout << "result = " << result << std::endl;
    return 0;
}
