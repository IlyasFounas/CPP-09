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
 * @brief it check the last number of the block with the other last number blocks
 * when the last number of the block is inferior of a last number block
 * it insert the block before it.
 */
void PmergeMe::binaryInsert(int &level, std::vector<int> &to_insert, int &pos, int &nb_take)
{
    (void)level;
    int i = 0;
    int j = 1;
    int actual_pos;
    if (to_insert.size() <= 0 || pos == 1)
        return ;
    int cmp = *to_insert.rbegin();

    std::cout << "level= " << level << std::endl;
    if (pos % 2 != 0)
        actual_pos = pos * 2 -1;
    else
        actual_pos = pos * 2;    
    for (std::vector<int>::iterator it = this->first.begin();
        it != this->first.end(); it++)
    {
        if (j == actual_pos)
        {
            for (std::vector<int>::iterator its = this->first.begin();
                its != it; its++)
            {
                if (i != 0 && (i + 1) % (nb_take) == 0)
                {
                    if (cmp <= *(its))
                    {
                        this->first.insert(its - (nb_take - 1), to_insert.begin(), to_insert.end());
                        this->first.erase(it + nb_take, it + (nb_take * 2));
                        return ;
                    }
                }
                i++;
            }
        }
        if (i != 0 && (i + 1) % (nb_take) == 0)
        {
            j++;
        }
        i++;
    }
}

void PmergeMe::binarySearch(int &level, std::vector<int> &to_insert, std::vector<int> &p,
    std::vector<int> &ij)
{
    int i;
    int j;
    int nb_swap = *std::max_element(ij.begin(), ij.end()); //nb of number to swap
    int nb_take;

    if (level == 0)
        nb_take = (int)p.size(); 
    else
        nb_take = (1 << level);
    if (nb_swap == 1)
        return ; //don't need to search and sort
    for (std::vector<int>::iterator itj = ij.begin();
        itj != ij.end(); itj++)
    {
        i = 0;
        j = 1;
        if (*itj != 1)
        {
            for (std::vector<int>::iterator it = p.begin();
                it != p.end(); it++)
            {
                if (j == *itj)
                {
                    to_insert.push_back(*it);
                }
                if ((i != 0 && (i + 1) % nb_take == 0) || (i != 0 && level == 0))
                {
                    j++;
                }
                i++;
            }
            std::cout << "m: ";
            printIt(to_insert, -1);
            binaryInsert(level, to_insert, *itj, nb_take);
            to_insert.clear();
            std::cout << "end of reverse recursive: ";
            printIt(this->first, -1);
        }
    }
}

void PmergeMe::calculMP(int &level, std::vector<int> &p)
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
        }
    }
}

/**
 * @brief The reverseMerge take a sort paired vector : 
 * like that | 2 15   3 45 | 89
 * and based on the level, we perform a binary search + an insert algorithm
 * with the jacobsthal suit and we calcul the jacobsthal index.
 * so here we can't sort Level 1: | 2 15    3 45 | 89
 * so here we can't sort Level 0: | 2 15 |  3 45 | 89
 * so here we can sort          : | 2 | 15 | 3 | 45 | 89
 * the elements that stay in the vector are : 15 and 45
 * and the elements that will be sort are : 2, 3 and 89
 */
void PmergeMe::reverseMerge(int &level, std::vector<int> &to_insert, std::vector<int> &p, std::vector<int> &ij, const int *j_suit)
{
    int j = 0;
    int clast_insert = 0;
    int last_insert = 0;
    int psize = 0;
    int i;
    
    calculMP(level, p);
    std::cout << "level " << level << std::endl << "p : ";
    printIt(p, -1);
    // calcul the jacobsthal index
    psize = (int)p.size() / (1 << level);
    i = psize;
    while ((int)ij.size() != psize)
    {
        if (i - j_suit[j] <= 0)
            i -= j_suit[j];
        if (i < 0)
            ij.push_back(psize);
        else
            ij.push_back(j_suit[j]);
        last_insert = *--ij.end();
        clast_insert = *--ij.end();
        if (j_suit[j] != 1 && (int)ij.size() != psize)
        {
            while ((int)ij.size() != clast_insert)
                ij.push_back(--last_insert);
        }
        j++;
    }
    std::cout << "ij : ";
    printIt(ij, -1);
    std::cout << std::endl;
    binarySearch(level, to_insert, p, ij);
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
int PmergeMe::recursiveMerge(int level, std::vector<int> &to_insert, std::vector<int> &p, std::vector<int> &ij, const int *j_suit)
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
        recursiveMerge(level + 1, to_insert, p, ij, j_suit);
    p.clear();
    to_insert.clear();
    ij.clear();
    reverseMerge(level, to_insert, p, ij, j_suit);
    return (level);
}

void PmergeMe::merge()
{
    static const int j_suit[] = {1, 3, 5, 11, 21, 43, 85, 171,
        341, 683, 1365, 2731, 5461, 10923, 21845, 43691, 87381, 174763, 349525};
    int end_level = 0;
    std::vector<int> m;
    std::vector<int> p;
    std::vector<int> ij;
    if (this->first.size() < 2)
    {
        std::cout << "Error: you should put at least two numbers" << std::endl;
        return ;
    }
    std::cout << "Before: " << std::endl;
    printIt(this->first, -1);
    end_level = recursiveMerge(0, m, p, ij, j_suit);
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
