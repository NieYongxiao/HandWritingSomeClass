#ifndef _SKIP_LIST_H
#define _SKIP_LIST_H

#include "Node.h"
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <cstring>
#include <mutex>
#include <fstream>


#define STORE_FILE "store/dumpFile"

std::mutex mtx;
std::string delimiter = ":";


template <typename K, typename V>
class SkipList{
public: 
    SkipList(int);
    ~SkipList();
    int size();
    void display_list();
public:
    void dump_file();
    void load_file();
public:
    int insert_element(K,V);
    bool search_element(K);
    void delete_element(K);
private:
    void get_key_value_from_string(const std::string& str,std::string* key,std::string*  value);
    bool is_valid_string(const std::string& str);
    int get_random_level();
    Node<K,V>* create_node(K,V,int);
private:
    // 跳表的最高层数
    int _max_level;

    // 目前的层数
    int _skip_list_level;

    // 头节点指针
    Node<K, V> *_header;

    // 文件操作符
    std::ofstream _file_writer;
    std::ifstream _file_reader;

    // 跳表当前数据和
    int _element_count;
};

//首先给出一些简单和后续需要的一些函数定义
template<typename K,typename V>
SkipList<K,V>::SkipList(int max_level)
{
    this->_max_level = max_level;
    this->_skip_list_level = 0;
    this->_element_count = 0;

    K k;
    V v;
    this->_header = new Node<K,V>(k,v,_max_level);
}

template<typename K,typename V>
SkipList<K,V>::~SkipList()
{
    if(_file_writer.is_open()) _file_writer.close();
    if(_file_reader.is_open()) _file_reader.close();

    delete _header;
}

template<typename K,typename V>
int SkipList<K,V>::size()
{
    return _element_count;
}

template<typename K,typename V>
void SkipList<K,V>::get_key_value_from_string(const std::string& str,std::string* key,std::string* value)
{
    if( !is_valid_string(str) ) return;

    *key = str.substr(0, str.find(delimiter));
    *value = str.substr(str.find(delimiter),str.length());
}

template<typename K,typename V>
bool SkipList<K,V>::is_valid_string(const std::string& str)
{
    if(str.empty()) return false;
    if(str.find(delimiter) == std::string::npos) return false;

    return true;
}

template<typename K,typename V>
int SkipList<K,V>::get_random_level()
{
    int k=1;
    while( rand()%2 ) k++;
    k = (k < _max_level) ? k : _max_level;
    return k;
}

template<typename K,typename V>
Node<K,V>* SkipList<K,V>::create_node(K k,V v,int level)
{
    Node<K,V>* n = new Node<K,V>(k,v,level);
    return n;
}

template<typename K,typename V>
void SkipList<K,V>::display_list()
{
    std::cout << "\n*****Skip List*****"<<"\n"; 
    for(int i=0; i<=_skip_list_level; i++)
    {
        Node<K,V> *node = this->_header->forward[i];
        std::cout<<"Level "<<i<<": ";
        while(node != nullptr)
        {
            std::cout<<node->get_key()<<" "<<node->get_value()<<";";
            node = node->forward[i];
        }
        std::cout<<std::endl;
    }
}


// 核心函数：对跳表进行插入删除搜索
// 在跳过列表中插入给定的键和值
// 返回 1 表示元素存在 
// 返回 0 表示插入成功
/* 
                           +------------+
                           |  insert 50 |
                           +------------+
level 4     +-->1+                                                      100
                 |
                 |                      insert +----+
level 3         1+-------->10+---------------> | 50 |          70       100
                                               |    |
                                               |    |
level 2         1          10         30       | 50 |          70       100
                                               |    |
                                               |    |
level 1         1    4     10         30       | 50 |          70       100
                                               |    |
                                               |    |
level 0         1    4   9 10         30   40  | 50 |  60      70       100
                                               +----+

*/


