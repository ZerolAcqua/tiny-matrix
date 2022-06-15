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
//-----------------------------------------------------------------------------------
// 
//		1.	整理注释，修改变量名称
//      2.	优化修改了部分的构造函数和静态函数
//			1)eye
//			2)ones
//			3)zeros
//		3.	修正了移动构造函数和移动赋值函数实现
//																			2021/10/02
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
//																			2021/10/05
// 
// 
//		1.	矩阵广播（只做了与数字的）
//		2.	复合赋值"+=""-="
//		3.	单元运算符"+/-"
//																			2021/10/09	
//	
//		1. 前后置自增"++"自减"--"，
//		2. 矩阵除法 "/"  
//		3. 修改了部分辅助函数，加上const关键字，使其不能修改成员变量
//		TODO:	
//		1.	关于数值是否要转化为矩阵的问题
//			1）涉及到运算的拓展问题，如三角函数等  
//			2）涉及到函数的简化问题，将单独的数字看作矩阵可以减少函数重载的数量，
//			   将所有的运算变为只含矩阵类的运算，并且可以将矩阵与数字的加减法统
//				一到广播当中
//			3) 但是也有问题，如果允许了广义的广播比如（6*4）+（3*2）这样的矩阵运算，
//			   那么本应该是错误的运算就会正常通过
//		2.	做完运算符重载后，关于一些辅助函数的重构问题
//		3.	分块矩阵
//		4.	精度和打印相关的参数
//		5.	reshape相关代码
//																			2021/10/11	
//-----------------------------------------------------------------------------------
// 
//		1.	整理添加了部分辅助函数的注释，并进行很小的改动
//		TODO:
//		1.	计算矩阵的特征值（QR算法？？）
//		2.	矩阵的幂，矩阵多项式计算不是梦！
//		3.	分块矩阵
//		4.	精度和打印相关的参数
//		5.	reshape相关代码
//																			2022/01/17																	
// 
//		1.	修改了浮点数判等的逻辑
//		2.	添加了矩阵形状检测的相关辅助函数
//		3.	添加diag函数
//		4.	添加了eigen函数，以及相关的hessenberg函数、qrDecom4Hessenberg函数
//			目前求矩阵的特征值只能计算实特征值，如果存在复特征值，就没办法计算了
//		5.	添加了randMatrix()函数，用于生成0~1伪随机元素组成的矩阵
//		TODO:
//		1.	想写一下异常抛出相关的东西，不然以后debug太累了，这大概又是一个不小的工程
//																			2022/01/21	
// 
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
	// ---- 静态成员变量 ----
	static int iPrecise;	// 控制流输出的显示精度

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

	/**
	*	@brief 静态函数
	*	由行、列向量生成对角矩阵，或者从一个矩阵获取对角线元素
	*
	*	@param mat 
	*	@return 返回对角矩阵，或由矩阵返回对角元素列向量
	*/
	static Matrix diag(Matrix mat);

	/**
	*	@brief 静态函数
	*	生成元素随机的方阵
	*
	*	@param n	方阵阶数
	*	@return		返回随机方阵
	*/
	static Matrix randMatrix(int n);

	/**
	*	@brief 静态函数
	*	生成元素随机的方阵
	*
	*	@param row	矩阵行数
	* 	@param col	矩阵列数
	*	@return		返回随机方阵
	*/
	static Matrix randMatrix(int row, int col);



	/**
	*	@brief 静态函数
	*	设置矩阵显示的精度
	*
	*	@param precise 精度
	*/
	static void setPrecise(int precise);


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
	*  @param os	ostream
	*  @param tmp	矩阵
	*  @return 返回os的引用即ostream&
	*/
	friend std::ostream& operator<<(std::ostream& os,  const Matrix& tmp);
	

