#include "Matrix.h"
#include "math.h"

#include "catch.hpp"

// #define _MATRIX_DEBUG

using namespace std;

TEST_CASE("equal and copy construction/assignment", "[special functions]")
{
  Matrix A = Matrix();
  Matrix B = Matrix();
  const Matrix tmpVector = Matrix({1, 2, 3});
  const Matrix tmpMatrix = Matrix({{1, 2, 3}, {4, 5, 6}, {7, 8, 9}});

  SECTION("empty matrix")
  {
    REQUIRE(A == B);
    REQUIRE((A.getCol() == 0 && A.getRow() == 0));
  }

  SECTION("vector equal")
  {
    A = Matrix({1, 2, 3});
    B = Matrix({1, 2, 3});

    REQUIRE(A == B);
    REQUIRE(A[0][0] == B[0][0]);
    REQUIRE(A[0][1] == B[0][1]);
    REQUIRE(A[0][2] == B[0][2]);

    REQUIRE(A != B + 1);
    REQUIRE(A != B.transpose());
  }

  SECTION("matrix equal")
  {
    A = Matrix({{1, 2, 3}, {4, 5, 6}, {7, 8, 9}});
    B = Matrix({{1, 2, 3}, {4, 5, 6}, {7, 8, 9}});

    REQUIRE(A == B);
    REQUIRE(A[0][0] == B[0][0]);
    REQUIRE(A[1][1] == B[1][1]);
    REQUIRE(A[2][2] == B[2][2]);

    REQUIRE(A != B + 1);
  }

  SECTION("copy construction")
  {
    A = Matrix({{1, 2, 3}, {4, 5, 6}, {7, 8, 9}});
    Matrix C = A;
    REQUIRE(A == C);
  }

  SECTION("copy assignment")
  {
    A = Matrix({{1, 2, 3}, {4, 5, 6}, {7, 8, 9}});
    B = A;
    REQUIRE(tmpMatrix == B);
  }
}

TEST_CASE("move construction/assignment", "[special functions]")
{
  Matrix A = Matrix();
  Matrix B = Matrix();
  const Matrix tmpMatrix = Matrix({{1, 2, 3}, {4, 5, 6}, {7, 8, 9}});

  SECTION("move construction")
  {
    A = tmpMatrix;
    Matrix C = std::move(A);
    REQUIRE(C == tmpMatrix);
    REQUIRE(A == B);
  }

  SECTION("move assignment")
  {
    Matrix C = Matrix();
    A = tmpMatrix;
    C = std::move(A);
    REQUIRE(C == tmpMatrix);
    REQUIRE(A == B);
  }
}


// 	//转置+移动构造
// 	Matrix C = A.transpose();
// 	cout << C << endl;
// 	//求逆+移动赋值
// 	B = A.inverse();
// 	cout << B << endl;

// 	//除法
// 	cout << "/" << endl;
// 	Matrix D = A * A - 2 * -(+(-A)) * 2;
// 	cout << D.transpose() << endl;
// 	cout << D.inverse() << endl;
// 	cout << A / D << endl;
// 	cout << 1 / D << endl;
// 	cout << D / 2 << endl;
// 	cout << D / 0 << endl;
// 	cout << D / D << endl;

// 	double det = 0;
// 	D.det(det);
// 	cout << det << endl;

// 	const Matrix E = D.transpose() + A;
// 	cout << E << endl;
// 	D[0][0] = 1;	// 非引用版本  正确
// 	//E[0][0] = 1;  //	 引用版本  错误

// 	//空矩阵的兼容
// 	Matrix F;
// 	Matrix G = ++--F--++ * 2 - 1;
// 	cout << F << endl;
// 	cout << G << endl;

// 	const Matrix H = 2 * A;
// 	Matrix I = H * 2;

// 	cout << H << endl;
// 	cout << I << endl;
// 	cout << ++--I--++ << endl;

// 	cout << I << endl;

// 	//矩阵分块（或者应该叫分割）
// 	cout << "分块" << endl;
// 	cout << I.getBlock(0, 0, 3, 3) << endl;
// 	cout << I.getBlock(-1, -1, 3, 3) << endl;
// 	cout << I.getBlock(0, 0, 4, 4) << endl;
// 	cout << I.getBlock(1, 1, -1, -1) << endl;
// 	cout << I.getBlock(1, 1, 2, 2) << endl;

// 	//设置分块
// 	cout << "设置分块" << endl;
// 	cout << I.setBlock(0, 0, Matrix::eye(4, 4)) << endl;
// 	cout << I.setBlock(1, 1, Matrix::eye(2, 2)) << endl;
// 	cout << I.setBlock(0, 1, Matrix::eye(2, 2)) << endl;
// 	cout << I.setBlock(0, 0, Matrix({ 3,4,5 })) << endl;


// 	//Matrix diag = { 1,2,3,4 };
// 	//cout << diag << Matrix::diag(diag) << Matrix::diag(diag.transpose()) << endl;
// 	//Matrix A = { {1,2,3,4},{5,6,7,8},{9,10,11,12} };
// 	//Matrix B = { {43,32,1},{5,3,8},{9,10,11},{3,0,0} };
// 	//cout << A << Matrix::diag(A) << B << Matrix::diag(B) << endl;


TEST_CASE("Hessenberg, QR decomposition and eigen value", "[eigen value]")
{
  const Matrix A = {{1, 2, 1, 2},
                    {2, 2, -1, 1},
                    {1, -1, 1, 1},
                    {2, 1, 1, 1}};
  const Matrix B = { {1, -1, 1},
                     {1, 3, -1},
                     {1, 1, 1} };

  SECTION("Hessenberg")
  {
    Matrix Qh = Matrix::eye(4);
    Matrix H = A.hessenberg(Qh);
    REQUIRE(H == Qh.transpose() * A * Qh);
  }

  SECTION("QR decomposition")
  {
    Matrix Q, R;
    Matrix H = A.hessenberg(Q);
    H.qrDecom(Q, R);
    REQUIRE(Q * Q.transpose() == Matrix::eye(4));
    REQUIRE(H == Q * R);
  }

  SECTION("eigen value")
  {
    Matrix E = B.eigen();
    REQUIRE(Matrix::diag(E)==Matrix::diag({1,2,2}));
  }
}


