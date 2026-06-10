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
        void    d_blockRotate(int &nb_take, const int &dst, const int &src);
        void    d_updateMP(std::deque<int> &m, std::deque<int> &p,
                    const int &dst, int &src, int &nb_take);
        void    d_binaryInsert(int &pos, std::deque<int> &m,
                    std::deque<int> &p, int &nb_take);
        void    d_binarySearch(int &level,
                    std::deque<int> &m, std::deque<int> &p, std::deque<int> &ij);
        void    d_calculMPO(int &level, std::deque<int> &m, std::deque<int> &p);
        void    d_reverseMerge(int &level, std::deque<int> &m,
                    std::deque<int> &p, std::deque<int> &ij, const int *j_suit);
        int     d_recursiveMerge(int level, std::deque<int> &m,
                    std::deque<int> &p, std::deque<int> &ij, const int *j_suit);
        
        // vector member functions
        void    blockSwap(int &pos1, int &pos2, int &level);
        void    blockRotate(int &nb_take, const int &dst, const int &src);
        void    updateMP(std::vector<int> &m, std::vector<int> &p,
                    const int &dst, int &src, int &nb_take);
        void    binaryInsert(int &pos, std::vector<int> &m,
                    std::vector<int> &p, int &nb_take);
        void    binarySearch(int &level,
                    std::vector<int> &m, std::vector<int> &p, std::vector<int> &ij);
        void    calculMPO(int &level, std::vector<int> &m, std::vector<int> &p);
        void    reverseMerge(int &level, std::vector<int> &m,
                    std::vector<int> &p, std::vector<int> &ij, const int *j_suit);
        int     recursiveMerge(int level, std::vector<int> &m,
                    std::vector<int> &p, std::vector<int> &ij, const int *j_suit);


        void    merge();
        void    printIt(std::vector<int> &print_vec);
        void    printIt(std::deque<int> &print_vec);
        void    insert(char **argv, int &error);
} ;

#endif
