#pragma once

#include<iostream>
using std::cout;
using std::endl;

//class SharedPtr {
//private:
//	int* m_ptr;
//	size_t* m_count;
//public:
//	SharedPtr() :m_ptr(nullptr), m_count(new size_t(0)){}
//	SharedPtr(int* ptr) :m_ptr(ptr), m_count(new size_t(1)){}
//};

template<typename T>
class SmartPtr {
private:
	T* m_ptr;
	size_t* m_count;
public:
	//构造函数
	SmartPtr();
	SmartPtr(T* ptr);
	//拷贝构造函数
	SmartPtr(const SmartPtr&);
	//移动构造函数
	SmartPtr(SmartPtr&&);
	//移动赋值函数
	void operator=(SmartPtr&&);
	//箭头运算
	T* operator->() const;
	//解引用
	T& operator*() const;
	//析构函数
	~SmartPtr();

	//重载bool运算符
	operator bool();

	T* get();

	size_t use_count();

	bool unique();

	void swap(SmartPtr&);
};
