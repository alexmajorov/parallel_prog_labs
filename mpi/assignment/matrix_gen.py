import numpy
import random 

with open("in.txt", "w") as file:
    n = 1000
    rank = random.randint(0, n)

    indecies = set()
    while len(indecies) != rank:
        indecies.add(random.randint(0, n))

    mat = numpy.tile(numpy.random.randint(-50, 50, (1, n)) + numpy.random.random((1, n)), (n, 1))
    
    for i in range(n):
        if i in indecies:
            mat[i] = numpy.random.randint(-50, 50, (1, n)) + numpy.random.random((1, n))
    
    #mat = numpy.transpose(mat)
    print("rank:", numpy.linalg.matrix_rank(mat))
    numpy.savetxt(file, mat, fmt="%.2f")