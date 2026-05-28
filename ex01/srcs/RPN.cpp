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

bool RPN::operationCheck(size_t i)
{
    if (_exp[i] == '-' || _exp[i] == '+' || _exp[i] == '*' || _exp[i] == '/')
        return (true);
    return (false);
}

bool RPN::simpleCheck(std::string &error_message, size_t &i)
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
    if (i <= 3 && operationCheck(i))
    {
        error_message = "Error: the operation sign is not correctly used";
        return false;
    }
    // else if (operationCheck(i) && i + 4 < _exp.size() && !operationCheck(i + 4))
    // {
    //     error_message = "Error: wrong input";
    //     return false;
    // }
    return true;
}

/**
 * @brief check if there is no forbidden characters such as : 
 * alphanumerique characters, and ()
 */
bool RPN::expressionCheck(std::string &error_message)
{
    size_t i = 0;
    int nb_digits = 0;
    int last_digit = 0;
    int last_ope = 0;

    while (_exp[i])
    {
        if (!simpleCheck(error_message, i))
            return false;
        if (isdigit(_exp[i]))
        {
            nb_digits++;
            if (last_digit == 2)
            {
                error_message = "Error: You can't put two digit in a row";
                return false;
            }
            last_digit++;
            last_ope = 0;
        }
        else if (operationCheck(i))
        {
            if ((i > 0 && _exp[i - 1] != ' ') 
                || (i + 1 < _exp.size() && _exp[i + 1] != ' '))
            {
                error_message = "Error: You have to put a space before and after any operations";
                return false;
            }
            last_digit = 0;
            // if (last_ope == 1)
            // {
            //     error_message = "Error: You can't put two operation in a row";
            //     return false;
            // }
            last_ope++;
        }
        if (nb_digits == 10)
        {
            error_message = "Error: You should put less than 10 digits";
            return false;
        }
        i++;
    }
    return true;
}

// member functions
bool RPN::calcul()
{
    long int res = 0;
    char digit;
    char operation;
    std::string error_message;

    if (_exp.empty())
    {
        std::cout << "there's no expression to be calculed" << std::endl;
        return false;
    }
    // check if the expression is calculable
    if (!expressionCheck(error_message))
    {
        std::cout << error_message << std::endl;
        return false;
    }
    fillStack();
    res = getDigit() - 48;
    while (i_stack.size() > 0)
    {
        if (i_stack.top() == '-' || i_stack.top() == '+' || i_stack.top() == '*' || i_stack.top() == '/')
        {
            operation = getDigit();
            if (operation == '+')
            {
                res += digit - 48;
            }
            if (operation == '-')
                res -= digit - 48;
            else if (operation == '*')
                res *= digit - 48;
            else if (operation == '/')
            {
                int div = digit - 48;
                if (div == 0)
                {
                    std::cout << "Error: it's impossible to do a division by 0" << std::endl;
                    return false;
                } 
                res /= div;
            }
            operation = '\0';
        }
        else 
        {
            digit = getDigit();
        }
        if (res > 2147483647 || res < -2147483648)
        {
            std::cout << "Error: the result overflow the limit" << std::endl;
            return false;
        }
    }
    setResult(res);
    return true;
}

// setters
void RPN::fillStack()
{
    for (int i = _exp.size(); i >= 0; i--)
    {
        i_stack.push(_exp[i]);
    }
}

void RPN::setExpression(const std::string &exp)
{
    this->_exp = exp;
}

void RPN::setResult(long int &result)
{
    this->_result = result;
}

// getters
char RPN::getDigit()
{
    char c;
    
    if (i_stack.size() > 0)
    {
        c = i_stack.top();
        i_stack.pop();
        if (i_stack.size() > 0)
            i_stack.pop();
    }
    else
        return '0';
    return c;
}

std::string RPN::getExpression()
{
    return (this->_exp);
}

long int RPN::getResult()
{
    return (this->_result);
}
