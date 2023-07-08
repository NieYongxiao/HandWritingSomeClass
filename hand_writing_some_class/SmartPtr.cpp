 #include"SmartPtr.h"

template<typename T>
SmartPtr<T>::SmartPtr():m_ptr(nullptr),m_count(new size_t){
	cout << "无参构造函数" << endl;
}

template<typename T>
SmartPtr<T>::SmartPtr(T* ptr):m_ptr(ptr),m_count(new size_t(1)){
	cout << "含参构造函数" << endl;
}

template<typename T>
SmartPtr<T>::SmartPtr(const SmartPtr& sma_ptr) {
	m_ptr = sma_ptr.m_ptr;
	m_count = sma_ptr.m_count;
	++(*m_count);
	cout << "拷贝构造函数" << endl;
}

template<typename T>
SmartPtr<T>::SmartPtr(SmartPtr&& sma_ptr) {
	m_ptr = sma_ptr.m_ptr;
	m_count = sma_ptr.m_count;
	++(*m_count);
	cout << "移动构造函数" << endl;
}

template<typename T>
void SmartPtr<T>::operator=(SmartPtr&& sma_ptr) {
	m_ptr = sma_ptr.m_ptr;
	m_count = sma_ptr.m_count;
	++(*m_count); 
	cout << "移动赋值函数" << endl;
}

template<typename T>
SmartPtr<T>::~SmartPtr() {
	--(*m_count);
	cout << "*m_count:  "<<*m_count << endl;
	if (*m_count == 0) {
		delete m_count;
		delete m_ptr;
		m_count = nullptr;
		m_ptr = nullptr;
		cout << "count为0 析构函数" << endl;
	}
}

template<typename T>
T& SmartPtr<T>::operator*() const{
	return this->m_ptr;
}

template<typename T>
T* SmartPtr<T>::operator->() const {
	return *this->m_ptr;
}

template<typename T>
SmartPtr<T>::operator bool() {
	return m_ptr == nullptr;
}

template<typename T>
T* SmartPtr<T>::get() {
	return m_ptr;
}

template<typename T>
bool SmartPtr<T>::unique() {
	return (*m_count) == 1;
}

template<typename T>
size_t SmartPtr<T>::use_count() {
	return *m_count;
}

template<typename T>
void SmartPtr<T>::swap(SmartPtr& sma_ptr) {
	std::swap(*this, sma_ptr);
}