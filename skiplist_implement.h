#include <iostream>
#include <cstdlib>
#include <cmath>
#include <cstring>
#include <mutex>
#include <fstream>
#include "skiplist.h"
#define STORE_FILE "E:/C++/skiplist/store/dumpFile"

std::mutex mtx;     // 临界区互斥
std::string delimiter = ":";

template<typename K, typename V>
Node<K, V>::Node(const K k, const V v, int l):key(k),value(v),level(l) {
    // level + 1，因为数组索引是从0 - level开始的
    this->next = new Node<K, V>*[level+1];
    //用0(NULL)填充next数组
    memset(this->next, 0, sizeof(Node<K, V>*)*(level+1));
};

//析构释放该值指针内存
template<typename K, typename V>
Node<K, V>::~Node() {
    delete []next;
};

//获取键
template<typename K, typename V>
K Node<K, V>::get_key() const {
    return key;
};

//获取值
template<typename K, typename V>
V Node<K, V>::get_value() const {
    return value;
};

//更新值
template<typename K, typename V>
void Node<K, V>::set_value(V value) {
    this->value=value;
};

//构造跳表
template<typename K, typename V>
SkipList<K, V>::SkipList(int max_level):_max_level(max_level),_skip_list_level(0),_node_count(0){
    //创建头节点，并初始化key和value为null
    K k;
    V v;
    this->_header = new Node<K, V>(k, v, _max_level);
};

template<typename K, typename V>
SkipList<K, V>::~SkipList() {

    if (_file_writer.is_open()) {
        _file_writer.close();
    }
    if (_file_reader.is_open()) {
        _file_reader.close();
    }
    delete _header;
}

template<typename K, typename V>
int SkipList<K, V>::get_random_level(){
    int k = 1;

    while (rand() % 2)
    {
        k++;
    }
    k = (k < _max_level) ? k : _max_level;
    return k;
};

//创建新节点
template<typename K, typename V>
Node<K, V>* SkipList<K, V>::create_node(const K k, const V v, int level) {
    Node<K, V> *n = new Node<K, V>(k, v, level);
    return n;
}

//查找节点
template<typename K, typename V>
bool SkipList<K, V>::search_node(K key) {
    std::cout << "---------search_node---------" << std::endl;
    Node<K, V> *current = _header;
    for (int i = _skip_list_level; i >= 0; i--) {
        while (current->next[i] && current->next[i]->get_key() < key) {
            current = current->next[i];
        }
    }
    current = current->next[0];
    if (current and current->get_key() == key) {
        std::cout << "Found key: " << key << ", value: " << current->get_value() << std::endl;
        return true;
    }
    std::cout << "Not Found Key:" << key << std::endl;
    return false;
}

//在跳跃列表中插入给定的键和值
template<typename K, typename V>
int SkipList<K, V>::insert_node(const K key, const V value)
{
    mtx.lock();
    Node<K, V> *current = this->_header;

    //创建并初始化更新数组
    //update是将node->next[i]放到以后操作的节点的数组
    Node<K, V> *update[_max_level+1];
    memset(update, 0, sizeof(Node<K, V>*)*(_max_level+1));

    //从当前最高的跳表开始
    for(int i = _skip_list_level; i >= 0; i--) {
        while(current->next[i] != NULL && current->next[i]->get_key() < key) {
            current = current->next[i];
        }
        update[i] = current;
    }

    //已经达到0级，向下指针指向右节点，需要插入键
    current = current->next[0];

    //如果当前节点的key等于搜索的key，我们得到它
    if (current != NULL && current->get_key() == key) {
        std::cout << "key: " << key << ", exists" << std::endl;
        current->set_value(value);
        mtx.unlock();
        return 1;
    }

    //如果current为NULL，这意味着我们已经到达关卡的末尾
    //如果当前的key不等于key，这意味着我们必须在更新的[0]和当前节点之间插入节点
    if (current == NULL || current->get_key() != key ) {

        //为node生成一个随机级别
        int random_level = get_random_level();
        //如果随机级别大于跳跃列表的当前级别，则用指向头的指针初始化更新值
        if (random_level > _skip_list_level) {
            for (int i = _skip_list_level+1; i < random_level+1; i++) {
                update[i] = _header;
            }
            _skip_list_level = random_level;
        }

        //创建一个随机生成的节点
        Node<K, V>* inserted_node = create_node(key, value, random_level);

        //插入节点
        for (int i = 0; i <= random_level; i++) {
            inserted_node->next[i] = update[i]->next[i];
            update[i]->next[i] = inserted_node;
        }
        std::cout << "Successfully inserted key:" << key << ", value:" << value << std::endl;
        _node_count++;
    }
    mtx.unlock();
    return 0;
}

