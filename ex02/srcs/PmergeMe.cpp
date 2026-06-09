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

void PmergeMe::d_binaryInsert(int &level, std::deque<int> &to_insert,
    std::deque<int> &m, int &nb_take)
{
    size_t i = 0;
    int j = 1;
    int compare = *to_insert.rbegin();

    if (to_insert.size() <= 0)
        return ;
    // specific case
    if (m.size() == 1)
    {
        if (compare < m.at(0))
            m.insert(m.begin(), to_insert.begin(), to_insert.end());
        else
            m.insert(m.begin() + 1, to_insert.begin(), to_insert.end());
        return ;
    }
    while (i < m.size())
    {
        if (((i != 0 && (i + 1) % (nb_take) == 0) && level != 0) || level == 0)
        {
            if (compare < m.at(i) && level > 0)
            {
                m.insert(m.begin() + i - (nb_take - 1), to_insert.begin(), to_insert.end());
                return ;
            }
            else if (compare < m.at(i) && level == 0)
            {
                m.insert(m.begin() + i - (nb_take - 1), to_insert.begin(), to_insert.end());
                return ;
            }
            else if ((int)(m.size() - (i + 1)) < nb_take)
            {
                m.insert(m.begin() + i + 1, to_insert.begin(), to_insert.end());
                return ;
            }
            j++;
        }
        i++;
    }
}

void PmergeMe::d_binarySearch(int &level, std::deque<int> &to_insert, 
    std::deque<int> &p, std::deque<int> &m, std::deque<int> &ij)
{
    int i;
    int j;
    int nb_take;

    if (level == 0)
        nb_take = 1; 
    else
        nb_take = (1 << level);
    for (std::deque<int>::iterator itj = ij.begin();
        itj != ij.end(); itj++)
    {
        i = 0;
        j = 1;
        for (std::deque<int>::iterator it = p.begin();
            it != p.end(); it++)
        {
            if (j == *itj)
            {
                to_insert.push_back(*it);
            }
            if ((i != 0 && (i + 1) % nb_take == 0) || level == 0)
                j++;
            i++;
        }
        d_binaryInsert(level, to_insert, m, nb_take);
        to_insert.clear();
    }
}

void PmergeMe::d_calculMPO(int &level, std::deque<int> &m, std::deque<int> &p, std::deque<int> &o)
{
    size_t i = 0;
    int pos = 1;
    int nb_take = 0;

    if (level == 0)
        nb_take = 1; 
    else
        nb_take = (1 << level);
    while (i < this->snd.size())
    {
        if (level > 0 && (i + 1) % (1 << level) == 0)
        {
            if (pos % 2 != 0)
            {
                p.insert(p.end(), (this->snd.begin() + (i - (nb_take - 1))),
                    (this->snd.begin() + i + 1));
            }
            else 
            {
                m.insert(m.end(), (this->snd.begin() + (i - (nb_take - 1))),
                    (this->snd.begin() + i + 1));
            }
            pos++;
        }
        else if ((int)(this->snd.size() / nb_take) * nb_take <= (int)i && level > 0)
        {            
            if (m.size() == 0)
                m.push_back(this->snd.at(i));
            else   
                o.push_back(this->snd.at(i));
        }
        else if (level == 0)
        {
            if (i == 0 || i % 2 == 0)
                p.push_back(this->snd.at(i));
            else 
                m.push_back(this->snd.at(i));
        }
        i++;
    }
}

void PmergeMe::d_reverseMerge(int &level, std::deque<int> &to_insert, std::deque<int> &m,
    std::deque<int> &p, std::deque<int> &o, std::deque<int> &ij, const int *j_suit)
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
    d_binarySearch(level, to_insert, p, m, ij);
}

int PmergeMe::d_recursiveMerge(int level, std::deque<int> &to_insert, std::deque<int> &m,
    std::deque<int> &p, std::deque<int> &o, std::deque<int> &ij, const int *j_suit)
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
        d_recursiveMerge(level + 1, to_insert, m, p, o, ij, j_suit);
    m.clear();
    p.clear();
    o.clear();
    to_insert.clear();
    ij.clear();
    d_reverseMerge(level, to_insert, m, p, o, ij, j_suit);
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

// |1523 452| |465 8563| |452 1253| |8 5232| 
void PmergeMe::blockRotate(int &nb_take, const int &pos1, const int &pos2)
{
    std::cout << "check here = "<< pos1 
    << " " << pos2 << " " << nb_take << std::endl;
    std::rotate(first.begin() + pos1,
                first.begin() + pos2,
                first.begin() + pos2 + nb_take);
    // std::cout << "post rotate print = " << std::endl;
    // printIt(this->first); 
}

void PmergeMe::updateMP(std::vector<int> &m, std::vector<int> &p,
    int &dst, int &src, int &nb_take)
{
    for (int i = 0; i < (int)m.size(); i++)
        if (m.at(i) >= dst && m.at(i) < src)
            m.at(i) += nb_take;
    for (int i = 0; i < (int)p.size(); i++)
        if (p.at(i) >= dst && p.at(i) < src)
            p.at(i) += nb_take;
}

