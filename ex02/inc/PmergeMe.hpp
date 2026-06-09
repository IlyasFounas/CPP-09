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
        void    d_binaryInsert(int &level, std::deque<int> &to_insert,
                    std::deque<int> &m, int &nb_take);
        void    d_binarySearch(int &level, std::deque<int> &to_insert, 
                    std::deque<int> &p, std::deque<int> &m, std::deque<int> &ij);
        void    d_calculMPO(int &level, std::deque<int> &m, std::deque<int> &p, std::deque<int> &o);
        void    d_reverseMerge(int &level, std::deque<int> &to_insert, std::deque<int> &m,
                    std::deque<int> &p, std::deque<int> &o, std::deque<int> &ij, const int *j_suit);
        int     d_recursiveMerge(int level, std::deque<int> &to_insert, std::deque<int> &m,
                    std::deque<int> &p, std::deque<int> &o, std::deque<int> &ij, const int *j_suit);
        
        // vector member functions
        void    blockSwap(int &pos1, int &pos2, int &level);
        // void    binaryInsert(int &level, std::vector<int> &to_insert,
        //             std::vector<int> &m, int &nb_take);
        void    binaryInsert(int &level, int &pos,
                    std::vector<int> &m, std::vector<int> &p, std::vector<int> &o, int &nb_take);
        // void    binarySearch(int &level, std::vector<int> &to_insert, 
        //             std::vector<int> &p, std::vector<int> &m, std::vector<int> &ij);
        void    binarySearch(int &level, std::vector<int> &to_insert, 
                    std::vector<int> &m, std::vector<int> &p, std::vector<int> &o, std::vector<int> &ij);
        void    calculMPO(int &level, std::vector<int> &m, std::vector<int> &p, std::vector<int> &o);
        void    reverseMerge(int &level, std::vector<int> &to_insert, std::vector<int> &m,
                    std::vector<int> &p, std::vector<int> &o, std::vector<int> &ij, const int *j_suit);
        int     recursiveMerge(int level, std::vector<int> &to_insert, std::vector<int> &m,
                    std::vector<int> &p, std::vector<int> &o, std::vector<int> &ij, const int *j_suit);


        void    blockRotate(int &nb_take, std::vector<int> &o, const int &pos1, const int &pos2);
        void    merge();
        void    printIt(std::vector<int> &print_vec);
        void    printIt(std::deque<int> &print_vec);
        void    insert(char **argv, int &error);
} ;

#endif
