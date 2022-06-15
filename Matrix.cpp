#include "Matrix.h"
#include <math.h>
#include <algorithm>

#ifndef EPSILON
#define EPSILON 1e-15
#endif // !EPSILON

#ifndef ITER_TIMES
#define ITER_TIMES 100
#endif // ITER_TIMES



int Matrix::iPrecise = 5;

/*-----------------------构造函数----------------------*/
//默认构造函数
Matrix::Matrix()
{
	this->miRow = 0, this->miCol = 0;
	this->mpBuf = nullptr;
}
//double类转矩阵
Matrix::Matrix(double num)
{
	this->miRow = 1, this->miCol = 1;
	this->mpBuf = new double[1];
	mpBuf[0] = num;
}
//使用初始化列表(行向量)
Matrix::Matrix(const MatIniLst& lst)
{
	int i = 0;
	this->miRow = 1, this->miCol = lst.size();
	if (!isSizeValidity())
		return;
	this->mpBuf = new double[this->miRow * this->miCol];
	for (auto itm : lst)
	{
		mpBuf[i++] = itm;
	}
}
//使用初始化列表(一般的矩阵,可自动补0)
Matrix::Matrix(const std::initializer_list<MatIniLst>& lst)
{
	int i = 0;
	this->miRow = lst.size();
	for (auto itm : lst)
	{
		//找出列数
		if (this->miCol < itm.size())
			this->miCol = itm.size();
	}
	if (!isSizeValidity())
		return;
	this->mpBuf = new double[this->miRow * this->miCol];
	for (auto itm : lst)
	{
		for (auto ele : itm)
		{
			this->mpBuf[i++] = ele;
		}
		//补0
		for (int j = itm.size(); j < this->miCol; j++)
		{
			this->mpBuf[i++] = 0;
		}
	}
}

//使用初始化列表(一般的矩阵,可自动补0)
Matrix::Matrix(const std::initializer_list<MatIniLst>& lst, int col)
{
	int i = 0;
	this->miRow = lst.size();
	this->miCol = col;
	for (auto itm : lst)
	{
		//找出列数
		if (this->miCol < itm.size())
			this->miCol = itm.size();
	}
	if (!isSizeValidity())
		return;
	this->mpBuf = new double[this->miRow * this->miCol];
	for (auto itm : lst)
	{
		for (auto ele : itm)
		{
			this->mpBuf[i++] = ele;
		}
		//补0
		for (int j = itm.size(); j < this->miCol; j++)
		{
			this->mpBuf[i++] = 0;
		}
	}
}
//使用一维数组初始化，
//输入数组的首地址、矩阵行数、矩阵列数、数组长度
Matrix::Matrix(double* p, int row, int col, int count):miRow(row),miCol(col)
{
	if (!isSizeValidity())
		return;
	if (count < row * col)
	{
		this->miRow = 0;
		this->miCol = 0;
		this->mpBuf = nullptr;
		return;
	}
	this->mpBuf = new double[this->miRow * this->miCol];
	for (int i = 0; i < this->miRow * this->miCol; i++)
	{
		this->mpBuf[i] = p[i];
	}
}

/*-----------------------拷贝构造函数----------------------*/
Matrix::Matrix(const Matrix& tmp) :miRow(tmp.miRow), miCol(tmp.miCol)
{
	if (this->mpBuf != nullptr)
		delete[]this->mpBuf;
	//没必要
	if (!isSizeValidity())
		return;
	this->mpBuf = new double[this->miRow * this->miCol];
	memcpy(this->mpBuf, tmp.mpBuf, this->miRow * this->miCol * sizeof(double));
}
/*-----------------------移动构造函数----------------------*/
Matrix::Matrix(Matrix&& tmp) :miRow(tmp.miRow), miCol(tmp.miCol), mpBuf(tmp.mpBuf)
{
	tmp.mpBuf = nullptr;
	tmp.miRow = 0;
	tmp.miCol = 0;
	//没必要
	if (!isSizeValidity())
		return;
}
/*-----------------------析构函数----------------------*/
Matrix::~Matrix()
{
	if (this->mpBuf == nullptr)
		return;
	delete[]mpBuf;
	mpBuf = nullptr;
	this->miRow = this->miCol = 0;
}




