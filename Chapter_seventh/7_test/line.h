//组合类练习,类的静态成员应用，友元函数，友元类的应用


//*******友元函数和友元类*****
//目的：不通过类中的成员函数去获取类中的私有数据成员，而是直接获取类中的私有数据成员，这样可以减少函数调用的开销
//C++的友元关系是单项的，即声明B是A的友元!=A是B的友元

#ifndef _LINE_           
#define _LINE_
#include <iostream>
#include <iomanip>
#include <cmath>
#include <cassert>
using namespace std;

#define Fri 1    //是否友元
#define Ref 1    //是否引用

class Point
{
#if Fri
	friend class Line;
#endif 

public: 
	Point(int a, int b);            //构造函数
//	Point() = default;             //默认构造函数
	Point() { cout << "default succeed" << endl; }
	Point(const Point &p);         //复制构造函数
	~Point() { cnt--; }
	int getX() const { return x; }       //成员函数
	int getY()  { return y; }
	void setx(int a) { x = a; }
	static void ShowCnt(){ cout << "Object Cnt = " << cnt << endl; }   //静态函数成员，用于处理静态数据成员

	friend float dist(const Point &a, const Point &b);       //声明dist函数为Point类的友元函数,并不是该类的成员函数
	                                             //声明过后，这个函数可以使用类中的私有成员
private:
	int x, y;
	static int cnt;                 //静态数据成员声名，用于记录点的个数，
	                                //是这个类类型所共有的，不归某个类对象所有
};
int Point::cnt = 0;                 //静态数据成员必须在类外定义和初始化

Point::Point(int a, int b)                     //构造函数实现
	:x(a),y(b)
{
	cnt++;
	cout << "constructor Point: " << x << "," << y << endl;
}
Point::Point(const Point &p)                   //复制构造函数实现
{
	x = p.x;
	y = p.y;
	cnt++;
	cout << "constructor Point copy succeed!!" << endl;
}
float dist(const Point &a, const Point &b)                 //友元函数实现，在该函数里可以访问私有成员
{
	double x, y;
	x = a.x - b.x;
	y = a.y - b.y;
	return (float)(sqrt)(x*x + y*y);
}

//组合类声明
class Line                                //类中的成员是另一个类的对象
{
public:
#if Ref
	Line(Point &pstarst, Point &pend);     //可以传引用
	Line(int a, int b, int c, int d);      //也可以不用传对象，直接传部件类所需的参数也可以
#else
	Line(Point pstarst, Point pend);       //constructor function
#endif // Ref

	Line(const Line &L);                   //copy constructor function
	float getDis();

private:
	Point  Lstart,Lend;                   //私有成员是别的类的对象，声明顺序决定了构造函数初始化的顺序

};

#if Ref
Line::Line(Point &pstarst, Point &pend)      //构造函数实现，括号里是参数列表，冒号后面是初始化列表，大括号里面是函数实现
	:Lstart(pstarst), Lend(pend)             //初始化的顺序跟初始化列表的顺序无关，由Line类中的私有成员变量声明的顺序有关
{
	cout << "constructor Line" << endl;      //Line的构造函数运行完后，系统自动析构pstarst，pend
}
Line::Line(int a, int b, int c, int d)
	: Lstart(a, b), Lend(c, d)
{}
#else
Line::Line(Point pstarst, Point pend)     //构造函数实现，括号里是参数列表，冒号后面是初始化列表，大括号里面是函数实现
	: Lstart(pstarst), Lend(pend)             //初始化的顺序跟初始化列表的顺序无关，由Line类中的私有成员变量声明的顺序有关
{
	cout << "constructor Line" << endl;      //Line的构造函数运行完后，系统自动析构pstarst，pend
}
#endif // Ref



Line::Line(const Line &L)                 //复制构造函数
	:Lstart(L.Lstart),Lend(L.Lend)
{
	cout << "constructor Line copy succeed!!" << endl;
}
float Line::getDis()                     //类Line里的成员函数实现
{
	double x, y;

#if !Fri
	x = Lend.getX() - Lstart.getX();
	y = Lend.getY() - Lstart.getY();
#else
	//下面这两句等同于上面两句，原因是将Line设为Point的友元函数，
	//所以Line中的Point类对象就可以直接调用Point类中的私有数据成员
	x = Lend.x - Lstart.x;
	y = Lend.y - Lstart.y;
#endif // !Fri

	return (float)(sqrt)(x*x + y*y);
}


class ArrayofPoint      //动态数组类
{
public:
	ArrayofPoint(int size);
	ArrayofPoint(const ArrayofPoint&v);
	~ArrayofPoint();

	Point& element(int index)
	{
		assert(index >= 0 && index < size);  //assert的作用是现计算表达式 expression ，如果其值为假（即为0），那么它先向stderr打印一条出错信息，
		                                     //然后通过调用 abort 来终止程序运行。
		return points[index];                //只能通过传引用来调用points内的成员函数
	}

private:
	Point *points;
	int size;

};

ArrayofPoint::ArrayofPoint(int size)
	:size(size)
{
//	points = new Point;                         //为points指针申请一个Point类的内存空间
	points = new Point[size];                   //为points指针申请一组Point类的内存空间
}

ArrayofPoint::ArrayofPoint(const ArrayofPoint&v) //深层复制构造
{
	size = v.size;
	points = new Point[size];
	for (int i = 0; i < size; i++)
	{
		points[i] = v.points[i];
	}
}

ArrayofPoint::~ArrayofPoint()
{
	cout << "Deleting........" << endl;
	delete[] points;
}


#endif // !_LINE_
