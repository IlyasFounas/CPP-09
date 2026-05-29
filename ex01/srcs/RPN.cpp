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
    if (this->_exp[i] == '-' || this->_exp[i] == '+' || this->_exp[i] == '*' || this->_exp[i] == '/')
        return (true);
    return (false);
}

/**
 * @brief check if there is no forbidden characters such as : 
 * alphanumerique characters, and ()
 */
bool RPN::expressionCheck(std::string &error_message)
{
    int nb_digits = 0;
    int nb_ope = 0;
    for (size_t i = 0; this->_exp[i]; i++)
    {
        if ((this->_exp[i] >= 'a' && this->_exp[i] <= 'z') 
            || (this->_exp[i] >= 'A' && this->_exp[i] <= 'Z'))
        {
            error_message = "Error: alpha numeric characters are forbidden";
            return false;
        }
        else if (this->_exp[i] == '(' || this->_exp[i] == ')')
        {
            error_message = "Error: the parenthese are not handled";
            return false;
        }
        else if (isdigit(this->_exp[i]) || operationCheck(i))
        {
            if (nb_digits == 1 || nb_ope == 1)
            {
                error_message = "Error: need space between two characters";
                return false;
            }
            nb_digits++;
            nb_ope++;
        }
        else if (this->_exp[i] == ' ')
        {
            nb_digits = 0;
            nb_ope = 0;
        }
    }
    return true;
}

// member functions
bool RPN::calcul()
{
    long int res = 0;
    long int nb1 = 0;
    long int nb2 = 0;
    std::string error_message = "Error: empty input";

    if (this->_exp.empty() || !expressionCheck(error_message))
    {
        std::cout << error_message << std::endl;
        return false;
    }
    for (size_t i = 0; this->_exp[i]; i++)
    {
        if (operationCheck(i))
        {
            if (this->i_stack.size() < 2)
            {
                std::cout << "Error: not enough digit to do the operation" << std::endl;
                return false;
            }
            // calcul
            nb2 = this->i_stack.top();
            this->i_stack.pop();
            nb1 = this->i_stack.top();
            this->i_stack.pop();
            if (this->_exp[i] == '-')
                res = nb1 - nb2;
            else if (this->_exp[i] == '+')
                res = nb1 + nb2;
            else if (this->_exp[i] == '*')
                res = nb1 * nb2;
            else if (this->_exp[i] == '/')
            {
                if (nb2 == 0)
                {
                    std::cout << "Error: divide by 0 is not possible" << std::endl;
                    return false;
                }
                res = nb1 / nb2;
            }
            if (res > 2147483647 || res < -2147483649)
            {
                std::cout << "Error: overflow occured" << std::endl;
                return false;
            }
            this->i_stack.push(res);
        }
        else if (this->_exp[i] != ' ')
            this->i_stack.push(this->_exp[i] - 48);
    }
    if (this->i_stack.size() == 1)
        setResult(this->i_stack.top());
    else if (this->i_stack.size() > 1)
    {
        std::cout << "Error: wrong input" << std::endl;
        return false;
    }
    return true;
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