//删除节点
template<typename K, typename V>
void SkipList<K, V>::delete_node(K key) {
    mtx.lock();
    Node<K, V> *current = this->_header;
    Node<K, V> *update[_max_level+1];
    memset(update, 0, sizeof(Node<K, V>*)*(_max_level+1));


    for (int i = _skip_list_level; i >= 0; i--) {
        while (current->next[i] !=NULL && current->next[i]->get_key() < key) {
            current = current->next[i];
        }
        update[i] = current;
    }

    current = current->next[0];
    if (current != NULL && current->get_key() == key) {
        for (int i = 0; i <= _skip_list_level; i++) {
            if (update[i]->next[i] != current)
                break;
            update[i]->next[i] = current->next[i];
        }
        while (_skip_list_level > 0 && _header->next[_skip_list_level] == 0) {
            _skip_list_level --;
        }
        std::cout << "Deleted successfully: "<< key << std::endl;
        _node_count --;
    }
    else
        std::cout << "No node to delete was found: " <<key <<  std::endl;
    mtx.unlock();
    return;
}

//展示已存数据
template<typename K, typename V>
void SkipList<K, V>::display_list() {

    std::cout << "\n---------SkipList---------"<<"\n";
    for (int i = 0; i <= _skip_list_level; i++) {
        Node<K, V> *node = this->_header->next[i];
        std::cout << "Level " << i << ": ";
        while (node != NULL) {
            std::cout << node->get_key() << ":" << node->get_value() << ";";
            node = node->next[i];
        }
        std::cout << std::endl;
    }
}

//数据落盘
template<typename K, typename V>
void SkipList<K, V>::dump_file() {

    std::cout << "---------dump_file---------" << std::endl;
    _file_writer.open(STORE_FILE);
    Node<K, V> *node = this->_header->next[0];

    while (node != NULL) {
        _file_writer << node->get_key() << ":" << node->get_value() << "\n";
        std::cout << node->get_key() << ":" << node->get_value() << ";\n";
        node = node->next[0];
    }

    _file_writer.flush();
    _file_writer.close();
    return ;
}

//加载数据
template<typename K, typename V>
void SkipList<K, V>::load_file() {

    _file_reader.open(STORE_FILE);
    std::cout << "---------load_file---------" << std::endl;
    std::string line;
    std::string key;
    std::string value;
    while (getline(_file_reader, line)) {
        get_key_value_from_string(line, key, value);
        if (key.empty() || value.empty()) {
            continue;
        }
        insert_node(stoi(key), value);
        std::cout << "key:" << key << "    value:" << value << std::endl;
    }
    _file_reader.close();
}

//获取当前跳表的大小
template<typename K, typename V>
int SkipList<K, V>::size() {
    return _node_count;
}

template<typename K, typename V>
void SkipList<K, V>::get_key_value_from_string(const std::string& str, std::string &key, std::string &value) {

    if(!is_valid_string(str)) {
        return;
    }
    key = str.substr(0, str.find(delimiter));
    value = str.substr(str.find(delimiter)+1, str.length());
}

template<typename K, typename V>
bool SkipList<K, V>::is_valid_string(const std::string& str) {

    if (str.empty()) {
        return false;
    }
    if (str.find(delimiter) == std::string::npos) {
        return false;
    }
    return true;
}

