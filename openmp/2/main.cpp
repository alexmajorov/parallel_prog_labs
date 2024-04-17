#include <iostream>
#include <iomanip>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <chrono>
#include <omp.h>


#define NUM_THREADS 3
#define LIST_LEN 10000000
#define EPS 10e-9

class node
{
public:
    double data;
    node *next;
    node *prev;
    
    node(double data_)
    {
        data = data_;
        next = nullptr;
        prev = nullptr;
    }

    void print(void)
    {
        std::cout << data << " ";
        node *current = next;

        while (current != nullptr) 
        {
            std::cout << std::setprecision(3) \
            << current->data << " ";
            current = current->next;
        }
        return;
    }
};

class linked_list
{
public:
    linked_list(void)
    {
        head = nullptr;
        tail = nullptr;
    }

    linked_list(node *n)
    {
        head = n;
        node *cur = head;
        node *pr = head;
        while (cur)
        {
            pr = cur;
            cur = cur->next;
        }
        tail = pr;
    }

    ~linked_list(void) 
    {
        while (head != nullptr) pop_front();
    }

    double front(void)
    {
        if (head == nullptr) return 0;
        else return head->data;
    }

    node *push_back(double data) 
    {
        node *ptr = new node(data);
        ptr->prev = tail;
        if (tail != nullptr) tail->next = ptr;
        if (head == nullptr) head = ptr;
        tail = ptr;
        return ptr;
    }

    void pop_front(void) 
    {
        if (head == nullptr) return;
        node *ptr = head->next;
        if (ptr != nullptr) ptr->prev = nullptr;
        else tail = nullptr;
        delete head;
        head = ptr;
        return;
    }

    void print(void)
    {
        node *current = head;

        while (current != nullptr) 
        {
            std::cout << current->data << " ";
            current = current->next;
        }

        std::cout << "\n";
        return;
    }

    std::vector<node *> split(int k)
    {
        node *curr = head;
        int n = 0;
        while (curr)
        {
            curr = curr->next;
            n++;
        }
        auto [q, r] = div(n, k);
        std::vector<int> iLen(k, q);
        for (int i = 0; i < r; ++i)
            ++iLen[i];
        std::vector<node *> ans(k);
        curr = head;
        for (int i = 0; i < k; ++i)
        {
            ans[i] = curr;
            int j = 0;
            node *pr = nullptr;
            while(j < iLen[i])
            {
                pr = curr;
                curr = curr->next;
                ++j;
            }
            if (pr)
                pr->next = nullptr;
        }

        return ans;
    }

    void divide_by_first(void)
    {
        if (head == nullptr) return;
        double scale = this->front();
        node *cur = head;
        if (std::abs(scale) > EPS)
        {
            while (cur)
            {
                cur->data /= scale;
                cur = cur->next;
            }
        }
        return;
    }

private:
    node *head;
    node *tail;
};


int main(void)
{
    std::ios_base::sync_with_stdio(0);
    std::cin.tie(0); std::cout.tie(0);
    srand((unsigned int)time(0));

    linked_list l;
    for (int i = 0; i < LIST_LEN; ++i)
        l.push_back(rand() % 100 - 50);

    /*
    std::cout << "Before: ";
    l.print();
    */

    std::vector<node *> lvec;
    lvec = l.split(NUM_THREADS);

    auto beg = std::chrono::high_resolution_clock::now();
    double scale = l.front();
    if (std::abs(scale) > EPS)
    {
        omp_set_num_threads(NUM_THREADS);
        #pragma omp parallel shared(scale)
        {
            int cur_thread = omp_get_thread_num();
            node *cur = lvec[cur_thread];
            while (cur != nullptr)
            {
                cur->data /= scale;
                cur = cur->next;
            }   
        }    
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> ms = end - beg;
    std::cout << "         list len: " << LIST_LEN << std::endl;
    std::cout << "number of threads: " << NUM_THREADS << std::endl;
    std::cout << "      openmp time: " << ms.count() << "ms" << std::endl;

    linked_list l2;
    for (int i = 0; i < LIST_LEN; ++i)
        l2.push_back(rand() % 100 - 50);
    
    beg = std::chrono::high_resolution_clock::now();
    l2.divide_by_first();
    end = std::chrono::high_resolution_clock::now();
    ms = end - beg;
    std::cout << "        cons time: " << ms.count() << "ms" << std::endl;

    /*
    std::cout << " After: ";
    for (auto i: lvec)
        i->print();
    std::cout << "\n";
    */

    return 0;
}