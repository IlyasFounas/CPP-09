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

// deque member functions
void PmergeMe::d_blockSwap(int &pos1, int &pos2, int &level)
{
    int block_size = (1 << level);

    std::swap_ranges(snd.begin() + (pos1 - block_size + 1),
                 snd.begin() + (pos1 + 1),
                 snd.begin() + (pos2 - block_size + 1));
}

void PmergeMe::d_binaryInsert(std::deque<int> &block,
    std::deque<int> &m, int &nb_take)
{
    int compare = block.at(nb_take - 1);
    int left    = 0;
    int right   = (int)m.size() / nb_take;

    while (left < right)
    {
        int mid = (left + right) / 2;
        if (m.at((mid + 1) * nb_take - 1) > compare)
            right = mid;
        else
            left = mid + 1;
    }
    m.insert(m.begin() + left * nb_take, block.begin(), block.end());
}

void PmergeMe::d_binarySearch(int &level,
    std::deque<int> &m, std::deque<int> &p, std::deque<int> &ij)
{
    int nb_take = (level == 0) ? 1 : (1 << level);

    for (std::deque<int>::iterator itj = ij.begin(); itj != ij.end(); itj++)
    {
        int block_start = (*itj - 1) * nb_take;
        std::deque<int> block(p.begin() + block_start,
                               p.begin() + block_start + nb_take);
        d_binaryInsert(block, m, nb_take);
    }
}

void PmergeMe::d_calculMPO(int &level, std::deque<int> &m,
    std::deque<int> &p, std::deque<int> &o)
{
    int i = 0;
    int pos = 0;
    int nb_take = 1;
    std::deque<int>::iterator ite = this->snd.end();
    
    if (level != 0)
        nb_take = (1 << level);
    // std::cout << "LEVEL= " << level << std::endl;
    for (std::deque<int>::iterator it = this->snd.begin();
        it != ite; it++)
    {
        if (level != 0)
        {
            if (i + nb_take - 1 >= (int)this->snd.size())
            {    
                o.insert(o.end(), it, this->snd.end());
                i += (int)this->snd.size() - i;
                return ;
            }
            else if (i == 0 || i % nb_take == 0)
            {
                if (i == 0 || pos % 2 == 0)
                    p.insert(p.end(), it, it + (nb_take));
                else
                    m.insert(m.end(), it, it + (nb_take));
                pos++;
                i += nb_take - 1;
                it = this->snd.begin() + i;
            }
            i++;
        }
        else if (level == 0)
        {
            if (i == 0 || i % 2 == 0)
                p.push_back(*it);
            else
                m.push_back(*it);
            i++;
        }
    }
}

void PmergeMe::d_reverseMerge(int &level, std::deque<int> &m,
    std::deque<int> &p, std::deque<int> &ij, const int *j_suit, std::deque<int> &o)
{
    int j = 0;
    int clast_insert = 0;
    int last_insert = 0;
    int psize = 0;
    int i;
    
    d_calculMPO(level, m, p, o);
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
    // std::cout << "la pend = ";
    // printIt(p);
    // std::cout << "la main = ";
    // printIt(m);
    // std::cout << "les odds = ";
    // printIt(o);
    // std::cout << "ij = ";
    // printIt(ij);
    d_binarySearch(level, m, p, ij);
    // std::cout << "vec = ";
    // printIt(snd);
}

