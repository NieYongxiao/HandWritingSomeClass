#include"StringFunction.h"
#include <cassert>

//�����ַ���
char* self_strcpy(char* dest, const char* src) {
	//���ܳ����ڴ��ص�������srcĳЩֵ�����ǣ�������Ҫ�Ӻ���ǰ����
	//�˴�����д
	char* res = dest;
	assert(dest != nullptr);
	assert(src != nullptr);
	while (*src != '\0') {
		*dest = *src;
		dest++;
		src++;
	}
	*dest = '\0'; //�ֶ����Ͻ�������ע���ַ�����dest�Ĵ�С�����ϸ��scr�Ĵ�
	return res;
}

//׷���ַ���������
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

//�Ƚ��ַ���
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

//����src��һ�γ�����dest��λ��
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