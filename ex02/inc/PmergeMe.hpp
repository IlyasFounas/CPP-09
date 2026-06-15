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

        // deque member functions
        void    d_blockSwap(int &pos1, int &pos2, int &level);
        void    d_binaryInsert(std::deque<int> &block,
                    std::deque<int> &m, int &nb_take);
        void    d_binarySearch(int &level,
                    std::deque<int> &m, std::deque<int> &p, std::deque<int> &ij);
        void    d_calculMPO(int &level, std::deque<int> &m, std::deque<int> &p, std::deque<int> &o);
        void    d_reverseMerge(int &level, std::deque<int> &m,
                    std::deque<int> &p, std::deque<int> &ij, const int *j_suit, std::deque<int> &o);
        int     d_recursiveMerge(int level, std::deque<int> &m,
                    std::deque<int> &p, std::deque<int> &ij, const int *j_suit, std::deque<int> &o);
        
        // vector member functions
        void    blockSwap(int &pos1, int &pos2, int &level);
        void    binaryInsert(std::vector<int> &block,
                    std::vector<int> &m, int &nb_take);
        void    binarySearch(int &level,
                    std::vector<int> &m, std::vector<int> &p, std::vector<int> &ij);
        void    calculMPO(int &level, std::vector<int> &m, std::vector<int> &p, std::vector<int> &o);
        void    reverseMerge(int &level, std::vector<int> &m,
                    std::vector<int> &p, std::vector<int> &ij, const int *j_suit, std::vector<int> &o);
        int     recursiveMerge(int level, std::vector<int> &m,
                    std::vector<int> &p, std::vector<int> &ij, const int *j_suit, std::vector<int> &o);


        void    merge();
        void    printIt(std::vector<int> &print_vec);
        void    printIt(std::deque<int> &print_vec);
        void    insert(char **argv, int &error);
} ;



// template<typename T>
// bool is_sorted(const std::vector<T>& vec) {
//     return std::adjacent_find(vec.begin(), vec.end(), std::greater<T>()) == vec.end();
// }
// template<typename T>
// bool is_sorted_d(const std::deque<T>& vec) {
//     return std::adjacent_find(vec.begin(), vec.end(), std::greater<T>()) == vec.end();
// }

#endif
