run: create_archive multiplicate_matrices multiplicate_matrices_g multiplicate_matrices_O3
multiplicate_matrices: matrix_arc.a
        g++ -g main.cpp matrix_arc.a -o multiplicate_matrices
multiplicate_matrices_g:
        g++ -o multiply_matrices_g main.cpp matrix.cpp -g
multiplicate_matrices_O3:
        g++ -o multiply_matrices_O3 main.cpp matrix.cpp -O3
matrix.o:
        g++ -c matrix.cpp -g
create_archive: matrix.o
        ar rc matrix_arc.a matrix.o
clean:
        rm *.o *.a multiplicate_matrices multiply_matrices_g  multiply_matrices_O3