/*-----------------------静态函数-----------------------*/
// 用于生成一些特殊矩阵
Matrix Matrix::eye(int row)
{
	if (row <= 0)
	{
		return Matrix();
	}
	int col = row;
	double* buf = new double[row * col];
	memset(buf, 0, row * col * sizeof(double));
	for (int i = 0, j = 0; i < row; i++, j++)
	{
		buf[i * col + j] = 1;
	}
	Matrix temp = Matrix(buf, row, col, row * col);
	delete[] buf;
	return temp;
}
Matrix Matrix::eye(int row, int col)
{
	if (row <= 0 || col <= 0)
	{
		return Matrix();
	}
	double* buf = new double[row * col];
	memset(buf, 0, row * col * sizeof(double));
	for (int i = 0,j = 0; i < row && j < col; i++, j++)
	{
		buf[i * col + j] = 1;
	}
	Matrix temp = Matrix(buf, row, col, row * col);
	delete[] buf;
	return temp;
}
Matrix Matrix::ones(int row)
{
	if (row <= 0)
	{
		return Matrix();
	}
	int col = row;
	double* buf = new double[row * col];
	memset(buf, 0, row * col * sizeof(double));
	for (int i = 0; i < row * col; i++)
	{
		buf[i] = 1;
	}
	Matrix temp = Matrix(buf, row, col, row * col);
	delete[] buf;
	return temp;
}
Matrix Matrix::ones(int row, int col)
{
	if (row <= 0 || col <= 0)
	{
		return Matrix();
	}
	double* buf = new double[row * col];
	memset(buf, 0, row * col * sizeof(double));
	for (int i = 0; i < row*col; i++)
	{
			buf[i] = 1;
	}
	Matrix temp = Matrix(buf, row, col, row * col);
	delete[] buf;
	return temp;
}
Matrix Matrix::zeros(int row)
{
	if (row <= 0)
	{
		return Matrix();
	}
	int col = row;
	double* buf = new double[row * col];
	memset(buf, 0, row * col * sizeof(double));
	Matrix temp = Matrix(buf, row, col, row * col);
	delete[] buf;
	return temp;
}
Matrix Matrix::zeros(int row, int col)
{
	if (row <= 0 || col <= 0)
	{
		return Matrix();
	}
	double* buf = new double[row * col];
	memset(buf, 0, row * col * sizeof(double));
	Matrix temp = Matrix(buf, row, col, row * col);
	delete[] buf;
	return temp;
}
void Matrix::setPrecise(int precise)
{
	iPrecise = precise;
}

//对角矩阵
Matrix Matrix::diag(Matrix mat)
{
	int row = mat.miRow;
	int col = mat.miCol;

	// 从行、列向量，到对角矩阵
	if (row == 1)
	{
		Matrix result = Matrix::eye(col);
		for (int i = 0; i < col; i++)
		{
			result.mpBuf[i * col + i] = mat.mpBuf[i];
		}
		return result;
	}
	else if (col == 1)
	{
		Matrix result = Matrix::eye(row);
		for (int i = 0; i < row; i++)
		{
			result.mpBuf[i * row + i] = mat.mpBuf[i];
		}
		return result;
	}
	// 从矩阵到对角元素列向量
	if (row > col)
		row = col;
	Matrix result = Matrix::zeros(row, 1);
	for (int i = 0; i < row; i++)
	{
		result.mpBuf[i] = mat.mpBuf[i * col + i];
	}
	return result;
}

// 随机矩阵
Matrix Matrix::randMatrix(int n)
{
	if (n < 1)
	{
		return Matrix();
	}
		Matrix result = Matrix::zeros(n);
	for (int i = 0; i < n * n; i++)
	{
		result.mpBuf[i] = rand() / double(RAND_MAX);
	}
	return result;
}
Matrix Matrix::randMatrix(int row, int col)
{
	if (row < 1 || col < 1)
	{
		return Matrix();
	}
	Matrix result = Matrix::zeros(row, col);
	for (int i = 0; i < row * col; i++)
	{
		result.mpBuf[i] = rand() / double(RAND_MAX);
	}
	return result;
}