template<typename K, typename V>
int SkipList<K,V>::insert_element(const K key, const V value)
{
    mtx.lock();
    Node<K,V>* current = this->_header;

    //创建更新数组并对其进行初始化，更新是放置节点的数组，node->forward[i]稍后应操作
    Node<K,V>* update[_max_level+1];
    memset(update,0,sizeof(Node<K,V>*) * (_max_level+1));

    for(int i= _skip_list_level; i>=0; i--)
    {
        while(current->forward[i] != nullptr && current->forward[i]->get_key() < key)
            current = current->forward[i];
        update[i] = current;
    }

    //此时以到达level 0 
    current = current->forward[0];

    if(current != nullptr && current->get_key() == key)
    {
        std::cout<<"key: "<<key <<", exists"<<std::endl;
        mtx.unlock();
        return 1;
    }

    if(current == nullptr || current->get_key() != key)
    {
        int random_level = get_random_level();

        if(random_level > _skip_list_level)
        {
            for(int i= _skip_list_level+1; i< random_level+1;++i)
                update[i] = _header;
            _skip_list_level = random_level;
        }

        Node<K,V>* inserted_node = create_node(key,value,random_level);

        for(int i=0; i<=random_level;++i)
        {
            inserted_node->forward[i] = update[i]->forward[i];
            update[i]->forward[i] = inserted_node;
        }
        std::cout << "Successfully inserted key:" << key << ", value:" << value << std::endl;
        _element_count++;
    }
    mtx.unlock();
    return 0;
}

template<typename K,typename V>
void SkipList<K,V>::delete_element(K key)
{
    mtx.lock();
    Node<K,V>* current = this->_header;
    Node<K,V> *update[_max_level+1];
    memset(update , 0,sizeof(Node<K,V>*) * (_max_level+1) );

    for(int i=_skip_list_level;i>=0;i--)
    {
        while(current->forward[i] != nullptr && current->forward[i]->get_key() < key )
            current = current->forward[i];
        update[i] = current;
    }

    current = current->forward[0];
    if(current != nullptr && current->get_key() == key)
    {
        for(int i=0;i<=_skip_list_level;++i)
        {
            if(update[i]->forward[i] != current) 
                break;
            update[i]->forward[i] = current->forward[i];
        }

        while(_skip_list_level > 0 && _header->forward[_skip_list_level] == 0)
            _skip_list_level--;
        
        std::cout<<"Successful deleted key" <<key <<std::endl;
        _element_count --;
    }
    mtx.unlock();
    
    return;
}

template<typename K, typename V> 
bool SkipList<K, V>::search_element(K key) {

    std::cout << "search_element-----------------" << std::endl;
    Node<K, V> *current = _header;

    // start from highest level of skip list
    for (int i = _skip_list_level; i >= 0; i--) {
        while (current->forward[i] && current->forward[i]->get_key() < key) {
            current = current->forward[i];
        }
    }

    //reached level 0 and advance pointer to right node, which we search
    current = current->forward[0];

    // if current node have key equal to searched key, we get it
    if (current and current->get_key() == key) {
        std::cout << "Found key: " << key << ", value: " << current->get_value() << std::endl;
        return true;
    }

    std::cout << "Not Found Key:" << key << std::endl;
    return false;
}


//最后需要将数据从内存存入磁盘

template<typename K,typename V>
void SkipList<K,V>::dump_file()
{
    std::cout << "dump_file-----------------" << std::endl;
    _file_writer.open(STORE_FILE);
    Node<K,V>* node = this->_header->forward[0];

    while(node != NULL)
    {
        _file_writer << node->get_key() <<":" << node->get_value()<<"\n";
        std::cout << node->get_key() << ":" << node->get_value() <<";\n";
        node = node->forward[0];
    }
    _file_writer.flush();
    _file_writer.close();

    return ;
}

template<typename K, typename V>
void SkipList<K, V>::load_file()
{
    _file_reader.open(STORE_FILE);
    std::cout<<"load_file-----------------" << std::endl;
    std::string line;
    std::string* key = new std::string();
    std::string* value = new std::string();
    while(getline(_file_reader,line))
    {
        get_key_value_from_string(line, key, value);
        if(key->empty() || value->empty()) 
            continue;
        insert_element(*key,*value);

        std::cout << "key:" << *key << "value:" << *value << std::endl;
    }
    _file_reader.close();
}



#endif