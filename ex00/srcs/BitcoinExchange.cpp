#include "BitcoinExchange.hpp"
#include <cstdlib>
#include <cerrno>
#include <cmath>

BitcoinExchange::BitcoinExchange() : _file_name("NULL"), year(0), month(0),
    day(0), year_csv(0), month_csv(0), day_csv(0) {}

BitcoinExchange::BitcoinExchange(const std::string &file_name) : _file_name(file_name), year(0), month(0),
    day(0), year_csv(0), month_csv(0), day_csv(0) {}

BitcoinExchange::BitcoinExchange(const BitcoinExchange &cpy)
{
    *this = cpy;
}

BitcoinExchange &BitcoinExchange::operator=(const BitcoinExchange &cpy)
{
    if (this != &cpy)
    {
        this->_file_name = cpy._file_name;
        this->_internal_db = cpy._internal_db;
    }
    return (*this);
}

BitcoinExchange::~BitcoinExchange() {}

//setters
void BitcoinExchange::setFileName(const std::string &file_name)
{
    this->_file_name = file_name;
}

// getters
std::string BitcoinExchange::getFileName()
{
    return (this->_file_name);
}


// member functions
void BitcoinExchange::parseDataFile()
{
    int i = 0;
    size_t pos;
    std::ifstream read_file;
    std::string line;
    std::string s1;
    std::string s2;

    read_file.open("data/test.csv", std::ios::app);
    while (std::getline(read_file, line))
    {

        pos = line.find(",");
        if (pos == std::string::npos)
        {
            s1 = line;
            s2 = "NULL";
        }
        else
        {
            s1 = line.substr(0, pos);
            s2 = line.substr(pos + 1);
        }
        if (i == 0)
        {
            if (s1 != "date" || s2 != "exchange_rate")
                throw BadInputException();
        }
        else
        {   if (s2.size() == 0)
                throw BadInputException();
            for (size_t i = 0; i < s2.size(); i++)
            {
                if (s2[i] != '-' && s2[i] != '.')
                {
                    if (!isdigit(s2[i]))
                        throw BadInputException();
                }
            }
        }
        insertDataValue(s1, s2);
        i++;
    }
}

void BitcoinExchange::parseInputFile()
{
    std::ifstream read_file;
    std::string line;
    std::string file_name = getFileName();
    std::string s1 = "";
    std::string s2 = "";
    size_t pos;
    int i = 0;
    
    if (file_name == "NULL")
        throw NoFileException();
    read_file.open(file_name.c_str(), std::ios::app);
    while (std::getline(read_file, line))
    {
        pos = line.find(" | ");
        if (pos == std::string::npos)
        {
            insertDataValue(line, "NULL");
            std::cout << "Error: bad input => " << line << std::endl;
            continue ;
        }
        else
        {
            s1 = line.substr(0, pos);
            s2 = line.substr(pos + 3);
        }
        if (i == 0)
        {
            if (pos == std::string::npos || (s1 != "date" || s2 != "value"))
                throw BadInputFirstLineException();
        }
        else if (i != 0)
        {
            std::string error_message;
            long double printExchange = calculTheExchange(s1, s2, error_message);

            if (!error_message.empty())
            {
                std::cout << error_message << std::endl;
            }
            else
            {
                std::cout << s1 << " => " << s2 
                << " = " << printExchange << std::endl;
            }
        }
        i++;
    }
}

void BitcoinExchange::insertDataValue(const std::string &s1, const std::string &s2)
{
    _internal_db.insert(std::make_pair(s1, s2));
}

bool BitcoinExchange::checkRequirements(long int year, long int month, long int day, std::string &error_message, int &err)
{
    if (year < 2009 || year > 2026)
    {
        error_message = "Error: no available data at this date"; 
        err = 1;
        return (false);
    }
    if (month < 1 || month > 12)
    {
        error_message = "Error: no available data at this date";
        err = 1;
        return (false);
    }
    if ((day < 1 || day > 31)
        || (day == 29 && ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))))
    {
        error_message = "Error: no available data at this date";
        err = 1;
        return (false);
    }
     if (month == 4 || month == 6 || month == 9 || month == 11) {
        if (day == 31)
        {
            error_message = "Error: no available data at this date";
            err = 1;
            return (false);
        }
    }
    return (true);
}

