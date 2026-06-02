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
        void blockSwap(int &pos1, int &pos2);
        int recursiveMerge(int level);
        void calculMP(int &level, std::vector<int> &m, std::vector<int> &p);
        int reverseRecursiveMerge(int level, const int *j_suit);
        void merge();
        void printIt(std::vector<int> &print_vec, const int &level);
        void insert(const std::string &s);
} ;

#endif