void PmergeMe::binaryInsert(int &pos, std::vector<int> &m,
    std::vector<int> &p, int &nb_take, int &level)
{
    int left    = 0;
    int right   = (int)m.size();
    int src     = p.at(pos - 1);
    int compare = this->first.at(src + nb_take - 1);
    int mid = 0;
    int mid_idx = 0;

    std::cout << "nb_take " << nb_take << " compare = " << this->first.at(src + (nb_take - 1)) << std::endl;
    if (src == 0)
        return ;
    std::cout << "compare = " << this->first.at(src + (nb_take - 1)) << std::endl;
    if (level == 0 && m.size() > 0)
    {
        if (compare < this->first.at(m.at(0) - 1))  // compare avec l'élément juste avant m[0]
        {
            blockRotate(nb_take, 0, src);
            updateMP(m, p, m.at(0), src, nb_take);
            return ;
        }
    }
    while (left < right)
    {
        mid = (left + right) / 2;
        mid_idx = m.at(mid);

        if (this->first.at(mid_idx + nb_take - 1) > compare)
        {
            std::cout << "actual val = " << this->first.at(mid_idx + nb_take - 1) << std::endl;
            right = mid;
        }
        else
            left = mid + 1;
    }

    // already at the good place
    if (left == (int)m.size())
        return ;

    int dst = m.at(left);
    blockRotate(nb_take, dst, src);

    // update the old index in m and p
    updateMP(m, p, dst, src, nb_take);
}

void PmergeMe::binarySearch(int &level,
    std::vector<int> &m, std::vector<int> &p, std::vector<int> &ij)
{
    int nb_take;

    if (level == 0)
        nb_take = 1; 
    else
        nb_take = (1 << level);
    for (std::vector<int>::iterator itj = ij.begin();
        itj != ij.end(); itj++)
    {
        binaryInsert(*itj, m, p, nb_take, level);
    }
}

void PmergeMe::calculMPO(int &level, std::vector<int> &m, std::vector<int> &p)
{
    size_t i = 0;
    int pos = 1;
    int nb_take = 0;

    std::cout << "LEVEL " << level << std::endl;
    if (level == 0)
        nb_take = 1; 
    else
        nb_take = (1 << level);
    while (i < this->first.size())
    {
        if (level > 0 && (i + 1) % (1 << level) == 0)
        {
            if (pos % 2 != 0)
                p.push_back(i - (nb_take - 1));
            else 
                m.push_back(i - (nb_take - 1));
            pos++;
        }
        else if ((int)(this->first.size() / nb_take) * nb_take <= (int)i && level > 0)
        {
            if (m.size() == 0)
                m.push_back(i);
        }
        else if (level == 0)
        {
            if (i == 0 || i % 2 == 0)
                p.push_back(i);
            else 
                m.push_back(i);
        }
        i++;
    }
    std::cout << "print first list = ";
    printIt(this->first);
    std::cout << "main list = ";
    printIt(m);
    std::cout << "pends list = ";
    printIt(p);
    std::cout << std::endl;
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
void PmergeMe::reverseMerge(int &level, std::vector<int> &m,
    std::vector<int> &p, std::vector<int> &ij, const int *j_suit)
{
    int j = 0;
    int clast_insert = 0;
    int last_insert = 0;
    int psize = 0;
    int i;
    
    calculMPO(level, m, p);
    // calcul the jacobsthal index
    psize = (int)p.size();
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
    binarySearch(level, m, p, ij);
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
int PmergeMe::recursiveMerge(int level, std::vector<int> &m,
    std::vector<int> &p, std::vector<int> &ij, const int *j_suit)
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
        recursiveMerge(level + 1, m, p, ij, j_suit);
    m.clear();
    p.clear();
    ij.clear();
    reverseMerge(level, m, p, ij, j_suit);
    return (level);
}

#include <ctime>

void PmergeMe::merge()
{
    static const int j_suit[] = {1, 3, 5, 11, 21, 43, 85, 171,
        341, 683, 1365, 2731, 5461, 10923, 21845, 43691, 87381, 174763, 349525};
    std::vector<int> to_insert;
    std::vector<int> m;
    std::vector<int> p;
    std::vector<int> o;
    std::vector<int> ij;

    // std::deque<int> to_insert2;
    // std::deque<int> m2;
    // std::deque<int> p2;
    // std::deque<int> o2;
    // std::deque<int> ij2;
    double duration;
    // double duration2;
    clock_t start;
    clock_t end;
    // clock_t start2;
    // clock_t end2;

    if (this->first.size() < 2 && this->snd.size() < 2)
    {
        std::cout << "Error: you should put at least two numbers" << std::endl;
        return ;
    }
    std::cout << "Before: " << std::endl;
    printIt(this->first);
    printIt(this->snd);

    start = clock();
    recursiveMerge(0, to_insert, m, p, j_suit);
    end = clock();
    duration = (double)(end - start) * 1000000 / CLOCKS_PER_SEC;

    // start2 = clock();
    // d_recursiveMerge(0, to_insert2, m2, p2, o2, ij2, j_suit);
    // end2 = clock();
    // duration2 = (double)(end2 - start2) * 1000000 / CLOCKS_PER_SEC;

    std::cout << "After: " << std::endl;
    printIt(this->first);
    printIt(this->snd);
    std::cout << "Time to process a range of " 
    << this->first.size() << " elements with PmergeMe: " << duration / 1000 << " ms" << std::endl;
    // std::cout << "Time to process a range of " 
    // << this->snd.size() << " elements with PmergeMe: " << duration2 / 1000 << " ms" << std::endl;
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
