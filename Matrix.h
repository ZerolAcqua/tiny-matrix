#ifndef _Matrix_H
#define _Matrix_H
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <iomanip>
#include <initializer_list>

//************************************************************************************
// 										Matrix类
//
//		本类用于进行矩阵的基本计算
//		1.	矩阵加法，矩阵数乘，矩阵乘法
//		2.	矩阵广播，矩阵合并，矩阵判等
//		3.	高斯消元法，求秩
//		4.	计算行列式，矩阵求逆，求伴随矩阵
//																			2021/3/22
//																				Acqua
//---------------------------------version: 0.1.0-------------------------------------
// 
//		1.	整理注释，修改变量名称
//      2.	优化修改了部分的构造函数和静态函数
//			1)eye
//			2)ones
//			3)zeros
//		3.	修正了移动构造函数和移动赋值函数实现
//																			2021/10/2
//		4.	重载了<<运算运算符实现流输出
//		5.	无参构造函数和错误返回的矩阵均改为0*0的空矩阵，并修改了
//			部分函数内检查矩阵形状的代码(主要是空矩阵的判断)
//		6.	修改了大部分成员函数：变为非成员函数并作为友元函数，
//			参数设置为常量引用，左值和右值现在均可绑定。包括
//			1) 成员函数 =rvalue  =lvalue
//			2) 友元函数 Matrix+Matrix Matrix-Matrix
//			3) 友元函数 Matrix*Matrix
//			4) 友元函数 factor*Matrix Matrix*factor
//		TODO:	
//		1. 矩阵广播，
//		2. 复合赋值"+=""-="，
//		3. 前后置自增"++"自减"--"，
//		4. 单元运算符"+/-",
//		5. 矩阵除法 "/"  
//																			2021/10/5
// 
// 
//		1.	矩阵广播（只做了与数字的）
//		2.	复合赋值"+=""-="
//		3.	单元运算符"+/-"
//																			2021/10/9	
//	
//		1. 前后置自增"++"自减"--"，
//		2. 矩阵除法 "/"  
//		3. 修改了部分辅助函数，加上const关键字，使其不能修改成员变量
// 
//																			2021/10/11	
//---------------------------------version: 0.1.2-------------------------------------
// 
// 
// 
//		TODO:	
//		1.	前后置自增"++"自减"--"，
//		2.	矩阵除法 "/"
//		3.	关于数值是否要转化为矩阵的问题
//			1）涉及到运算的拓展问题，如三角函数等  
//			2）涉及到函数的简化问题，将单独的数字看作矩阵可以减少函数重载的数量，
//			   将所有的运算变为只含矩阵类的运算，并且可以将矩阵与数字的加减法统
//				一到广播当中
//			3) 但是也有问题，如果允许了广义的广播比如（6*4）+（3*2）这样的矩阵运算，
//			   那么本应该是错误的运算就会正常通过
//		4.	做完运算符重载后，关于一些辅助函数的重构问题
//																		
//		
// 
// 
//************************************************************************************


typedef std::initializer_list<double> MatIniLst;	//初始化列表

class Matrix
{
	// ---- 成员变量 ----
private:
	int miRow = 0;		//行数
	int miCol = 0;		//列数
	double* mpBuf = nullptr;	//存储矩阵

public:
	// ---- 构造析构函数 ----

	/**
	*	@brief 无参数构造函数
	* 
	*	获取一个0*0的未分配内存的空矩阵
	*	不合法的计算一般返回本矩阵
	*/
	explicit Matrix();

	/**
	*	@brief 构造函数
	*   将double类型转化为1*1矩阵
	*
	*	@param num 数字
	*/
	Matrix(double num);

	/**
	*	@brief 构造函数
	*	使用初始化列表构造矩阵
	*
	*	@param lst 初始化列表
	*/
	Matrix(const MatIniLst& lst);


	/**
	*	@brief 构造函数
	*	使用初始化列表构造矩阵，列数为最多的一行的元素个数，可以以0补足其它行的元素
	*
	*	@param lst 初始化列表（两层）
	*/
	Matrix(const std::initializer_list<MatIniLst>& lst);

