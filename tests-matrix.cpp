#include "Matrix.h"
#include "math.h"

#include "catch.hpp"

using namespace std;

TEST_CASE("default construction", "[construction]")
{
  Matrix A = Matrix();
  REQUIRE(A.getCol() == 0);
  REQUIRE(A.getRow() == 0);
  // Todo
}

TEST_CASE("equal and copy construction/assignment", "[special functions]")
{
  Matrix A = Matrix();
  Matrix B = Matrix();
  const Matrix tmpVector = Matrix({1, 2, 3});
  const Matrix tmpMatrix = Matrix({{1, 2, 3}, {4, 5, 6}, {7, 8, 9}});

  SECTION("empty matrix")
  {
    A = Matrix();
    B = Matrix();
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
    B = Matrix();
    Matrix C = std::move(A);
    REQUIRE(C == tmpMatrix);
    REQUIRE(A == B);
  }

  SECTION("move assignment")
  {
    Matrix C = Matrix();
    B = Matrix();
    A = tmpMatrix;
    C = std::move(A);
    REQUIRE(C == tmpMatrix);
    REQUIRE(A == B);
  }
}

TEST_CASE("static function", "[static function]")
{
  Matrix emptyMat = Matrix();
  SECTION("eye")
  {
    Matrix eye3 = {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}};
    Matrix eye34 = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}};
    Matrix eye43 = {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}, {0, 0, 0}};

    REQUIRE((Matrix::eye(3) == eye3 && Matrix::eye(3, 3) == eye3));
    REQUIRE((Matrix::eye(3, 4) == eye34 && Matrix::eye(4, 3) == eye43));
    REQUIRE((Matrix::eye(0) == emptyMat && Matrix::eye(-1) == emptyMat));
    REQUIRE((Matrix::eye(3, 0) == emptyMat && Matrix::eye(0, 3) == emptyMat && Matrix::eye(3, -1) == emptyMat && Matrix::eye(-1, 3) == emptyMat));
  }
  SECTION("ones")
  {
    Matrix ones3 = {{1, 1, 1}, {1, 1, 1}, {1, 1, 1}};
    Matrix ones34 = {{1, 1, 1, 1}, {1, 1, 1, 1}, {1, 1, 1, 1}};
    Matrix ones43 = {{1, 1, 1}, {1, 1, 1}, {1, 1, 1}, {1, 1, 1}};

    REQUIRE((Matrix::ones(3) == ones3 && Matrix::ones(3, 3) == ones3));
    REQUIRE((Matrix::ones(3, 4) == ones34 && Matrix::ones(4, 3) == ones43));
    REQUIRE((Matrix::ones(0) == emptyMat && Matrix::ones(-1) == emptyMat));
    REQUIRE((Matrix::ones(3, 0) == emptyMat && Matrix::ones(0, 3) == emptyMat && Matrix::ones(3, -1) == emptyMat && Matrix::ones(-1, 3) == emptyMat));
  }
  SECTION("zeros")
  {
    Matrix zeros3 = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}};
    Matrix zeros34 = {{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}};
    Matrix zeros43 = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}};

    REQUIRE((Matrix::zeros(3) == zeros3 && Matrix::zeros(3, 3) == zeros3));
    REQUIRE((Matrix::zeros(3, 4) == zeros34 && Matrix::zeros(4, 3) == zeros43));
    REQUIRE((Matrix::zeros(0) == emptyMat && Matrix::zeros(-1) == emptyMat));
    REQUIRE((Matrix::zeros(3, 0) == emptyMat && Matrix::zeros(0, 3) == emptyMat && Matrix::zeros(3, -1) == emptyMat && Matrix::zeros(-1, 3) == emptyMat));
  }
  SECTION("diag")
  {
    Matrix rowVector = Matrix({1, 2, 3});
    Matrix colVector = Matrix({1, 2, 3}).transpose();
    Matrix diagMatrix3 = Matrix({{1, 0, 0}, {0, 2, 0}, {0, 0, 3}});
    Matrix diagMatrix34 = Matrix({{1, 0, 0, 0}, {0, 2, 0, 0}, {0, 0, 3, 0}});
    Matrix diagMatrix43 = Matrix({{1, 0, 0}, {0, 2, 0}, {0, 0, 3}, {0, 0, 0}});

    REQUIRE(Matrix::diag(rowVector) == diagMatrix3);
    REQUIRE(Matrix::diag(colVector) == diagMatrix3);
    REQUIRE(Matrix::diag(diagMatrix3) == colVector);
    REQUIRE(Matrix::diag(diagMatrix34) == colVector);
    REQUIRE(Matrix::diag(diagMatrix43) == colVector);
  }
}

