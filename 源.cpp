#include "Matrix.h"
#include "math.h"

using namespace std;
int main()
{
	//构造
	Matrix A = { 1,2,3 };
	cout << A << endl;
	//拷贝构造
	Matrix B = -+-+-+-A;
	B += 1;
	cout << B << endl;
	//拷贝赋值
	A = {{1,2,3},
		 {4,5,6},
		 {7,8,10}};
	cout << A << endl;

	//转置+移动构造
	Matrix C = A.transpose();
	cout << C<< endl;
	//求逆+移动赋值
	B = A.inverse();
	cout << B << endl;
	
	//除法
	cout << "/" << endl;
	Matrix D = A * A - 2 * -(+(-A)) * 2;
	cout << D.transpose()<< endl;
	cout << D.inverse() << endl;
	cout << A / D << endl;
	cout << 1 / D << endl;
	cout << D / 2 << endl;
	cout << D / 0 << endl;
	cout << D / D << endl;




	double det = 0;
	D.det(det);
	cout << det << endl;

	const Matrix E = D.transpose() + A;
	cout << E << endl;
	D[0][0] = 1;	// 非引用版本  正确
	//E[0][0] = 1;  //	 引用版本  错误

	//空矩阵的兼容
	Matrix F;
	Matrix G = ++--F--++ * 2 - 1;
	cout << F << endl;
	cout << G << endl;

	const Matrix H = 2 * A;
	Matrix I = H * 2;


	cout << H << endl;
	cout << I << endl;
	cout << ++--I--++ << endl;

	cout << I << endl;


}