/*----------------------运算符重载---------------------*/
// = 拷贝赋值
Matrix& Matrix::operator=(const Matrix& tmp)
{
	if (this == &tmp)
		return *this;

	if (this->mpBuf != nullptr)
		delete[]this->mpBuf;
	this->miRow = tmp.miRow;
	this->miCol = tmp.miCol;
	if (!isSizeValidity())
		return *this;
	this->mpBuf = new double[this->miRow * this->miCol];
	memcpy(this->mpBuf, tmp.mpBuf, this->miRow * this->miCol * sizeof(double));
	return *this;
}
// = 移动赋值
Matrix& Matrix::operator=(Matrix&& tmp)
{
	if (this == &tmp)
		return *this;

	if (this->mpBuf != nullptr)
		delete[]this->mpBuf;
	this->miRow = tmp.miRow;
	this->miCol = tmp.miCol;
	this->mpBuf = tmp.mpBuf;
	tmp.miRow = 0;
	tmp.miCol = 0;
	tmp.mpBuf = nullptr;
	isSizeValidity();
	return *this;
}

// + 加法（若不能计算，返回空矩阵）
Matrix operator+(const Matrix& lMat, const Matrix& rMat)
{
	return lMat.add(rMat);
}
// - 减法（若不能计算，返回空矩阵）
Matrix operator-(const Matrix& lMat, const Matrix& rMat)
{
	return lMat.sub(rMat);
}
// * 乘法（若不能计算，返回空矩阵）
Matrix operator*(const Matrix& lMat, const Matrix& rMat)
{
	return lMat.rMultiple(rMat);
}
// * 数乘
Matrix operator*(const double& lFactor, const Matrix& rMat)
{
	return rMat.sMultiple(lFactor);
}
Matrix operator*(const Matrix& lMat, const double& rFactor)
{
	return lMat.sMultiple(rFactor);
}

//  除法（若不能计算，返回空矩阵，与乘法相似）
Matrix operator/(const Matrix& lMat, const Matrix& rMat)
{
	return lMat * rMat.inverse();
}
Matrix operator/(const double& lFactor, const Matrix& rMat)
{
	return lFactor * rMat.inverse();
}
Matrix operator/(const Matrix& lMat, const double& rFactor)
{
	if (fabs(rFactor)<EPSILON)
		return Matrix();
	return double(1) / rFactor * lMat;
}


// 广播
Matrix operator+(const Matrix& lMat, const double& rNum)
{
	return lMat + rNum * Matrix::ones(lMat.miRow, lMat.miCol);
}
Matrix operator+(const double& lNum, const Matrix& rMat)
{
	return rMat + lNum * Matrix::ones(rMat.miRow, rMat.miCol);
}
Matrix operator-(const Matrix& lMat, const double& rNum)
{
	return lMat - rNum * Matrix::ones(lMat.miRow, lMat.miCol);
}
Matrix operator-(const double& lNum, const Matrix& rMat)
{
	return rMat - lNum * Matrix::ones(rMat.miRow, rMat.miCol);
}



// += 自增（若不能计算，返回空矩阵）
Matrix& Matrix::operator+=(const Matrix& tmp)
{
	*this = *this + tmp;
	return *this;
}
Matrix& Matrix::operator+=(const double& num)
{
	*this = *this + num;
	return *this;
}
// -= 自减（若不能计算，返回空矩阵）
Matrix& Matrix::operator-=(const Matrix& tmp)
{
	*this = *this - tmp;
	return *this;
}
Matrix& Matrix::operator-=(const double& num)
{
	*this = *this - num;
	return *this;
}