	/**
	*	@brief 构造函数
	*	使用初始化列表构造矩阵，根据设定的列数，可以以0补足初始化列表中没有的元素
	*
	*	@param lst 初始化列表（两层）
	*	@param col 矩阵列数
	*/
	explicit Matrix(const std::initializer_list<MatIniLst>& lst, int col);

	/**
	*	@brief 构造函数
	*	使用指针所指的数据构造矩阵,若数组长度小于需要的长度row*col，则会报错
	*
	*	@param p double型指针，是要输入的数据
	*	@param row 矩阵行数
	*	@param col 矩阵列数
	*   @param count 数组长度
	*/
	explicit Matrix(double* p, int row, int col, int count);

	/**
	*	@brief 拷贝构造函数
	*
	*/
	Matrix(const Matrix& tmp);

	/**
	*	@brief 移动构造函数
	*
	*/
	Matrix(Matrix&& tmp);

	/**
	*	@brief 析构函数
	*/
	~Matrix();

	// ---- 静态成员函数 ----

	/**
	*	@brief 静态函数
	*	返回一个方阵，用于生成单位方阵
	*
	*	@param row 方阵的阶数
	*	@return 返回单位方阵
	*/
	static Matrix eye(int row);

	/**
	*	@brief 静态函数
	*	返回一个矩阵，用于生成单位矩阵
	*
	*	@param row 矩阵的行数
	*	@param col 矩阵的列数
	*	@return 返回单位矩阵
	*/
	static Matrix eye(int row, int col);

	/**
	*	@brief 静态函数
	*	返回一个方阵，用于生成元素全为1的方阵
	*
	*	@param row 方阵的阶数
	*	@return 返回元素全为1的方阵
	*/
	static Matrix ones(int row);

	/**
	*	@brief 静态函数
	*	返回一个方阵，用于生成元素全为1的矩阵
	*
	*	@param row 矩阵的行数
	*	@param col 矩阵的列数
	*	@return 返回元素全为1的矩阵
	*/
	static Matrix ones(int row, int col);

	/**
	*	@brief 静态函数
	*	返回一个方阵，用于生成元素全为0的方阵
	*
	*	@param row 方阵的阶数
	*	@return 返回元素全为0的方阵
	*/
	static Matrix zeros(int row);


	/**
	*	@brief 静态函数
	*	返回一个方阵，用于生成元素全为0的矩阵
	*
	*	@param row 矩阵的行数
	*	@param col 矩阵的列数
	*	@return 返回元素全为0的矩阵
	*/
	static Matrix zeros(int row, int col);

	// ---- 运算符重载 ----

	/**
	*	@brief 运算符重载，拷贝赋值函数
	*	将矩阵赋值
	*
	*	@param tmp 待赋值的矩阵
	*	@return 返回赋值之后的该矩阵的引用
	*/
	Matrix& operator=(const Matrix& tmp);

	/**
	*	@brief 运算符重载，移动赋值函数
	*	将矩阵赋值
	*
	*	@param tmp 待赋值的矩阵
	*	@return 返回赋值之后的该矩阵的引用
	*/
	Matrix& operator=(Matrix&& tmp);

	/**
	*	@brief 运算符重载,友元函数
	*	将本矩阵与所给矩阵相加求和
	*
	*	@param lMat 左加矩阵
	*	@param rMat 右加矩阵
	*	@return 返回求和之后的该矩阵的引用
	*/
	friend Matrix operator+(const Matrix& lMat, const Matrix& rMat);

	/**
	*	@brief 运算符重载,友元函数
	*	将本矩阵与所给矩阵相减求差
	*
	*	@param lMat 左被减矩阵
	*	@param rMat 右减矩阵
	*	@return 返回求差之后的矩阵
	*/
	friend Matrix operator-(const Matrix& lMat, const Matrix& rMat);