public:

	/**
	*	@brief 公有函数
	*	
	*	@return 返回该矩阵的行数
	*/
	int getRow()const;

	/**
	*	@brief 公有函数
	*
	*	@return 返回该矩阵的列数
	*/
	int getCol()const;

	/**
	*	@brief 公有函数
	*	计算矩阵与本矩阵的和，与“+”等价
	*	
	*	@param tmp 加上的矩阵
	*	@return 返回求和后的结果
	*/
	Matrix add(const Matrix& tmp)const;

	/**
	*	@brief 公有函数
	*	计算本矩阵与矩阵的差，与“-”等价
	*
	*	@param tmp 减去的矩阵
	*	@return 返回求差后的结果
	*/
	Matrix sub(const Matrix& tmp)const;

	/**
	*	@brief 公有函数
	*	计算某矩阵右乘本矩阵，与“*”等价
	*
	*	@param tmp 右乘的矩阵
	*	@return 返回右乘后的结果
	*/
	Matrix rMultiple(const Matrix& tmp)const;

	/**
	*	@brief 公有函数
	*	计算矩阵数乘，与“*”等价
	*
	*	@param tmp 所乘的数
	*	@return 返回右乘后的结果
	*/
	Matrix sMultiple(const double& factor)const;

	/**
	*	@brief 公有函数
	*	矩阵转置
	*
	*	@return 返回转置的结果
	*/
	Matrix transpose()const;

	/**
	*	@brief 公有函数
	*	计算矩阵的行列式，不存在三行四列的行列式（雾）
	*
	*	@param result 返回行列式的值
	*	@return 能求行列式返回true，否则返回false
	*/
	bool det(double& result)const;

	/**
	*	@brief 公有函数
	*	对矩阵进行高斯消元
	*
	*	@return 返回高斯消元的结果
	*/
	Matrix gauss()const;

	/**
	*	@brief 公有函数
	*	矩阵求逆
	*
	*	@return 返回求逆的结果
	*/
	Matrix inverse()const;

	/**
	*	@brief 公有函数
	*	计算矩阵的秩
	*
	*	@return 返回矩阵的秩
	*/
	int rank()const;

	/**
	*	@brief 公有函数
	*	检测矩阵形状的有效性
	*
	*	@return 若矩阵的形状有效，返回true,
	*			否则返回false，并将矩阵置为空矩阵
	*/
	bool isSizeValidity();

	/*
	*	@brief	公有函数
	*	检测矩阵是否为方阵
	* 
	*	@return	若矩阵为方阵，返回true，
	*			否则返回false
	*/
	bool isSquareMatrix()const;

	/*
	*	@brief	公有函数
	*	检测矩阵是否为空矩阵
	*
	*	@return	若矩阵为空矩阵，返回true，
	*			否则返回false
	*/
	bool isEmptyMatrix()const;


	/**
	*	@brief 公有函数
	*	行线性相加
	*
	*	@param des		要加到的目标行
	*	@param src		作为被加的行
	*	@param factor	所乘上的线性因数
	*/
	void rowAdd(int des, int src, double factor);

	/**
	*	@brief 公有函数
	*	行交换
	*
	*	@return	（对行列而言）若变号返回true，
	*			否则返回false
	*/
	bool rowExchange(int des, int src);

	/**
	*	@brief 公有函数
	*	行数乘
	*
	*	@param des		要数乘的目标行
	*	@param factor	所乘上的线性因数
	*/
	void rowMultiply(int des, double factor);

	/**
	*	@brief 静态函数
	*	矩阵左右拼接
	* 
	*	@param left		拼接在左边的矩阵
	*	@param right	拼接在右边的矩阵
	*	@return			返回拼接后的矩阵
	*/
	static Matrix lrMerge(Matrix& left, Matrix& right);

	/**
	*	@brief 静态函数
	*	矩阵上下拼接
	* 
	*	@param up		拼接在上边的矩阵
	*	@param down		拼接在下边的矩阵
	*	@return			返回拼接后的矩阵
	*/
	static Matrix udMerge(Matrix& up, Matrix& down);

	/**
	*	@brief 公有函数
	*	从矩阵得到一块矩阵
	*
	*	@param startRowId	分出的矩阵左上角元素在原矩阵中的行号（从0起算）
	*	@param startColId	分出的矩阵左上角元素在原矩阵中的列号（从0起算）
	*	@param blockRow		分出的矩阵的行数
	*	@param blockCol		分出的矩阵的列数
	*	@return				返回分出的矩阵，
	*						若不能分出矩阵，返回空矩阵
	*/
	Matrix getBlock(int startRowId, int startColId, int blockRow, int blockCol)const;
	
	/**
	*	@brief 公有函数
	*	将矩阵的某一块设置为给定的矩阵
	*
	*	@param startRowId	给定的矩阵左上角元素在原矩阵中的行号（从0起算）
	*	@param startColId	给定的矩阵左上角元素在原矩阵中的列号（从0起算）
	*	@param block		给定的矩阵
	*	@return				返回本矩阵自身，
	*						若不能设置，则不做修改，依然返回本矩阵自身
	*/
	Matrix& setBlock(int startRowId, int startColId, const Matrix& block);

	/**
	*	@brief 公有函数
	*	将该矩阵化为上Hessenberg矩阵（方阵）
	* 
	*	@param Q		由本矩阵相似变换到上Hessenberg矩阵的对称正交阵
	*					H=Q^T*A*Q
	*	@return			与该矩阵相似的上Hessenberg矩阵
	*/
	Matrix hessenberg(Matrix& Q)const;

	/**
	*	@brief 公有函数
	*	对该矩阵进行QR分解
	*
	*	@param Q	正交矩阵
	*	@param R	广义上三角矩阵
	*/
	void qrDecom(Matrix& Q, Matrix& R)const;
	
	/*
	*	@brief 公有函数
	*	求矩阵的特征值（从大到小排列）
	*
	*	@return		返回特征值组成的列向量，从大到小排列
	*				迭代超限,存在复特征值等原因导致无法求解，则返回空矩阵
	*/
	Matrix eigen()const;


private:
	/**
	*	@brief 私有静态函数
	*	特殊的符号函数，用于求Hessenberg矩阵
	*
	*	@param	num		要输入的数
	*	@return			输入非负数返回1，输入负数返回-1
	*/
	static signed char sgn(double num);
	
	/**
	*	@brief 私有函数
	*	矩阵拼接（用于高斯消元法求逆）
	*
	*	@param right	要拼接在本矩阵右边的矩阵
	*	@return			拼接后的矩阵
	*/
	Matrix merge(Matrix& right)const;

	/**
	*	@brief 私有函数
	*	矩阵拆分（用于高斯消元法求逆）
	*
	*	@param col		拆分后左边矩阵最后一列对应的原矩阵列的下标
	*	@return			拆分后的右矩阵
	*/
	Matrix lrDivide(int col)const;

	/**
	*	@brief 私有函数
	*	对该Hessenberg矩阵（方阵）进行完全QR分解
	*	若该矩阵不为Hessenberg则会产生错误的结果
	*	在QR分解法求特征值使用
	*
	*	@param Q	正交矩阵
	*	@param R	上三角矩阵
	*	@return		如果可以进行完全QR分解，返回true
	*				否则返回false,且Q和R为空矩阵
	*/
	bool qrDecom4Hessenberg(Matrix& Q, Matrix& R)const;
};



#endif // !_Matrix
