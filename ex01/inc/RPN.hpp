#ifndef RPN_HPP
#define RPN_HPP

#include <iostream>
#include <algorithm>
#include <stack>

class RPN {
    private:
        std::string _exp;
        long int _result;
        std::stack<long int> i_stack;

    public:
        RPN();
        RPN(const RPN &cpy);
        RPN &operator=(const RPN &cpy);
        ~RPN();

        // member functions
        bool checkExpression(std::string &error_message);
        void calcul();

        // setters
        void setExpression(const std::string &expression);
        void setResult(long int &result);

        // getters
        std::string getExpression();
        long int getResult();
        
} ;

#endif