#include"StringFunction.h"
#include <cassert>

//复制字符串
char* self_strcpy(char* dest, const char* src) {
	//可能出现内存重叠，导致src某些值被覆盖，所以需要从后往前复制
	//此处懒得写
	char* res = dest;
	assert(dest != nullptr);
	assert(src != nullptr);
	while (*src != '\0') {
		*dest = *src;
		dest++;
		src++;
	}
	*dest = '\0'; //手动加上结束符，注意字符数组dest的大小必须严格比scr的大
	return res;
}

//追加字符串到后面
char* self_strcat(char* dest, const char* src) {
	char* res = dest;
	assert(dest != nullptr);
	assert(src != nullptr);
	while (*dest != '\0') 
		dest++;
	while(*src != '\0'){
		*(dest++) = *(src++);
	}
	*dest = '\0';
	return res;
}

//比较字符串
int self_strcmp(const char* dest, const char* src) {
	assert(dest != nullptr);
	assert(src != nullptr);
	while (*dest != '\0' && *src != '\0') {
		if (*dest > *src) return 1;
		else if (*dest < *src) return -1;
		dest++;
		src++;
	}
	if (*dest == '\0' && *src == '\0') return 0;
	if (*dest == '\0') return -1;
	return 1;
}

//查找src第一次出现在dest的位置
char* self_strstr(char* dest,char* src) {
	char* res = dest;
	assert(dest != nullptr);
	assert(src != nullptr);
	if (*src == '\0') return nullptr;
	while (*dest != '\0') {
		char* s1 = dest, * s2 = src;
		while (*s1 != '\0' && *s2 != '\0' && *s1 == *s2)
			++s1, ++s2;
		if (*s2 == '\0') 
			return src;
		if (*s1 == '\0')
			return nullptr;
		dest++;
	}
	return nullptr;
}