	/**
	*	@brief 运算符重载,友元函数
	*	将本矩阵与所给矩阵相乘求矩阵乘积
	*
	*	@param lMat 左乘矩阵
	*	@param rMat 右乘矩阵
	*	@return 返回求矩阵乘积之后的新矩阵
	*/
	friend Matrix operator*(const Matrix& lMat, const Matrix& rMat);

	/**
	*	@brief 运算符重载,友元函数
	*	double*matrix
	*	返回一个矩阵
	*
	*	@param lFactor 左乘的系数
	*	@param rMat 矩阵
	*	@return 返回矩阵数乘后的结果
	*/
	friend Matrix operator*(const double& lFactor, const Matrix& rMat);

	/**
	*	@brief 运算符重载,友元函数
	*   matrix*double
	*	返回一个矩阵
	* 
	*	@param lMat 矩阵
	*	@param rFactor 右乘的系数
	*	@return 返回矩阵数乘后的结果
	*/
	friend Matrix operator*(const Matrix& lMat, const double& rFactor);

	/**
	*	@brief 运算符重载,友元函数
	*	将本矩阵与所给矩阵相除求矩阵乘积
	*
	*	@param lMat 被除矩阵
	*	@param rMat 右除矩阵
	*	@return 返回求矩阵相除之后的矩阵
	*/
	friend Matrix operator/(const Matrix& lMat, const Matrix& rMat);

	/**
	*	@brief 运算符重载,友元函数
	*	double/matrix
	*	返回一个矩阵
	*
	*	@param lFactor 被除的系数
	*	@param rMat 右除矩阵
	*	@return 返回矩阵相除后的结果
	*/
	friend Matrix operator/(const double& lFactor, const Matrix& rMat);

	/**
	*	@brief 运算符重载,友元函数
	*   matrix/double
	*	返回一个矩阵
	*
	*	@param lMat 被除的矩阵
	*	@param rFactor 右除系数
	*	@return 返回矩阵相除后的结果
	*/
	friend Matrix operator/(const Matrix& lMat, const double& rFactor);

	/**
	*	@brief 运算符重载,友元函数
	*	matrix+double
	*	返回一个矩阵
	*
	*	@param lMat 左加的矩阵
	*	@param rMat 右加的数字
	*	@return 返回矩阵广播后的结果
	*/
	friend Matrix operator+(const Matrix& lMat, const double& rNum);
	
	/**
	*	@brief 运算符重载,友元函数
	*	double+matrix
	*	返回一个矩阵
	*
	*	@param lNum 左加的数字
	*	@param rMat 右加的矩阵
	*	@return 返回矩阵广播后的结果
	*/
	friend Matrix operator+(const double& lNum, const Matrix& rMat);
	
	/**
	*	@brief 运算符重载,友元函数
	*	matrix-double
	*	返回一个矩阵
	*
	*	@param lMat 被减的矩阵
	*	@param rMat 减的数字
	*	@return 返回矩阵广播后的结果
	*/
	friend Matrix operator-(const Matrix& lMat, const double& rNum);
	
	/**	
	*	@brief 运算符重载,友元函数
	*	double-matrix
	*	返回一个矩阵
	*
	*	@param lNum 被减的数字
	*	@param rMat 减的矩阵
	*	@return 返回矩阵广播后的结果
	*/
	friend Matrix operator-(const double& lNum, const Matrix& rMat);

	/**
	*	@brief 运算符重载
	*   复合赋值+=
	*
	*	@param tmp 要加上的矩阵
	*	@return 返回结果矩阵的引用
	*/
	Matrix& operator+=(const Matrix& tmp);

	/**
	*	@brief 运算符重载
	*   复合赋值+= 
	*
	*	@param tmp 要加上的矩阵
	*	@return 返回结果该矩阵的引用
	*/
	Matrix& operator+=(const double& num);

	/**
	*	@brief 运算符重载
	*   复合赋值-=
	*
	*	@param tmp 要减去的矩阵
	*	@return 返回结果矩阵的引用
	*/
	Matrix& operator-=(const Matrix& tmp);

	/**
	*	@brief 运算符重载
	*   复合赋值+=
	*
	*	@param tmp 要减去的数字
	*	@return 返回结果矩阵的引用
	*/
	Matrix& operator-=(const double& num);
	
