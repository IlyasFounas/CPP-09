#ifndef RPN_HPP
#define RPN_HPP

#include <iostream>
#include <algorithm>
#include <stack>

class RPN {
    private:
        std::string _exp;
        long int _result;
        std::stack<char> i_stack;

    public:
        RPN();
        RPN(const RPN &cpy);
        RPN &operator=(const RPN &cpy);
        ~RPN();

        // member functions
        bool operationCheck(size_t i);
        bool simpleCheck(std::string &error_message, size_t &i);
        bool expressionCheck(std::string &error_message);
        bool calcul();

        // setters
        void fillStack();
        void setExpression(const std::string &expression);
        void setResult(long int &result);

        // getters
        char getDigit();
        std::string getExpression();
        long int getResult();
        
} ;

#endif