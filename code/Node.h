#ifndef SKIPLIST_NODE_H
#define SKIPLIST_NODE_H

//跳表节点
template<typename K, typename V>
class Node {
public:
    Node() {}
    Node(const K k,const V v,const int level);
    ~Node();
    K get_key() const;
    V get_value() const;
    void set_value(V v);
    //线性数组保存第n层指向下一个节点的指针
    //next[3]:当前节点在第三层的下一个指针节点
    Node<K, V> **next;

private:
    K key;
    V value;
    int level;
};

#endif

