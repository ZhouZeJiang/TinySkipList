#include "Node.h"
#include <fstream>

#ifndef SKIPLIST_SKIPLIST_H
#define SKIPLIST_SKIPLIST_H

//跳表
template <typename K, typename V>
class SkipList {
public:
    SkipList(int);
    ~SkipList();
    int get_random_level();
    Node<K, V>* create_node(K, V, int);
    int insert_node(K, V);//插入数据
    void display_list();//展示已存数据
    bool search_node(K);//查询数据
    void delete_node(K);//删除数据
    void dump_file();//数据写入
    void load_file();//加载数据
    int size();//返回数据规模
private:
    void get_key_value_from_string(const std::string& str, std::string &key, std::string &value);
    bool is_valid_string(const std::string& str);
private:
    //跳表的最大级别
    int _max_level;
    //跳表的当前级别
    int _skip_list_level;
    //指向头结点的指针
    Node<K, V> *_header;
    //文件操作
    std::ofstream _file_writer;
    std::ifstream _file_reader;
    //当前元素计数
    int _node_count;
};

#endif