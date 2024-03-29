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
matrix<T> square_matrix_multiply_recursive(matrix<T>&, matrix<T>&);

template <typename T>
class matrix_base
{
      public:
         void divide_matrix();
         void assign_matrix(int);
         void merge_matrix(tuple<T, T, T, T, T>);
         matrix_base(size_t, matrix<int>&, vector<matrix<T>*>&);
      private:
         size_t len;
         matrix<int>& index ;
         matrix<matrix<T>> vec;
         vector<matrix<T>*>& V;
};

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
     auto size = M.size();
     for (auto row = 0; row != size; ++row)
     {
          for (auto col = 0; col != size; ++col)
               cout << M[row][col] << " ";
          cout << endl; 
     }
}

template <typename T>
matrix_base<T>::matrix_base(size_t _len, matrix<int>& _index,  
vector<matrix<T>*>& _V)  : len(_len), index(_index), V(_V)
{
     vec.assign(3, matrix<vector<T>>{});    
     divide_matrix();
}

template <typename T>
void matrix_base<T>::divide_matrix()
{
     for (auto i = 0; i != 3; ++i)
     {
          vec[i].assign(4, matrix<T>{});
          for (auto j = 0, k = 0; j != 4; ++k)
          {
               if (k == 0)
                   vec[i][j].assign(len, vector<T>{}); 
               vec[i][j][k].assign(len, 0);
               if (k == len - 1)
               {
                   ++j;
                   k = -1; 
               }
          }
     }
}

template <typename T>
void matrix_base<T>::merge_matrix(tuple<T, T, T, T, T> index)
{
     auto i = get<4>(index);
     auto matrix_l = square_matrix_multiply_recursive(vec[0][get<0>(index)], vec[1][get<1>(index)]);
     auto matrix_r = square_matrix_multiply_recursive(vec[0][get<2>(index)], vec[1][get<3>(index)]);
     auto size = matrix_l.size();
     for (auto row = 0; row != size; ++row)
     {
          for (auto col = 0; col != size; ++col)
               vec[2][i][row][col] = matrix_l[row][col] + matrix_r[row][col];
     }
}

template <typename T>
void matrix_base<T>::assign_matrix(int reverse)
{
     for (auto i = 0; i != 4; ++i)
     {
          for (auto j = 0, k = 0; k !=  len; ++j)
          {
               auto row = k + index[i][0];
               auto col = j + index[i][1];
               if (reverse)
               {
                   vec[0][i][k][j] = (*V[0])[row][col];
                   vec[1][i][k][j] = (*V[1])[row][col];
               } 
               else   
               {
                   (*V[2])[row][col] = vec[2][i][k][j];
               }
               if (j == len - 1)
               {  
                   j = -1;
                   ++k; 
               }
          }
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
          matrix<int> index = { {0, 0}, {0, len}, {len, 0}, {len, len} };
          vector<matrix<T>*> V = { &A, &B, &C };
          matrix_base<T> mb(len, index, V);
          mb.assign_matrix(1);
          mb.merge_matrix(make_tuple(0,1,1,3,1));
          mb.merge_matrix(make_tuple(0,0,1,2,0));
          mb.merge_matrix(make_tuple(2,0,3,2,2));
          mb.merge_matrix(make_tuple(2,1,3,3,3));
          mb.assign_matrix(0);
     }
     return C;
}