long int BitcoinExchange::extractDateValue(const std::string &date, int &err, std::string &error_message, bool csv)
{
    size_t pos = date.find("-");
    size_t last_pos = date.find("-", pos + 1);

    if (pos == std::string::npos || last_pos == std::string::npos)
    {
        error_message = "Error: invalid date format (expected yyyy-mm-dd)";
        err = 1;
        return (-1);
    }
    if (csv)
    {
        this->year_csv = ft_atoi(date.substr(0, pos), err, error_message);
        if (err == 1)
            return (-1);
        this->month_csv = ft_atoi(date.substr(pos + 1, last_pos - (pos + 1)), err, error_message);
        if (err == 1)
            return (-1);
        this->day_csv = ft_atoi(date.substr(last_pos + 1), err, error_message);
        if (err == 1)
            return (-1);
    }
    else
    {
        this->year = ft_atoi(date.substr(0, pos), err, error_message);
        if (err == 1)
            return (-1);
        this->month = ft_atoi(date.substr(pos + 1, last_pos - (pos + 1)), err, error_message);
        if (err == 1)
            return (-1);
        this->day = ft_atoi(date.substr(last_pos + 1), err, error_message);
        if (err == 1)
            return (-1);
    }
    return (0);
}

/**
 * @brief This function return the value of the closer input date in the csv database
 * year 2021-05-22 et input = 2011-03-06
 * last_year 2009-12-03
 */
long double BitcoinExchange::returnDataValue(const std::string &input_date, int &err, std::string &error_message)
{
    std::string closest_date;
    
    if (extractDateValue(input_date, err, error_message, false) == -1)
        return -1;
    if (!checkRequirements(this->year, this->month, this->day, error_message, err))
        return -1;
    // find the exect date in the database
    if (this->_internal_db.size() <= 0)
        return (0);
    std::map<std::string, std::string>::iterator it = this->_internal_db.lower_bound(input_date);
    if (it == this->_internal_db.end())
    {
        closest_date = this->_internal_db.rbegin()->first;
    } 
    else if (it == this->_internal_db.begin())
    {
        closest_date = it->first;
    }
    else
    {
        std::map<std::string, std::string>::iterator prev_it = it;
        prev_it--;
        long input_num = std::atol(input_date.substr(0, 4).c_str()) * 10000 +
                        std::atol(input_date.substr(5, 2).c_str()) * 100 +
                        std::atol(input_date.substr(8, 2).c_str());

        long next_num = std::atol(it->first.substr(0, 4).c_str()) * 10000 +
                       std::atol(it->first.substr(5, 2).c_str()) * 100 +
                       std::atol(it->first.substr(8, 2).c_str());

        long prev_num = std::atol(prev_it->first.substr(0, 4).c_str()) * 10000 +
                       std::atol(prev_it->first.substr(5, 2).c_str()) * 100 +
                       std::atol(prev_it->first.substr(8, 2).c_str());
        if (std::abs(next_num - input_num) < std::abs(input_num - prev_num))
            closest_date = it->first;
        else
            closest_date = prev_it->first;
    }
    // return the good value
    return std::strtold(this->_internal_db[closest_date].c_str(), NULL);
}


long double BitcoinExchange::calculTheExchange(const std::string &s1, const std::string &s2,
    std::string &error_message)
{
    int err = 0;
    char *error_check = NULL;

    long double nb = std::strtold(s2.c_str(), &error_check);
    if (errno == ERANGE || nb == HUGE_VALL || nb == -HUGE_VALL)
    {
        error_message = "Error: overflow detected.";
        return 0;
    }
    if (error_check == NULL || *error_check != '\0')
    {
        error_message = "Error: wrong input.";
        return 0;
    }
    if (nb > 2147483647 || nb < -2147483648)
    {
        error_message = "Error: overflow detected.";
        return 0;
    }
    if (nb < 0)
    {
        error_message = "Error: not a positive number.";
        return 0;
    }
    if (nb > 1000)
    {
        error_message = "Error: a valid input in between 0 and 1000.";
        return 0;
    }
    long double data_value = returnDataValue(s1, err, error_message);
    if (err == 1)
        return 0;
    return (long double)nb * data_value;
}


long int BitcoinExchange::ft_atoi(const std::string &s, int &err, std::string &error_message)
{
    size_t i = 0;
    long int res = 0;
    int negatif = 1;

    if (s.empty())
    {
        err = 1;
        error_message = "Error: there is no input.";
        return (-1);
    }
    while ((s[i] >= 9 && s[i] <= 13) || s[i] == 32)
        i++;
    if (s[i] == '-' || s[i] == '+')
    {
        if (s[i] == '-')
            negatif = -1;
        i++;
    }
    while (s[i] >= '0' && s[i] <= '9')
    {
        res = (res * 10) + (s[i] - 48);
        if (res > INT_MAX || res < INT_MIN)
        {
            if (res * negatif > 2147483647)
                error_message = "Error: too large number.";
            else
                error_message = "Error: too low number.";    
            return (res * negatif);
        }
        i++;
    }
    if (i < s.length())
    {
        err = 1;
        return (-1);
    }
    return (res * negatif);
}

// exceptions
const char *BitcoinExchange::NoFileException::what() const throw()
{
    return "Error: There's no input file";
}

const char *BitcoinExchange::BadInputException::what() const throw()
{
    return "Error: bad input in the database csv";
}

const char *BitcoinExchange::BadInputFirstLineException::what() const throw()
{
    return "Error: the first line does not respect the right format";
}