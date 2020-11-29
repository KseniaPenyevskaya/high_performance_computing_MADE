#include <mpi.h>
#include <cstdlib>
#include "time.h"

int getSeed(int rank) {
    int seed = (unsigned)time(NULL);
    seed = (seed & 0xFFFFFFF0) | (rank + 1);
    return seed;
}

bool inArray(int element, int* array, int count){
    for (int i = 0; i < count; ++i) {
        if (array[i] == element) return true;
    }
    return false;
}

int main(int argc, char ** argv){
    int rank;
    int size;
    int seed;
    char name;
    int count = 0;

    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int *order = new int[size];
    char *names = new char[size];

    name = rank - 'a';
    seed = getSeed(rank);
    srand(seed);
    int recieverRank = 0;

    if (rank == 0){
        order[rank] = rank;
        names[rank] = name;
        count += 1;
        while(recieverRank == rank){
            recieverRank = rand() % size;
        }
        MPI_Ssend(&count, 1, MPI_INT, recieverRank, 0, MPI_COMM_WORLD);
        MPI_Ssend(order, size, MPI_INT, recieverRank, 0, MPI_COMM_WORLD);
        MPI_Ssend(names, size, MPI_CHAR, recieverRank, 0, MPI_COMM_WORLD);
    }
    else {
        MPI_Recv(&count, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        MPI_Recv(order, size, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        MPI_Recv(names, size, MPI_CHAR, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        order[count] = rank;
        names[count] = name;
        count += 1;

        if (count < size){
            int recieverRank = rand() % size;
            while(inArray(recieverRank, order, count)){
                recieverRank = rand() % size;
            }
            MPI_Ssend(&count, 1, MPI_INT, recieverRank, 0, MPI_COMM_WORLD);
            MPI_Ssend(order, count, MPI_INT, recieverRank, 0, MPI_COMM_WORLD);
            MPI_Ssend(names, count, MPI_CHAR, recieverRank, 0, MPI_COMM_WORLD);
        }
    }
    MPI_Finalize();
    delete[] order;
    delete[] names;
    return 0;
}


