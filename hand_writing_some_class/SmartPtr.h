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
	//���캯��
	SmartPtr();
	SmartPtr(T* ptr);
	//�������캯��
	SmartPtr(const SmartPtr&);
	//�ƶ����캯��
	SmartPtr(SmartPtr&&);
	//�ƶ���ֵ����
	void operator=(SmartPtr&&);
	//��ͷ����
	T* operator->() const;
	//������
	T& operator*() const;
	//��������
	~SmartPtr();

	//����bool�����
	operator bool();

	T* get();

	size_t use_count();

	bool unique();

	void swap(SmartPtr&);
};