	/**
	*	@brief 运算符重载,友元函数
	*   判等
	*
	*	@param lMat 左矩阵
	*	@param rMat 右矩阵
	*	@return 返回判等结果
	*/
	friend bool operator==(const Matrix& lMat, const Matrix& rMat);
	/**
	*	@brief 运算符重载,友元函数
	*   判不等
	*
	*	@param lMat 左矩阵
	*	@param rMat 右矩阵
	*	@return 返回判不等结果
	*/
	friend bool operator!=(const Matrix& lMat, const Matrix& rMat);

	/**
	*	@brief 运算符重载,友元函数
	*   取相反
	*
	*	@param Mat 矩阵
	*	@return 返回取反结果
	*/
	friend Matrix operator-(const Matrix& Mat);
	/**
	*	@brief 运算符重载,友元函数
	*   不变
	*
	*	@param Mat 矩阵
	*	@return 返回不变的结果
	*/
	friend Matrix operator+(const Matrix& Mat);


	/**
	*	@brief 运算符重载
	*   前置递增
	*
	*	@return 递增后的矩阵引用
	*/
	Matrix& operator++();
	
	/**
	*	@brief 运算符重载
	*   后置递增
	*	
	*	@param 区分前后置的参数
	*	@return 递增前的矩阵
	*/
	Matrix operator++(int);
	
	/**
	*	@brief 运算符重载
	*   前置递减
	*
	*	@return 递减后的矩阵引用
	*/
	Matrix& operator--();
	
	/**
	*	@brief 运算符重载
	*   后置递减
	*
	*	@param 区分前后置的参数
	*	@return 递减前的矩阵
	*/
	Matrix operator--(int);


	/**
	*	@brief 运算符重载
	*   取下标
	*	返回下标对应的矩阵的某一行
	*   一般连续使用两个[]以获取某个指针
	*   不要在类的方法中使用取下标运算符！！！
	*	有两个版本，此为非常量版本
	*
	*	@param num 下标
	*	@return 返回下标对应的矩阵的某一行的第一个元素的指针
	*/
	double* operator[](int num);
	/**
	*	@brief 运算符重载
	*   取下标
	*	返回下标对应的矩阵的某一行
	*   一般连续使用两个[]以获取某个指针
	*   不要在类的方法中使用取下标运算符！！！
	*	有两个版本，此为常量版本
	*
	*	@param num 下标
	*	@return 返回下标对应的矩阵的某一行的第一个元素的指针
	*/
	const double* operator[](int num)const;

	/**
	*	@brief 运算符重载,友元函数
	*   用于实现cout<< xxx << xxx
	*	返回ostream的引用
	*
	*  @param os ostream
	*  @param tmp 矩阵
	*  @return 返回os的引用即ostream&
	*/
	friend std::ostream& operator<<(std::ostream& os,  const Matrix& tmp);
	



	// ↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓
	// 施工中！！！施工中！！！施工中！！！施工中！！！
	// 施工中！！！施工中！！！施工中！！！施工中！！！
	// 施工中！！！施工中！！！施工中！！！施工中！！！
	// 施工中！！！施工中！！！施工中！！！施工中！！！
	// ↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓


public:
	int getRow();
	int getCol();

	const Matrix add(const Matrix& tmp)const;
	const Matrix sub(const Matrix& tmp)const;
	const Matrix rMultiple(const Matrix& tmp)const;
	const Matrix sMultiple(const double& factor)const;

	const Matrix transpose()const;
	bool det(double& result)const;
	Matrix gauss();
	const Matrix inverse()const;
	int rank();

	bool sizeValidityCheck();
	void rAdd(int des, int src, double factor);
	bool rExchange(int des, int src);
	void rTime(int des, double factor);
	Matrix merge(Matrix& right)const;
	Matrix divide(int col);
	static Matrix LRMerge(Matrix& left, Matrix& right);
	static Matrix UDMerge(Matrix& up, Matrix& down);



};




#endif // !_Matrix
