#include "BitcoinExchange.hpp"

BitcoinExchange::BitcoinExchange() : _file_name("NULL") {}

BitcoinExchange::BitcoinExchange(const std::string &file_name) : _file_name(file_name) {}

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

void BitcoinExchange::setFileName(const std::string &file_name)
{
    this->_file_name = file_name;
}

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

    read_file.open("data.csv", std::ios::app);
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
        insertDataValue(s1, s2);
        i++;
    }
}

void BitcoinExchange::parseInputFile()
{
    std::ifstream read_file;
    std::string line;
    std::string file_name = getFileName();
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
        }
        else if (i != 0)
        {
            std::string s1 = line.substr(0, pos);
            std::string s2 = line.substr(pos + 3);
            std::cout << s1 << " => " << s2 
            << " = " << calculTheExchange(s1, s2) << std::endl;
        }
        i++;
    }
}

void BitcoinExchange::insertDataValue(const std::string &s1, const std::string &s2)
{
    _internal_db.insert(std::make_pair(s1, s2));
}

void BitcoinExchange::printDbValues()
{
    for (std::map<std::string, std::string>::iterator it = this->_internal_db.begin();
        it != this->_internal_db.end(); it++)
    {
        std::cout << it->first << " " << it->second << std::endl;
    }
}

std::string BitcoinExchange::calculTheExchange(const std::string &s1, const std::string &s2)
{
    (void)s1;
    (void)s2;
    return ("NULL");
}


// exceptions
const char *BitcoinExchange::NoFileException::what() const throw()
{
    return "There's no input file";
}

const char *BitcoinExchange::BadInputException::what() const throw()
{
    return "Error: bad input in the data csv";
}