// ==和!= 判等(只有两个矩阵相同时，返回true)
bool operator==(const Matrix& lMat, const Matrix& rMat)
{
	int count = lMat.miRow * lMat.miCol;
	if (count != rMat.miRow * rMat.miCol)
		return false;
	for (int i = 0; i < count; i++)
	{
		if (fabs(lMat.mpBuf[i] - rMat.mpBuf[i])> EPSILON)
			return false;
	}
	return true;
}
bool operator!=(const Matrix& lMat, const Matrix& rMat)
{
	return !(lMat == rMat);
}
// - 取相反
Matrix operator-(const Matrix& Mat)
{
	return Mat.sMultiple(-1);
}
// + 不变
Matrix operator+(const Matrix& Mat)
{
	return Mat;
}



// ++
Matrix& Matrix::operator++()
{
	*this += 1;
	return *this;
}
Matrix Matrix::operator++(int)
{
	Matrix tmp = *this;
	*this += 1;
	return tmp;
}
// --
Matrix& Matrix::operator--()
{
	*this -= 1;
	return *this;
}
Matrix Matrix::operator--(int)
{
	Matrix tmp = *this;
	*this -= 1;
	return tmp;
}


// []取下标(要用两个[]以访问元素)
// ---------------------------------
// 不要在类的方法中使用取下标运算符！！！
// 不要在类的方法中使用取下标运算符！！！
// 不要在类的方法中使用取下标运算符！！！
// 请直接使用mpBuf[]获取元素 ！！！
// ---------------------------------
double*  Matrix::operator[](int num)
{
	return this->mpBuf + num * this->miCol;
}
const double*  Matrix::operator[](int num)const
{
	return this->mpBuf + num * this->miCol;
}


// <<流输出
std::ostream& operator<<(std::ostream& os,  const Matrix& tmp)
{
	os << '[' << std::endl;
	for (int i = 0; i < tmp.miRow; i++)
	{
		os << "   ";
		for (int j = 0; j < tmp.miCol; j++)
		{
			os << std::setw(Matrix::iPrecise+8) << std::left << std::setprecision(Matrix::iPrecise) << tmp.mpBuf[i * tmp.miCol + j];
		}
		os << std::endl;
	}
	os << ']' << std::endl;
	return os;
}




/*-----------------------访问函数----------------------*/
//返回矩阵的行数
int Matrix::getRow()const
{
	return this->miRow;
}
//返回矩阵的列数
int Matrix::getCol()const
{
	return this->miCol;
}

//矩阵加法
Matrix Matrix::add(const Matrix& tmp)const
{
	//同型检查
	if (this->miRow != tmp.miRow || this->miCol != tmp.miCol)
	{
		return Matrix();
	}
	//矩阵加法
	Matrix result = *this;

	for (int i = 0; i < miRow * miCol; i++)
	{
		result.mpBuf[i] += tmp.mpBuf[i];
	}
	return result;
}

