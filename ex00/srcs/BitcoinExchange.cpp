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
void BitcoinExchange::parseInputFile()
{
    int i = 0;
    std::ifstream read_file;
    std::string line;
    std::string file_name = getFileName();
    size_t pos;
    
    if (file_name == "NULL")
        throw NoFileException();
    read_file.open(file_name.c_str(), std::ios::app);
    while (std::getline(read_file, line))
    {
        std::cout << line << std::endl;
        if (i == 0)
        {
            pos = line.find('|');
            if (pos == std::string::npos)
            {
                insertDataValue(line, "NULL");
            }
            else
            {
                insertDataValue(line.substr(0, pos), line.substr(pos + 1));
            }
        }
        i++;
    }
}

void BitcoinExchange::insertDataValue(const std::string &s1, const std::string &s2)
{
    std::cout << s1 << " " << s2 << std::endl;
    this->_internal_db[s1] = s2;

}

// exceptions
const char *BitcoinExchange::NoFileException::what() const throw()
{
    return "There's no input file";
}

const char *BitcoinExchange::BadInputException::what() const throw()
{
    return "Error: bad input =>";
}