int PmergeMe::d_recursiveMerge(int level, std::deque<int> &m,
    std::deque<int> &p, std::deque<int> &ij, const int *j_suit, std::deque<int> &o)
{
    int tmp1 = 0;
    int tmp2 = 0;
    int pos1 = 0;
    int pos2 = 0;

    for (size_t i = 0; i < this->snd.size(); i++)
    {
        if ((level == 0 && (i % 2 == 0 || i == 0)))
        {
            tmp1 = this->snd[i];
            pos1 = i;
        }
        else if (level == 0)
        {
            tmp2 = this->snd[i];
            pos2 = i;
            if (tmp1 > tmp2)
                d_blockSwap(pos1, pos2, level);
            pos1 = 0;
            pos2 = 0;
        }
        if (level > 0 && (i + 1) % (1 << level) == 0)
        {
            if (pos1 == 0)
            {
                tmp1 = this->snd[i];
                pos1 = i;
            }
            else 
            {
                tmp2 = this->snd[i];
                pos2 = i;
                if (tmp1 > tmp2)
                    d_blockSwap(pos1, pos2, level);
                pos1 = 0;
                pos2 = 0;
            }
        }
    }
    if ((level == 0 && this->snd.size() > 2)
        || (double)(this->snd.size()) / 2 >= (double)(2 << level))
        d_recursiveMerge(level + 1, m, p, ij, j_suit, o);
    m.clear();
    p.clear();
    ij.clear();
    o.clear();
    d_reverseMerge(level, m, p, ij, j_suit, o);
    this->snd = m;
    this->snd.insert(this->snd.end(), o.begin(), o.end());
    return (level);
}

// vector member functions
void PmergeMe::blockSwap(int &pos1, int &pos2, int &level)
{
    int block_size = (1 << level);

    std::swap_ranges(first.begin() + (pos1 - block_size + 1),
                 first.begin() + (pos1 + 1),
                 first.begin() + (pos2 - block_size + 1));
}

void PmergeMe::binaryInsert(std::vector<int> &block,
    std::vector<int> &m, int &nb_take)
{
    int compare = block.at(nb_take - 1);
    int left    = 0;
    int right   = (int)m.size() / nb_take;

    while (left < right)
    {
        int mid = (left + right) / 2;
        if (m.at((mid + 1) * nb_take - 1) > compare)
            right = mid;
        else
            left = mid + 1;
    }
    m.insert(m.begin() + left * nb_take, block.begin(), block.end());
}

void PmergeMe::binarySearch(int &level,
    std::vector<int> &m, std::vector<int> &p, std::vector<int> &ij)
{
    int nb_take = (level == 0) ? 1 : (1 << level);

    for (std::vector<int>::iterator itj = ij.begin(); itj != ij.end(); itj++)
    {
        int block_start = (*itj - 1) * nb_take;
        std::vector<int> block(p.begin() + block_start,
                               p.begin() + block_start + nb_take);
        binaryInsert(block, m, nb_take);
    }
}

void PmergeMe::calculMPO(int &level, std::vector<int> &m,
    std::vector<int> &p, std::vector<int> &o)
{
    int i = 0;
    int pos = 0;
    int nb_take = 1;
    std::vector<int>::iterator ite = this->first.end();
    
    if (level != 0)
        nb_take = (1 << level);
    // std::cout << "LEVEL= " << level << std::endl;
    for (std::vector<int>::iterator it = this->first.begin();
        it != ite; it++)
    {
        if (level != 0)
        {
            if (i + nb_take - 1 >= (int)this->first.size())
            {    
                o.insert(o.end(), it, this->first.end());
                i += (int)this->first.size() - i;
                return ;
            }
            else if (i == 0 || i % nb_take == 0)
            {
                if (i == 0 || pos % 2 == 0)
                    p.insert(p.end(), it, it + (nb_take));
                else
                    m.insert(m.end(), it, it + (nb_take));
                pos++;
                i += nb_take - 1;
                it = this->first.begin() + i;
            }
            i++;
        }
        else if (level == 0)
        {
            if (i == 0 || i % 2 == 0)
                p.push_back(*it);
            else
                m.push_back(*it);
            i++;
        }
    }
}

void PmergeMe::reverseMerge(int &level, std::vector<int> &m,
    std::vector<int> &p, std::vector<int> &ij, const int *j_suit, std::vector<int> &o)
{
    int j = 0;
    int clast_insert = 0;
    int last_insert = 0;
    int psize = 0;
    int i;
    
    calculMPO(level, m, p, o);
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
    // std::cout << "la pend = ";
    // printIt(p);
    // std::cout << "la main = ";
    // printIt(m);
    // std::cout << "les odds = ";
    // printIt(o);
    // std::cout << "ij = ";
    // printIt(ij);
    binarySearch(level, m, p, ij);
    // std::cout << "vec = ";
    // printIt(first);
}