//矩阵减法
Matrix Matrix::sub(const Matrix& tmp)const
{
	//同型检查
	if (this->miRow != tmp.miRow || this->miCol != tmp.miCol)
	{
		return Matrix();
	}
	//矩阵减法
	Matrix result = *this;

	for (int i = 0; i < miRow * miCol; i++)
	{
		result.mpBuf[i] -= tmp.mpBuf[i];
	}
	return result;
}
//矩阵数乘
Matrix Matrix::sMultiple(const double& factor)const
{
	Matrix result = *this;
	//矩阵数乘
	for (int i = 0; i < miRow * miCol; i++)
	{
		result.mpBuf[i] *= factor;
	}
	return result;
}
//矩阵右乘，tmp是右矩阵
Matrix Matrix::rMultiple(const Matrix& tmp)const
{
	if (this->miCol != tmp.miRow)
	{
		return Matrix();
	}
	if (this->miRow <= 0 || this->miCol <= 0
		|| tmp.miRow <= 0 || tmp.miCol <= 0)
	{
		return Matrix();
	}

	Matrix result= Matrix::zeros(this->miRow, tmp.miCol);
	int count = 0;
	for (int i = 0; i < this->miRow; i++)
	{
		for (int j = 0; j < tmp.miCol; j++)
		{
			for (int k = 0; k < tmp.miRow; k++)
			{
				result.mpBuf[count] += this->mpBuf[i * this->miCol + k] * tmp.mpBuf[k * tmp.miCol + j];
			}
			count++;
		}
	}
	return result;
}
//矩阵转置
Matrix Matrix::transpose()const
{
	Matrix result = Matrix::zeros(this->miCol, this->miRow);
	for (int i = 0; i < this->miRow; i++)
	{
		for (int j = 0; j < this->miCol; j++)
		{
			result.mpBuf[j * this->miRow + i] = this->mpBuf[i * this->miCol + j];
		}
	}
	return result;
}
//求矩阵的行列式，
//若矩阵不为方阵，函数返回false,result为0
bool Matrix::det(double& result)const
{
	//是否为方阵
	if (!isSquareMatrix())
	{
		result = 0;
		return false;
	}
	//求行列式的值
	Matrix tmp = *this;

	int count = 0;	//变号次数
	for (int i = 0; i < tmp.miRow; i++)
	{
		int j = 0;
		for (j = i; j < tmp.miRow; j++)
		{
			if (tmp.mpBuf[j * tmp.miCol + i] != 0)
			{
				count += tmp.rowExchange(i, j);
				break;
			}
		}
		if (j == tmp.miRow)
		{
			result = 0;
			return true;
		}
		for (j = i + 1; j < tmp.miRow; j++)
		{
			double factor = tmp.mpBuf[tmp.miCol * j + i] / tmp.mpBuf[tmp.miCol * i + i];
			tmp.rowAdd(j, i, -factor);
		}
	}
	result = 1;
	for (int i = 0; i < tmp.miCol; i++)
	{
		result *= tmp.mpBuf[i * tmp.miCol + i];
	}
	//变号
	if (count % 2)
		result = -result;


	return true;
}
//高斯消元法
Matrix Matrix::gauss()const
{
	//复制一份
	Matrix tmp = *this;
	//消元过程
	int j = 0, count = 0;
	//阶梯化
	for (int i = 0; i < tmp.miCol; i++)
	{
		// 先将矩阵初步进行排列
		for (j = i - count; j < tmp.miRow; j++)
		{
			if (tmp.mpBuf[j * tmp.miCol + i] != 0)
			{
				tmp.rowExchange(i - count, j);
				break;
			}
		}
		// 若本列的元素均为零，从下一列的同一个初始行找起
		if (j == tmp.miRow)
		{
			count++;
			continue;
		}
		// 对角线下元素化为零
		for (j = i - count + 1; j < tmp.miRow; j++)
		{
			double factor = tmp.mpBuf[tmp.miCol * j + i] / tmp.mpBuf[tmp.miCol * (i - count) + i];
			tmp.rowAdd(j, i - count, -factor);
		}
	}
	//行最简化
	for (int i = 0; i < tmp.miRow; i++)
	{
		for (j = i; j < tmp.miCol; j++)
		{
			if (tmp.mpBuf[i * tmp.miCol + j] != 0)
				break;
		}
		if (j == tmp.miCol)
			continue;
		tmp.rowMultiply(i, 1 / tmp.mpBuf[i * tmp.miCol + j]);
		for (int k = i - 1; k >= 0; k--)
		{
			tmp.rowAdd(k, i, -tmp.mpBuf[k * tmp.miCol + j]);
		}
	}
	return tmp;
}
//求秩
int Matrix::rank()const
{
	Matrix tmp = this->gauss();
	int j, i;
	for (i = 0; i < tmp.miRow; i++)
	{
		for (j = i; j < tmp.miCol; j++)
		{
			if (tmp.mpBuf[i * tmp.miCol + j] != 0)
				break;
		}
		if (j == tmp.miCol)
			return i;
	}
	return i;
}
//求逆
Matrix Matrix::inverse()const
{
	double det;
	this->det(det);
	if (fabs(det)<EPSILON)
		return Matrix();
	Matrix diag = Matrix::eye(this->miRow);
	Matrix tmp = this->merge(diag);
	tmp = tmp.gauss();
	return tmp.lrDivide(this->miRow);
}

