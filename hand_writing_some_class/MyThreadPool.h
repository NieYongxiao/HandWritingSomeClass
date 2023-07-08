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
	MyThreadPool(size_t);  //构造，向threads中添加thread，thread的构造函数需要一个函数用于执行，
						   //而我们用schedual作为线程执行的函数，
						   //schedual中则执行get_one_task 获取tasks中要执行的任务
	~MyThreadPool();  //析构，threads.join()
	void shut_down();  //停止submit
	void restart();  //恢复submit
	template<class Function,class... Args>
	auto submit(Function&& f, Args&&... args) ->future<decltype(f(args...))>;   //提交需要完成的函数  可变参数
private:
	Task get_one_task();
	void schedual();
};

#endif