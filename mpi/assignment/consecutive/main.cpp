#include <iostream>
#include <vector>
#include <fstream>
#include <sys/time.h>
#include <time.h>

#define N 1000
#define EPS 1e-9

unsigned long long nanoseconds_now(void)
{
    struct timespec val;
    
    clock_gettime(CLOCK_MONOTONIC_RAW, &val);
    
    return val.tv_sec * 1000000000000 + val.tv_nsec;
}

int main(int argc, char *argv[]) 
{
    if (argc != 3) return 1;

    long long unsigned beg, end;
    int n = N;
    std::ifstream fin(argv[1]);

    double matrix[N][N];
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j) 
            fin >> matrix[i][j];

    beg = nanoseconds_now();
    int rank = n;
    std::vector<char> line_used(N, 0);
    for (int i = 0; i < n; ++i) 
    {
        int j;
        for (j = 0; j < n; ++j)
            if (!line_used[j] && std::abs(matrix[j][i]) > EPS)
                break;
        if (j == n)
            --rank;
        else 
        {
            line_used[j] = true;
            for (int p = i + 1; p < n; ++p)
                matrix[j][p] /= matrix[j][i];
            for (int k = 0; k < n; ++k)
                if (k != j && std::abs(matrix[k][i]) > EPS)
                    for (int p = i + 1; p < n; ++p)
                        matrix[k][p] -= matrix[j][p] * matrix[k][i];
        }
    }
    end = nanoseconds_now();

    std::ofstream fout(argv[2]);
    fout << rank;

    std::cout << "Сonsecutive:" << "\n";
    std::cout << "rank: " << rank << "\n";
    std::cout << "time: " << (end - beg) / 1000000ULL << " ms" << "\n";
    
    return 0;
}   
