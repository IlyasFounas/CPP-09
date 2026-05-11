#ifndef BITCOINEXCHANGE_HPP
#define BITCOINEXCHANGE_HPP

#include <iostream>
#include <map>
#include <exception>
#include <fstream>
#include <string>
#include <sstream>

class BitcoinExchange {
    private:
        std::string _file_name;
        std::map<std::string, std::string> _internal_db;

    public:
        BitcoinExchange();
        BitcoinExchange(const std::string &file_name);
        BitcoinExchange(const BitcoinExchange &cpy);
        BitcoinExchange &operator=(const BitcoinExchange &cpy);
        ~BitcoinExchange();

        // setters
        void setFileName(const std::string &file_name);
        
        // getters
        std::string getFileName();

        // member functions
        void        parseDataFile();
        void        parseInputFile();
        void        insertDataValue(const std::string &s1, const std::string &s2);
        void        printDbValues();
        std::string calculTheExchange(const std::string &s1, const std::string &s2,
            std::string &error_message);
        int         ft_atoi(const std::string &s, int &err);

        // exceptions
        class NoFileException : public std::exception {
            public:
				virtual const char *what() const throw();
        } ;

        class BadInputException : public std::exception {
            public:
				virtual const char *what() const throw();
        } ;

        class BadInputFirstLineException : public std::exception {
            public:
				virtual const char *what() const throw();
        } ;
};

#endif