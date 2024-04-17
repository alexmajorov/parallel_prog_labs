#include <iostream>
#include <set>
#include <fstream>
#include <cstdint>
#include <utility>
#include <mpi.h>
#include <vector>
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

    unsigned long long beg, end;
    int time_flag = true;

    int n = N;
    std::ifstream fin(argv[1]);
    std::set<std::vector<uint64_t>> groups;

    double matrix[N][N];
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j) 
        {
            fin >> matrix[i][j];
            if (std::abs(matrix[i][j]) < EPS)
            {
                if (matrix[i][j] < 0.0) matrix[i][j] = -EPS;
                else                    matrix[i][j] =  EPS;
            }
        }
    
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);    
    
    if (N % size != 0) exit(1);

    if (rank == 0 && time_flag)
    {
        beg = nanoseconds_now();
        time_flag = false;
    }

    int lines_per_process = N / size;
    int vals_per_line = N / 64 + ((N % 64) != 0);
    
    std::uint64_t tmp[lines_per_process * vals_per_line] = {0};

    for (int i = rank; i < n; i += size)
        for (int j = 0; j < n; ++j)
        {
            int flag = true;
            double scale = matrix[i][0] / matrix[j][0];
            for (int k = 1; k < n; ++k)
            {
                if (std::abs(matrix[i][k] / matrix[j][k] - scale) > EPS)
                {
                    flag = false;
                    break;
                }
            }
            if (!flag)
            {
                std::uint64_t one = 1;
                tmp[i / size * vals_per_line + j / 64] |= (one << (j % 64));
            }
        }
   
    std::uint64_t tmps[N * vals_per_line];
    int vals_per_process = lines_per_process * vals_per_line; 
    MPI_Gather(tmp, vals_per_process, MPI_UINT64_T, tmps, vals_per_process, MPI_UINT64_T, 0, MPI_COMM_WORLD);
    
    if (rank == 0)
    {      
        for (int i = 0; i < N; ++i)
        {
            std::vector<uint64_t> vec(&tmps[i * vals_per_line], &tmps[(i + 1) * vals_per_line]);
            groups.insert(vec);
        }
        
        int ans = groups.size();
        end = nanoseconds_now();

        std::ofstream fout(argv[2]);
        fout << rank;

        std::cout << "Parallel:" << "\n";
        std::cout << "rank: " << ans << "\n";
        std::cout << "time: " << (end - beg) / 1000000ULL << " ms" << "\n";
    }

    MPI_Finalize();
    return 0;
}