int PmergeMe::recursiveMerge(int level, std::vector<int> &m,
    std::vector<int> &p, std::vector<int> &ij, const int *j_suit, std::vector<int> &o)
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
                blockSwap(pos1, pos2, level);
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
                    blockSwap(pos1, pos2, level);
                pos1 = 0;
                pos2 = 0;
            }
        }
    }
    if ((level == 0 && this->first.size() > 2)
        || (double)(this->first.size()) / 2 >= (double)(2 << level))
        recursiveMerge(level + 1, m, p, ij, j_suit, o);
    m.clear();
    p.clear();
    ij.clear();
    o.clear();
    reverseMerge(level, m, p, ij, j_suit, o);
    this->first = m;
    this->first.insert(this->first.end(), o.begin(), o.end());
    return (level);
}

#include <ctime>

void PmergeMe::merge()
{
    static const int j_suit[] = {1, 3, 5, 11, 21, 43, 85, 171,
        341, 683, 1365, 2731, 5461, 10923, 21845, 43691, 87381, 174763, 349525};
    std::vector<int> m;
    std::vector<int> p;
    std::vector<int> ij;
    std::vector<int> o;

    std::deque<int> m2;
    std::deque<int> p2;
    std::deque<int> ij2;
    std::deque<int> o2;
    double duration;
    clock_t start;
    clock_t end;
    double duration2 = 0;
    clock_t start2;
    clock_t end2;

    if (this->first.size() < 2 && this->snd.size() < 2)
    {
        std::cout << "Error: you should put at least two numbers" << std::endl;
        return ;
    }
    std::cout << "Before: " << std::endl;
    printIt(this->first);

    start = clock();
    recursiveMerge(0, m, p, ij, j_suit, o);
    end = clock();
    duration = (double)(end - start) * 1000000 / CLOCKS_PER_SEC;

    start2 = clock();
    d_recursiveMerge(0, m2, p2, ij2, j_suit, o2);
    end2 = clock();
    duration2 = (double)(end2 - start2) * 1000000 / CLOCKS_PER_SEC;

    std::cout << std::endl << std::endl;
    std::cout << "After: " << std::endl;
    printIt(this->first);
    std::cout << "Time to process a range of " 
    << this->first.size() << " elements with PmergeMe: " << duration / 1000 << " ms" << std::endl;
    std::cout << "Time to process a range of " 
    << this->snd.size() << " elements with PmergeMe: " << duration2 / 1000 << " ms" << std::endl;
    // std::cout << "is sorted ? let's see it = " << is_sorted(this->first) << std::endl;
    // std::cout << "is sorted ? let's see it = " << is_sorted_d(this->snd) << std::endl;
}

void PmergeMe::printIt(std::vector<int> &print_vec)
{
    for (std::vector<int>::iterator it = print_vec.begin();
        it != print_vec.end(); it++)
    {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
}

void PmergeMe::printIt(std::deque<int> &print_vec)
{
    for (std::deque<int>::iterator it = print_vec.begin();
        it != print_vec.end(); it++)
    {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
}

// TODO IMPROVE THIS ONE
void PmergeMe::insert(char **argv, int &error) 
{
    long int nb = 0;

    for (size_t j = 1; argv[j]; j++)
    {
        nb = std::atol(argv[j]);
        if (nb > 2147483647)
        {
            std::cout << "Overflow" << std::endl;
            error = 1;
            return ;
        }
        if (nb < 0)
        {
            std::cout << "Error" << std::endl;
            error = 1;
            return ;
        }
        first.push_back(nb);
        snd.push_back(nb);
        for (size_t i = 0; argv[j][i]; i++)
        {
            if (!isdigit(argv[j][i]) || argv[j][i] == ' ')
            {
                std::cout << "Error" << std::endl;
                error = 1;
                return ;
            }
        }
    }
}