//矩阵形状的合法性检测
bool Matrix::isSizeValidity()
{
	if (this->miRow <= 0 || this->miCol <= 0)
	{
		this->miRow = 0, this->miCol = 0;
		if (this->mpBuf != nullptr)
		{
			delete[] this->mpBuf;
			this->mpBuf = nullptr;
		}
		return false;
	}
	else
		return true;
}

//方阵检测
bool Matrix::isSquareMatrix()const
{
	return this->miRow > 0 && this->miRow == this->miCol;
}

//空矩阵检测
bool Matrix::isEmptyMatrix()const
{
	if (this->miRow == 0 || this->miCol == 0)
		return true;
	else 
		return false;
}


//行列式性质
//行线性相加，把src行倍乘factor后加到des上
void Matrix::rowAdd(int des, int src, double factor)
{
	for (int i = 0; i < this->miCol; i++)
	{
		this->mpBuf[des * this->miCol + i] += factor * this->mpBuf[src * this->miCol + i];
	}
	return;
}
//行交换,注意变号的问题
bool Matrix::rowExchange(int des, int src)
{
	double tmp;
	for (int i = 0; i < this->miCol; i++)
	{
		tmp = this->mpBuf[src * this->miCol + i];
		this->mpBuf[src * this->miCol + i] = this->mpBuf[des * this->miCol + i];
		this->mpBuf[des * this->miCol + i] = tmp;
	}
	//是否变号
	return !des == src;
}
//将指定行乘以factor
void Matrix::rowMultiply(int des, double factor)
{
	for (int i = 0; i < this->miCol; i++)
	{
		this->mpBuf[des * this->miCol + i] *= factor;
	}
	return;
}
//左右合并
Matrix Matrix::lrMerge(Matrix& left,Matrix& right)
{
	if (left.miRow != right.miRow)
		return Matrix();
	Matrix result = Matrix::zeros(left.miRow, left.miCol + right.miCol);
	for (int i = 0; i < left.miRow; i++)
	{
		for (int j = 0; j < left.miCol; j++)
		{
			result.mpBuf[i * result.miCol + j] = left.mpBuf[i * left.miCol + j];
		}
		for (int j = 0; j < right.miCol; j++)
		{
			result.mpBuf[i * result.miCol + left.miCol + j] = right.mpBuf[i * right.miCol + j];
		}
	}
	return result;
}
//上下合并
Matrix Matrix::udMerge(Matrix& up, Matrix& down)
{
	if (up.miCol != down.miCol)
		return Matrix();
	Matrix result = Matrix::zeros(up.miRow + down.miRow, up.miCol);
	memcpy(result.mpBuf, up.mpBuf, up.miCol * up.miRow * sizeof(double));
	memcpy(result.mpBuf + up.miCol * up.miRow, down.mpBuf, down.miCol * down.miRow * sizeof(double));
	return result;
}

//从该矩阵中得到一块小矩阵
Matrix Matrix::getBlock(int startRowId, int startColId, int blockRow, int blockCol)const
{
	//检测分块合法性
	if (startRowId < 0 || startColId < 0 || blockRow <= 0 || blockCol <= 0 ||
		startRowId + blockRow > this->miRow || startColId + blockCol > this->miCol)
	{
		return Matrix();
	}

	Matrix result = Matrix::zeros(blockRow, blockCol);
	for (int row = 0; row < blockRow; row++)
	{
		for (int col = 0; col < blockCol; col++)
		{
			result.mpBuf[row * blockCol + col] = this->mpBuf[(row + startRowId) * this->miCol + col + startColId];
		}
	}
	return result;
}

