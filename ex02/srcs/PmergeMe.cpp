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
    int i = 0;
    std::vector<int> cpy1;
    std::vector<int> cpy2;
    std::vector<int>::reverse_iterator itr_cpy1;
    std::vector<int>::reverse_iterator itr_cpy2;

    for (std::vector<int>::iterator it = this->first.begin();
        it != this->first.end(); it++)
    {
        if (i <= pos1)
            cpy1.push_back(*it);
        else if (i > pos1 && i <= pos2)
            cpy2.push_back(*it);    
        i++;
    }
    itr_cpy1 = cpy1.rbegin();
    itr_cpy2 = cpy2.rbegin();
    while (--i >= 0)
    {
        if (i <= pos1 && itr_cpy2 != cpy2.rend())
        {
            this->first.at(i) = *itr_cpy2;
            itr_cpy2++;
        }
        else if (i > pos1 && i <= pos2 && itr_cpy1 != cpy1.rend())
        {
            this->first.at(i) = *itr_cpy1;
            itr_cpy1++;
        }
    }
}

/**
 * @brief This function sort the pairs of numbers in blocks based on the level
 * for this exemple : level = 0 | array = 45 3 15 2 89
 * we create a block of 2 pairs (because 2^(level:0)+1 = 2)
 * so these is the blocks : | 45 3 | 15 2 | 89
 * then we sort them      : | 3 45 | 2 15 | 89
 * level 1                : | 3 45   2 15 | 89
 * level 1                : | 2 15   3 45 | 89
 * When we can't create more blocks we stop the recursion
 */
int PmergeMe::recursiveMerge(int level)
{
    int tmp1 = 0;
    int tmp2 = 0;
    int pos1 = 0;
    int pos2 = 0;

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
            // std::cout << tmp1 << " | " << tmp2 << std::endl;
            if (tmp1 > tmp2)
                blockSwap(pos1, pos2);
            pos1 = 0;
            pos2 = 0;
        }
        if (level > 0 && (i + 1) % (1 << level) == 0)
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
                // std::cout << tmp1 << " | " << tmp2 
                // /* << " || " << (1 << level) << " ||| " << i % (1 << level) */ << std::endl;
                if (tmp1 > tmp2)
                    blockSwap(pos1, pos2);
                pos1 = 0;
                pos2 = 0;
            }
        }
    }
    std::cout << "end of recursive merge : ";
    printIt(this->first, level);
    if ((level == 0 && this->first.size() > 2)
        || (double)(this->first.size()) / 2 >= (double)(2 << level))
        return (recursiveMerge(level + 1));
    return (level);
}

void PmergeMe::calculMP(int &level, std::vector<int> &m, std::vector<int> &p)
{
    int j;
    int last_pos2 = 0;
    int pos1 = 0;
    int pos2 = 0;
    p.push_back(this->first.at(0));
    for (size_t i = 0; i < this->first.size(); i++)
    {
        j = 0;
        if (level > 0 && (i + 1) % (1 << level) == 0)
        {
            if (pos1 == 0)
                pos1 = i;
            else
            {
                pos2 = i;
                for (std::vector<int>::iterator it = this->first.begin();
                    it != this->first.end(); it++)
                {
                    if ((j > last_pos2 && j <= pos1))
                        p.push_back(*it);
                    else if (j > pos1 && j <= pos2)
                        m.push_back(*it);    
                    j++;
                }
                last_pos2 = pos2;
                pos1 = 0;
                pos2 = 0;
            }
        }
        else if (level == 0 && i != 0)
        {
            if (i % 2 == 0)
                p.push_back(this->first.at(i));
            else
                m.push_back(this->first.at(i));
        }
    }
}

int PmergeMe::reverseRecursiveMerge(int level, const int *j_suit)
{
    std::vector<int> m;
    std::vector<int> p;
    std::vector<int> ij;

    calculMP(level, m, p);
    std::cout << "level " << level << std::endl;
    std::cout << "m : ";
    printIt(m, -1);
    std::cout << "p : ";
    printIt(p, -1);
    std::cout << std::endl;
    (void)ij;
    // calcul the jacobsthal suit
    if (level > 0)
        reverseRecursiveMerge(level - 1, j_suit);
    return (0);
}

void PmergeMe::merge()
{
    static const int j_suit[] = {0, 1, 1, 3, 5, 11, 21, 43, 85, 171,
        341, 683, 1365, 2731, 5461, 10923, 21845, 43691, 87381, 174763, 349525};
    int end_level = 0;
    if (this->first.size() < 2)
    {
        std::cout << "Error: you should put at least two numbers" << std::endl;
        return ;
    }
    std::cout << "Before: " << std::endl;
    printIt(this->first, -1);
    end_level = recursiveMerge(0);
    reverseRecursiveMerge(end_level, j_suit);
    std::cout << "After: " << std::endl;
    printIt(this->first, -1);
}

void PmergeMe::printIt(std::vector<int> &print_vec, const int &level)
{
    (void)level;
    for (std::vector<int>::iterator it = print_vec.begin();
        it != print_vec.end(); it++)
    {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
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
