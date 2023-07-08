#include"MyThreadPool.h"

MyThreadPool::MyThreadPool(size_t size):is_stop(false) {
	size = min(unsigned int(size < 1 ? 1 : size), thread::hardware_concurrency());
	for (int i = 0; i < size; ++i) {
		threads.push_back(thread(MyThreadPool::schedual));
	}
}

MyThreadPool::~MyThreadPool() {
	for (thread& t : threads) {   //此处引用，线程不可复制
		t.join();
	}
}

MyThreadPool::Task MyThreadPool::get_one_task(){
	unique_lock<mutex> lock(mtx);
	cv_task.wait(lock, [this]() {return !tasks.empty(); });
	Task task = move(tasks.front());
	tasks.pop();
	return task;
}

void MyThreadPool::schedual() {
	while (true) {
		Task task = get_one_task();
		task();
	}
}

void MyThreadPool::shut_down() {
	this->is_stop.store(true);
}

void MyThreadPool::restart() {
	this->is_stop.store(false);
}

template<class Function,class... Args>
auto MyThreadPool::submit(Function&& f,Args&&...args) ->future<decltype(f(args...))> {
	if (this->is_stop.load()) {
		throw runtime_error("Submit has been stopped,you can use restart() to restart ");
	}
	using ResType = decltype(f());
	auto task = make_shared<packaged_task<ResType()>>(bind(forward<Function>(f), forward<Args>(args)...));
	{
		unique_lock<mutex> lock(mtx);
		tasks.emplace([task]() {(*task)()});
	}
	cv_task.notify_all();
	future<ResType> res = task->get_future();
	return res;
}

//template<class Function,class... Args>
//auto MyThreadPool::submit(Function&& f, Args&&... args) ->future<decltype(f(args...))> {
//	if (this->is_stop.load()) {
//		throw runtime_error("Submit has been stopped,you can use restart() to restart it");
//	}
//	using ResType = decltype(f(args...));
//	//加() 因为packaged_task参数为函数签名，包括返回值和参数
//	auto task = make_shared<packaged_task<ResType()>>(bind(forward<Function>(f), forward<Args>(args)...));
//	{
//		lock_guard<mutex> lock(mtx);
//		tasks.emplace([task]() {
//			(*task)();
//			});   //★★★★★
//	}
//	cv_task.notify_all();
//	future<ResType> res = task->get_future();
//	return res;
//}