TEST_CASE("matrix operation", "[arithmetic functions]")
{
  Matrix emptyMat = Matrix();
  SECTION("add")
  {
    Matrix A = Matrix({{1, 2, 3}, {4, 5, 6}, {7, 8, 9}});
    Matrix B = Matrix({{1, 3, 5}, {7, 9, 11}, {13, 15, 17}});
    Matrix C = Matrix({{2, 5, 8}, {11, 14, 17}, {20, 23, 26}});

    REQUIRE(A + B == C);
    REQUIRE(B + A == C);
    REQUIRE(A + emptyMat == emptyMat);
    REQUIRE(emptyMat + A == emptyMat);
    REQUIRE(A.add(B) == C);
    REQUIRE(B.add(A) == C);
    REQUIRE(A.add(emptyMat) == emptyMat);
    REQUIRE(emptyMat.add(A) == emptyMat);
  }
  SECTION("minus")
  {
    Matrix A = Matrix({{1, 2, 3}, {4, 5, 6}, {7, 8, 9}});
    Matrix B = Matrix({{1, 3, 5}, {7, 9, 11}, {13, 15, 17}});
    Matrix C = Matrix({{0, -1, -2}, {-3, -4, -5}, {-6, -7, -8}});

    REQUIRE(A - B == C);
    REQUIRE(B - A == -C);
    REQUIRE(A - emptyMat == emptyMat);
    REQUIRE(emptyMat - A == emptyMat);
    REQUIRE(A.sub(B) == C);
    REQUIRE(B.sub(A) == -C);
    REQUIRE(A.sub(emptyMat) == emptyMat);
    REQUIRE(emptyMat.sub(A) == emptyMat);
  }
  // todo
}

TEST_CASE("matrix-related functions", "[basic functions]")
{
  SECTION("transpose")
  {
    Matrix A = Matrix({{1, 2, 3}, {4, 5, 6}, {7, 8, 9}});
    Matrix B = Matrix();
    REQUIRE(A.transpose() == Matrix({{1, 4, 7}, {2, 5, 8}, {3, 6, 9}}));
    REQUIRE(A.transpose().transpose() == A);

    REQUIRE(B.transpose() == B);
  }

  SECTION("inverse")
  {
    Matrix invertibleMat = Matrix({{1, 2, 3}, {4, 5, 5}, {7, 8, 11}});
    Matrix nullMat = Matrix();
    Matrix uninvertibleMat = Matrix({{1, 2, 3}, {1, 2, 3}, {7, 8, 9}});

    REQUIRE(invertibleMat.inverse() != nullMat);
    REQUIRE(invertibleMat.inverse().inverse() == invertibleMat);
    REQUIRE(invertibleMat * invertibleMat.inverse() == Matrix::eye(3));
    REQUIRE(invertibleMat.inverse().transpose() == invertibleMat.transpose().inverse());

    REQUIRE(nullMat.transpose() == nullMat);

    REQUIRE(invertibleMat / invertibleMat == Matrix::eye(3));
    REQUIRE(uninvertibleMat / invertibleMat == uninvertibleMat * invertibleMat.inverse());
    REQUIRE(uninvertibleMat / uninvertibleMat == Matrix());
  }
}

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
  const Matrix B = {{1, -1, 1},
                    {1, 3, -1},
                    {1, 1, 1}};

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
    REQUIRE(Matrix::diag(E) == Matrix::diag({1, 2, 2}));
  }
}
