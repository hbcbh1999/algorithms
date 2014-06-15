// square_matrix_multiply_recursive

#include <tuple>
#include <vector>
#include <iostream>
using namespace std;

template <typename T>
using matrix = vector<vector<T>>;

template <typename T>
void print_matrix(const matrix<T>&);

template <typename T>
void divide_matrix(matrix<matrix<T>>&, size_t);

template <typename T>
void merge_matrix(matrix<matrix<T>>&, tuple<T, T, T, T, T>);

template <typename T>
matrix<T> square_matrix_multiply_recursive(matrix<T>&, matrix<T>&);

int main(int argc, char* argv[])
{
    matrix<int> matrix_A = { { 1, 2, 3, 4 }, 
                             { 5, 6, 7, 8 },
                             { 0, 4, 3, 2 },
                             { 9, 7, 2, 1 } 
                           };

    matrix<int> matrix_B = { { -1, 0, 2, 4 }, 
                             { -7, 8, 9, 1 },
                             { -2, 3, 0, 4 },
                             { -4, 0, 2, 3 }
                           };

    cout << "Matrix A:"  << endl;
    print_matrix(matrix_A);
    cout << endl << "Matrix B:"  << endl;
    print_matrix(matrix_B);
    cout << endl << "Matrix A x B:"  << endl;
    print_matrix(square_matrix_multiply_recursive(matrix_A, matrix_B));
}

template <typename T>
void print_matrix(const matrix<T>& M)
{
     for (auto row = 0; row != M.size(); ++row)
     {
          for (auto col = 0; col != M.size(); ++col)
               cout << M[row][col] << " ";
          cout << endl; 
     }
}

template <typename T>
void divide_matrix(matrix<matrix<T>>& vec, size_t len)
{
     for (auto i = 0; i != 3; ++i)
     {
          vec[i].assign(4, matrix<T>{});
          for (auto j = 0; j != 4; ++j)
          {
               vec[i][j].assign(len, vector<T>{}); 
               for (auto k = 0; k !=  len; ++k)
                    vec[i][j][k].assign(len, 0); 
          }
     }
}

template <typename T>
void merge_matrix(matrix<matrix<T>>& V, tuple<T, T, T, T, T> index)
{
     auto i = get<4>(index);
     auto matrix_l = square_matrix_multiply_recursive(V[0][get<0>(index)], V[1][get<1>(index)]);
     auto matrix_r = square_matrix_multiply_recursive(V[0][get<2>(index)], V[1][get<3>(index)]);
     for (auto j = 0; j != matrix_l.size(); ++j)
     {
          for (auto k = 0; k != matrix_r.size(); ++k)
               V[2][i][j][k] = matrix_l[j][k] + matrix_r[j][k];
     }
}

template <typename T>
matrix<T> square_matrix_multiply_recursive(matrix<T>& A, matrix<T>& B)
{
     auto n = A.size();
     int len = n / 2;
     matrix<T> C(n, vector<T>{});
     for (auto i = 0; i != n; ++i)
          C[i].assign(n, 0);
     if (n == 1)
          C[0][0] = A[0][0] * B[0][0];
     else
     {
          matrix<matrix<T>> vec(3, matrix<vector<T>>{});
          divide_matrix(vec, len);
          matrix<int> index = { {0, 0}, {0, len}, {len, 0}, {len, len} };
          for (auto i = 0; i != 4; ++i)
          {
               for (auto j = 0; j !=  len; ++j)
               {
                    for (auto k = 0; k != len; ++k)
                    {
                         auto row = j + index[i][0];
                         auto col = k + index[i][1];
                         vec[0][i][j][k] = A[row][col];
                         vec[1][i][j][k] = B[row][col];
                    } 
               }
          }
          merge_matrix(vec, make_tuple(0,1,1,3,1));
          merge_matrix(vec, make_tuple(0,0,1,2,0));
          merge_matrix(vec, make_tuple(2,0,3,2,2));
          merge_matrix(vec, make_tuple(2,1,3,3,3));
          for (auto i = 0; i != 4; ++i)
          {
               for (auto j = 0; j !=  len; ++j)
               {
                    for (auto k = 0; k != len; ++k)
                    {
                         auto row = j + index[i][0];
                         auto col = k + index[i][1];
                         C[row][col] = vec[2][i][j][k];
                    }
               }
          } 
     }
     return C;
}
