#include <math.h>
#include <vector>
#include <iostream>

double cosSimilarity(std::vector<double> A, std::vector<double>B)
{

    if(A.size() == 0 || B.size() == 0)
    {
        std::cout << "vector A and vector B can't be zero-vector" << std::endl;
        exit(-1);
    }
    if (A.size() != B.size())
    {
        std::cout << "Vector A and Vector B are not the same size" << std::endl;
        exit(-1);
    }

    double mul = 0.0;
    double d_a = 0.0;
    double d_b = 0.0 ;


    std::vector<double>::iterator B_iter = B.begin();
    std::vector<double>::iterator A_iter = A.begin();
    for( ; A_iter != A.end(); A_iter++ , B_iter++ )
    {
        mul += *A_iter * *B_iter;
        d_a += *A_iter * *A_iter;
        d_b += *B_iter * *B_iter;
    }

    if (d_a == 0.0 || d_b == 0.0)
    {
        std::cout << "vector A and vector B can't be zero-vector" << std::endl;
        exit(-1);
    }

    return mul / (sqrt(d_a) * sqrt(d_b));
}


int main()
{
    std::vector<double> A = {1, 2, 3, 4, 5};
    std::vector<double> B = {1, 2, 3, 6, 5};

    std::cout << cosSimilarity(A, B) << std::endl;
}