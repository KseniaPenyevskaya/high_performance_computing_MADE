#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <mpi.h>

typedef struct {
    int nx;
    int *u0;
    int *u1;
    int steps;
} life_t;

void life_init(const char *path, life_t *l);
void life_step(life_t *l);
void life_save_to_file(int step, life_t *l);

int main(int argc, char **argv){
    
    MPI_Init(&argc, &argv);

    
    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    //Создание декартовой топологии
    int dims = 0;
    int periods = 1;
    MPI_Dims_create(size, 1, &dims);
    MPI_Comm cart1d;
    MPI_Cart_create(MPI_COMM_WORLD, 1, &dims, &periods, 0, &cart1d);
    
    int rank;
    int coords;
    MPI_Comm_rank(cart1d, &rank);
    MPI_Comm_size(cart1d, &size);
    MPI_Status status;
    
    life_t l;
    if (rank == 0) {
        life_init(argv[1], &l);
        life_save_to_file(0, &l);
    }

    MPI_Bcast(&l.nx, 1, MPI_INT, 0, cart1d);
    MPI_Bcast(&l.steps, 1, MPI_INT, 0, cart1d);
    
    int tail = l.nx % dims;

    int blocksize;
    int ghost = 1;

    blocksize = l.nx / dims + 2 * ghost;
    if (rank == dims - 1) {
        blocksize += tail;
    }

    life_t ll;
    ll.nx = blocksize;
    ll.u0 = (int*)calloc(ll.nx, sizeof(int));
    ll.u1 = (int*)calloc(ll.nx, sizeof(int));
    
    int *beginnings;
    int *counts;

    beginnings = (int*)calloc(size, sizeof(int));
    counts = (int*)calloc(size, sizeof(int));

    int shift = l.nx / dims;

    for(int i = 0; i < size; ++i){
        beginnings[i] = i * shift;
        counts[i] = shift;
    }
    counts[size - 1] += tail;
    
    for (int i = 0; i < l.steps; ++i){
        MPI_Scatterv(&l.u0[0], &counts[0], &beginnings[0], MPI_INT, &ll.u0[ghost], ll.nx - 2 * ghost, MPI_INT, 0, cart1d);
        MPI_Scatterv(&l.u1[0], &counts[0], &beginnings[0], MPI_INT, &ll.u1[ghost], ll.nx - 2 * ghost, MPI_INT, 0, cart1d);
        
        // заполняем ghost cells
        int source;
        int dest;
        
        MPI_Cart_shift(cart1d, 0, 1, &source, &dest);
        MPI_Send(&ll.u0[blocksize - ghost - 1], 1, MPI_INT, dest, 0, cart1d);
        MPI_Recv(&ll.u0[0], 1, MPI_INT, source, 0, cart1d, &status);
        
        MPI_Cart_shift(cart1d, 0, -1, &source, &dest);
        MPI_Send(&ll.u0[ghost], 1, MPI_INT, dest, 0, cart1d);
        MPI_Recv(&ll.u0[blocksize - 1], 1, MPI_INT, source, 0, cart1d, &status);

        life_step(&ll);

        MPI_Gatherv(&ll.u0[ghost], blocksize - 2 * ghost, MPI_INT, &l.u0[0], counts, beginnings, MPI_INT, 0, cart1d);
        MPI_Gatherv(&ll.u1[ghost], blocksize - 2 * ghost, MPI_INT, &l.u1[0], counts, beginnings, MPI_INT, 0, cart1d);
        if (rank == 0){
             life_save_to_file(i, &l);
        }
    }
 
    MPI_Finalize();
    return 0;
}

/*
 * Загрузить входную конфигурацию.
 * Формат файла, число шагов, размер поля, затем идут координаты заполненых клеток:
 * steps
 * nx
 * i1
 * i2
 */

void life_init(const char *path, life_t *l){
    FILE *fd = fopen("rule101_input", "r");
    fscanf(fd, "%d\n", &l->steps);
    fscanf(fd, "%d\n", &l->nx);
    l->u0 = (int*)calloc(l->nx, sizeof(int));
    l->u1 = (int*)calloc(l->nx, sizeof(int));
    int i, r;
    while ((r = fscanf(fd, "%d\n", &i)) != EOF) {
        l->u0[i] = 1;
    }
    fclose(fd);
    printf("Field length: %d\n", l->nx);



}

void rule30(life_t *l){
    int sum;
    int res;
    for (int i = 1; i < l->nx - 1; ++i){
        res = 0;
        sum = l->u0[i-1] + l->u0[i] + l->u0[i+1];
        if (l->u0[i-1] == 1){
            if (sum == 1){
                res = 1;
            }
        }
        else {
            if (sum > 0){
                res = 1;
            }
        }
        l->u1[i] = res;
    }
}

void rule110(life_t *l){
    int sum;
    int res;
    for (int i = 1; i < l->nx - 1; ++i){
        sum = l->u0[i-1] + l->u0[i] + l->u0[i+1];
        if (l->u0[i-1] == 1){
            if (sum  == 2){
                 res = 1;
             }
             else {
                 res = 0;
             }
         }
         else {
             if (sum > 0){
                 res = 1;
             }
             else {
                 res = 0;
             }
         }
         l->u1[i] = res;
     }
}

void life_step(life_t *l){
    rule110(l);
    //rule30(l);
    int *tmp;
    tmp = l->u0;
    l->u0 = l->u1;
    l->u1 = tmp;
} 


void life_save_to_file(int step, life_t *l){
    FILE *f;
    f = fopen("rule101_output", "a");
    for (int i = 0; i < l->nx; ++i){
        if (l->u0[i] == 1) fprintf(f, "■");
        else fprintf(f, "□");
    }
    fprintf(f, "\n");
}
