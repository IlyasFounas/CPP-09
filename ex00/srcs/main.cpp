#include "BitcoinExchange.hpp"

void basic_process(BitcoinExchange &btc_program, const std::string &file_name)
{
    btc_program.setFileName(file_name);
    try
    {
        btc_program.parseDataFile();
        btc_program.parseInputFile();
        // btc_program.printDbValues();
    }
    catch(const std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }    
}

int main(int argc, char **argv)
{
    BitcoinExchange btc_program;
    if (argc < 2 || argc > 2)
    {
        std::cout << "Error no file provided." << std::endl;
        return (1);
    }
    basic_process(btc_program, argv[1]);
    return (0);
}