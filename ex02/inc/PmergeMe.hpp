#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <iostream>
#include <algorithm>
#include <vector>
#include <deque>

class PmergeMe {
    private:
        std::vector<int> first;
        std::deque<int> snd;

    public:
        PmergeMe();
        PmergeMe(const PmergeMe &cpy);
        PmergeMe &operator=(const PmergeMe &cpy);
        ~PmergeMe();

        // member functions
        void    blockSwap(int &pos1, int &pos2);
        void    binaryInsert(int &level, std::vector<int> &to_insert,
                    std::vector<int> &m, int &nb_take);
        void    binarySearch(int &level, std::vector<int> &to_insert, 
                    std::vector<int> &p, std::vector<int> &m, std::vector<int> &ij);
        void    calculMPO(int &level, std::vector<int> &m, std::vector<int> &p, std::vector<int> &o);
        void    reverseMerge(int &level, std::vector<int> &to_insert, std::vector<int> &m,
                    std::vector<int> &p, std::vector<int> &o, std::vector<int> &ij, const int *j_suit);
        int     recursiveMerge(int level, std::vector<int> &to_insert, std::vector<int> &m,
                    std::vector<int> &p, std::vector<int> &o, std::vector<int> &ij, const int *j_suit);
        void    merge();
        
        void    printIt(std::vector<int> &print_vec);
        void    insert(const std::string &s);
} ;

#endif