//将该矩阵中的一块设置为给定的矩阵
Matrix& Matrix::setBlock(int startRowId, int startColId,const Matrix& block)
{
	int blockRow = block.miRow, blockCol = block.miCol;
	//检测分块合法性
	if (startRowId < 0 || startColId < 0 || blockRow <= 0 || blockCol <= 0 ||
		startRowId + blockRow > this->miRow || startColId + blockCol > this->miCol)
	{
		return *this;
	}

	for (int row = 0; row < blockRow; row++)
	{
		for (int col = 0; col < blockCol; col++)
		{
			this->mpBuf[(row + startRowId) * this->miCol + col + startColId] = block.mpBuf[row * blockCol + col];
		}
	}
	return *this;
}

//将本矩阵（方阵）化为上Hessenberg矩阵
Matrix Matrix::hessenberg(Matrix &Q)const
{
	if (!this->isSquareMatrix())
	{
		return Matrix();
	}
	if (this->miCol < 3)
		return *this;

	Matrix result = *this;
	Matrix HouseHolder;
	Matrix x;
	Matrix temp;
	Q = Matrix::eye(this->miCol);

	int n = 0;
	double sigma = 0;
	double rho = 0;
	for (int i = 0; i < this->miCol - 2; i++)
	{
		n = result.miCol - i - 1;
		// 构造n阶HouseHolder矩阵

		x = result.getBlock(i + 1, i, n, 1);
		sigma = sgn(x.mpBuf[0]) * sqrt((x.transpose() * x).mpBuf[0]);
		// 说明该列已经是n*e的列向量了
		if (fabs(pow(x.mpBuf[0], 2) - pow(sigma, 2)) < EPSILON)
		{
			continue;
		}
		x.mpBuf[0] += sigma;
		rho = (x.transpose() * x).mpBuf[0] / 2;

		HouseHolder = Matrix::eye(n) - x * x.transpose() / rho;

		temp = Matrix::zeros(n, 1);
		temp.mpBuf[0] = -sigma;
		result.setBlock(i + 1, i, temp);

		temp = result.getBlock(0, i + 1, i + 1, n);
		result.setBlock(0, i + 1, temp * HouseHolder);

		temp = result.getBlock(i + 1, i + 1, n, n);
		result.setBlock(i + 1, i + 1, HouseHolder * temp * HouseHolder);
		

		Q = Q * Matrix::eye(this->miCol).setBlock(i + 1, i + 1, HouseHolder);
	}
	return result;
}

//对本矩阵进行QR分解
void Matrix::qrDecom(Matrix& Q, Matrix& R)const
{
	int col = this->miCol;
	int row = this->miRow;
	int size = col > row ? row : col;
	Q = Matrix::eye(row);
	R = *this;
	if (row < 2)
		return;

	Matrix HouseHolder;
	Matrix x;
	Matrix temp;

	int n = 0;
	double sigma = 0;
	double rho = 0;


	// 依然是使用镜面反射矩阵（HouseHolder矩阵）和分块计算
	for (int i = 0; i < size; i++)
	{
		// 计算(row-i)阶镜面反射矩阵
		n = row - i;
		x = R.getBlock(i, i, n, 1);
		sigma = sgn(x.mpBuf[0]) * sqrt((x.transpose() * x).mpBuf[0]);
		// 说明该列已经是n*e的列向量了
		if (fabs(pow(x.mpBuf[0], 2) - pow(sigma, 2)) < EPSILON)
		{
			continue;
		}
		x.mpBuf[0] += sigma;
		rho = (x.transpose() * x).mpBuf[0] / 2;
		HouseHolder = Matrix::eye(n) - x * x.transpose() / rho;


		temp = Matrix::zeros(n, 1);
		temp.mpBuf[0] = -sigma;
		R.setBlock(i, i, temp);

		temp = R.getBlock(i, i + 1, n, col - i - 1);
		R.setBlock(i, i + 1, HouseHolder * temp);


		Q = Q * Matrix::eye(row).setBlock(i, i, HouseHolder);
	}
	return;
}

