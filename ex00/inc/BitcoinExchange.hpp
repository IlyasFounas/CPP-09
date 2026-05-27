#ifndef BITCOINEXCHANGE_HPP
#define BITCOINEXCHANGE_HPP

#include <iostream>
#include <map>
#include <exception>
#include <fstream>
#include <string>
#include <sstream>

# define INT_MAX 2147483647
# define INT_MIN -2147483648

class BitcoinExchange {
    private:
        std::string _file_name;
        std::map<std::string, std::string> _internal_db;
        long int year;
        long int month;
        long int day;
        long int year_csv;
        long int month_csv;
        long int day_csv;

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
        bool        checkRequirements(long int year, long int month, long int day,
            std::string &error_message, int &err);
        long int    extractDateValue(const std::string &date, int &err, std::string &error_message, bool csv);
        long double    returnDataValue(const std::string &input_date, int &err,
            std::string &error_message);
        long int calculTheExchange(const std::string &s1, const std::string &s2,
            std::string &error_message);
        long int         ft_atoi(const std::string &s, int &err, std::string &error_message);


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