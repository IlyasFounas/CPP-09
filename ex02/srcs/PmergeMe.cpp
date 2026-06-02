#include "PmergeMe.hpp"

PmergeMe::PmergeMe() {}

PmergeMe::PmergeMe(const PmergeMe &cpy) 
{
    *this = cpy;
}

PmergeMe &PmergeMe::operator=(const PmergeMe &cpy) 
{
    if (this != &cpy)
    {
        this->first = cpy.first;
        this->snd = cpy.snd;
    }
    return (*this);
}

PmergeMe::~PmergeMe() {}

// member functions
void PmergeMe::blockSwap(int &pos1, int &pos2)
{
    (void)pos1;
    (void)pos2;
}

void PmergeMe::insert(const std::string &s) 
{
    for (size_t i = 0; s[i]; i++)
    {
        if (s[i] != ' ')
        {
            first.push_back(std::atol(&s[i]));
            snd.push_back(std::atol(&s[i]));
            i = s.find(" ", i);
        }
        if (i == std::string::npos)
            return;
    }
}

// 15 2 | 3 45 | 89 
// 2 15 | 3 45 | 
int PmergeMe::recursiveMerge(int level)
{
    int tmp1 = 0;
    int tmp2 = 0;
    int pos1 = 0;
    int pos2 = 0;
    std::cout << "start of recursive merge" << std::endl;
    for (size_t i = 0; i < this->first.size(); i++)
    {
        if ((level == 0 && (i % 2 == 0 || i == 0)))
        {
            tmp1 = this->first[i];
            pos1 = i;
        }
        else if (level == 0)
        {
            tmp2 = this->first[i];
            pos2 = i;
            std::cout << tmp1 << " | " << tmp2 << std::endl;
            pos2 = 0;
        }
        if (level > 0 && i % (1 << level) != 0 && i % 2 != 0)
        {
            if (pos1 == 0)
            {
                tmp1 = this->first[i];
                pos1 = i;
            }
            else 
            {
                tmp2 = this->first[i];
                pos2 = i;
                pos2 = 0;
            }
            std::cout << tmp1 << " | " << tmp2 << " " << (1 << level) << std::endl;
        }
        if (tmp1 > tmp2)
        {
            blockSwap(pos1, pos2);
        }
    }
    if ((level == 0 && this->first.size() > 2)
        || (double)(this->first.size()) / 2 > (double)(level * 2))
        recursiveMerge(level + 1);
    return (level);
}

void PmergeMe::merge() 
{
    if (this->first.size() < 2)
    {
        std::cout << "Error: you should put at least two numbers" << std::endl;
        return ;
    }
    std::cout << "Before: " << std::endl;
    printIt(1);
    recursiveMerge(0);
    std::cout << "After: " << std::endl;
    printIt(1);
}

void PmergeMe::printIt(const int &first)
{
    if (first == 1)
    {
        for (std::vector<int>::iterator it = this->first.begin();
            it != this->first.end(); it++)
        {
            std::cout << *it << " ";
        }
        std::cout << std::endl;
    }
    else
    {
        for (std::deque<int>::iterator it = this->snd.begin();
            it != this->snd.end(); it++)
        {
            std::cout << *it << " ";
        }
        std::cout << std::endl;
    }
}