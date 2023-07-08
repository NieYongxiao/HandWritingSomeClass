#pragma once
#ifndef MY_THREAD_POOL_H
#define MY_THREAD_POOL_H

#include <vector>
#include <queue>
#include <atomic>
#include <future>
#include <condition_variable>
#include <thread>
#include <functional>
#include<exception>
using namespace std;

class MyThreadPool {
public:
	using Task = function<void()>;
public:
	atomic<bool> is_stop;
	queue<Task> tasks;
	vector<thread> threads;
	mutex mtx;
	condition_variable cv_task;
public:
	MyThreadPool(size_t);  //���죬��threads�����thread��thread�Ĺ��캯����Ҫһ����������ִ�У�
						   //��������schedual��Ϊ�߳�ִ�еĺ�����
						   //schedual����ִ��get_one_task ��ȡtasks��Ҫִ�е�����
	~MyThreadPool();  //������threads.join()
	void shut_down();  //ֹͣsubmit
	void restart();  //�ָ�submit
	template<class Function,class... Args>
	auto submit(Function&& f, Args&&... args) ->future<decltype(f(args...))>;   //�ύ��Ҫ��ɵĺ���  �ɱ����
private:
	Task get_one_task();
	void schedual();
};

#endif