//计算矩阵的特征值
Matrix Matrix::eigen()const
{
	if (!this->isSquareMatrix())
		return Matrix();

	// 带原点位移的QR分解法
	int col = this->miCol;
	int count = 0;
	Matrix Rh;
	Matrix Hessenberg = this->hessenberg(Rh);
	Matrix bar = Matrix::eye(col) * Hessenberg.mpBuf[col * col - 1];
	Matrix Q, R;
	for (int i = 0; i < ITER_TIMES; i++)
	{
		Hessenberg -= bar;
		Hessenberg.qrDecom4Hessenberg(Q, R);
		Hessenberg = R * Q + bar;
		bar = Matrix::eye(col) * Hessenberg.mpBuf[col * col - 1];
		// 迭代中止条件
		count = 0;
		for (int i = 0; i < col-1; i++)
		{
			if (fabs(Hessenberg.mpBuf[(i + 1) * col + i]) < 1e-8)
			{
				count++;
			}
			else
				break;
		}
		if (count == col - 1)
		{
			Matrix result = Matrix::diag(Hessenberg);
			// 排序不想写了，直接用库(
			std::sort(result.mpBuf, result.mpBuf + result.miRow, [](int i, int j)->bool { return i > j; });
			return result;
		}
	}
	return Matrix();
}



/*---------------------------------------- 辅助函数 ------------------------------------------*/
//符号函数
signed char Matrix::sgn(double num)
{
	if (num < 0)
		return -1;
	else 
		return 1;
}
//合并、拆分（线性变换求可逆阵）
Matrix Matrix::merge(Matrix& right)const
{
	if (this->miRow != right.miRow)
		return Matrix();
	Matrix result = Matrix::zeros(this->miRow, this->miCol + right.miCol);
	for (int i = 0; i < this->miRow; i++)
	{
		for (int j = 0; j < this->miCol; j++)
		{
			result.mpBuf[i * result.miCol + j] = this->mpBuf[i * this->miCol + j];
		}
		for (int j = 0; j < right.miCol; j++)
		{
			result.mpBuf[i * result.miCol + this->miCol + j] = right.mpBuf[i * right.miCol + j];
		}
	}
	return result;
}
//将矩阵分为左右两个，左矩阵有col列,返回右矩阵
Matrix Matrix::lrDivide(int col)const
{
	if (this->miCol <= col)
		return Matrix();
	Matrix result = Matrix::zeros(this->miRow, this->miCol - col);
	for (int i = 0; i < this->miRow; i++)
	{
		for (int j = 0; j < this->miCol - col; j++)
		{
			result.mpBuf[i * result.miCol + j] = this->mpBuf[i * this->miCol + col + j];
		}
	}
	return result;
}

// 对本矩阵（Hessenberg方阵）进行完全QR分解
bool Matrix::qrDecom4Hessenberg(Matrix& Q, Matrix& R)const
{
	// 不可分解
	if (!isSquareMatrix())
	{
		Q = Matrix();
		R = Matrix();
		return false;
	}


	int col = this->miCol;
	int row = col;
	// 利用Givens变换
	//   cos phi		sin phi 
	//	-sin phi		cos phi	
	Q = Matrix::eye(col);
	R = *this;

	double r = 0;
	double cosphi = 1;
	double sinphi = 0;
	Matrix temp;
	Matrix Givens = Matrix::eye(2);

	for (int n = 0; n < col - 1; n++)
	{
		if (fabs(R.mpBuf[(n + 1) * col + n]) < EPSILON)
		{
			continue;
		}
		// 构造2阶Givens矩阵
		r = sqrt(pow(R.mpBuf[n * col + n], 2) + pow(R.mpBuf[(n + 1) * col + n], 2));
		cosphi = R.mpBuf[n * col + n] / r;
		sinphi = R.mpBuf[(n + 1) * col + n] / r;
		Givens.mpBuf[0] = cosphi;
		Givens.mpBuf[1] = sinphi;
		Givens.mpBuf[2] = -sinphi;
		Givens.mpBuf[3] = cosphi;

		// 用Givens矩阵进行更新
		temp = R.getBlock(n, 0, 2, col);
		R.setBlock(n, 0, Givens * temp);
		temp = Q.getBlock(n, 0, 2, col);
		Q.setBlock(n, 0, Givens * temp);

	}
	Q = Q.transpose();
	return true;
}
