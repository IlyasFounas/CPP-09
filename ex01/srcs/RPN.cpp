#include "RPN.hpp"

RPN::RPN() : _exp(""), _result(0) {}

RPN::RPN(const RPN &cpy) 
{
    *this = cpy;
}

RPN &RPN::operator=(const RPN &cpy) 
{
    if (this != &cpy)
    {
        this->_exp = cpy._exp;
        this->_result = cpy._result;
        this->i_stack = cpy.i_stack;
    }
    return *this;
}

RPN::~RPN() {}

/**
 * @brief check if there is no forbidden characters such as : 
 * alphanumerique characters, and ()
 */
bool RPN::checkExpression(std::string &error_message)
{
    int i;

    i = -1;
    while (_exp[++i])
    {
        if ((_exp[i] >= 'a' && _exp[i] <= 'z')
            || (_exp[i] >= 'A' && _exp[i] <= 'Z'))
        {
            error_message = "Error: alpha numeric characters are forbidden";
            return false;
        }
        else if (_exp[i] == '(' || _exp[i] == ')')
        {
            error_message = "Error: the parenthese are not handled";
            return false;
        }
    }
    return true;
}

// member functions
void RPN::calcul()
{
    std::string error_message;

    if (_exp.empty())
    {
        std::cout << "there's no expression to be calculed" << std::endl;
        return ;
    }
    // check if the expression is calculable
    if (!checkExpression(error_message))
    {
        std::cout << error_message << std::endl;
        return ;
    }
    std::cout << _exp << std::endl;
}

// setters
void RPN::setExpression(const std::string &exp)
{
    this->_exp = exp;
}

void RPN::setResult(long int &result)
{
    this->_result = result;
}

// getters
std::string RPN::getExpression()
{
    return (this->_exp);
}

long int RPN::getResult()
{
    return (this->_result);
}
