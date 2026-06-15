#include "BitcoinExchange.hpp"

int basic_process(BitcoinExchange &btc_program, const std::string &file_name)
{
    btc_program.setFileName(file_name);
    try
    {
        btc_program.parseDataFile();
        btc_program.parseInputFile();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    return 0;   
}

int main(int argc, char **argv)
{
    BitcoinExchange btc_program;
    int state = 0;
    if (argc < 2 || argc > 2)
    {
        std::cerr << "Error no file provided." << std::endl;
        return (1);
    }
    state = basic_process(btc_program, argv[1]);
    return (state);
}