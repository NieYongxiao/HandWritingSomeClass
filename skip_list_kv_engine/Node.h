#ifndef _NODE_H
#define _NODE_H

template<typename  K, typename V>
class Node{
public:
    Node(){}
    Node(K k, V v,int);
    ~Node();
    K get_key() const;
    V get_value() const;
    void set_value(V);    
    
    // Linear array to hold pointers to next node of different level
    Node<K,V> **forward;
private:
    int node_level;
    K key;
    V value;
};

template<typename K, typename V>
Node<K,V>::Node(K k, V v,int level)
{
    this->key = k;
    this->value = v;
    this->node_level = level;

    this->forward = new Node<K,V>*[level+1];

    memset(this->forward,0,sizeof(Node<K,V>*) * (level+1));
}

template<typename K, typename V>
Node<K,V>::~Node()
{
    delete[] this->forward;
}

template<typename K, typename V> 
K Node<K, V>::get_key() const 
{
    return key;
};

template<typename K, typename V> 
V Node<K, V>::get_value() const 
{
    return value;
};

template<typename K, typename V>
void Node<K,V>::set_value(V value)
{
    this->value = value;
}


#endif