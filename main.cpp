#include <iostream>
#include "skiplist_implement.h"

int main() {


    SkipList<int, std::string> skipList(6);
    srand((unsigned)time(NULL));
    skipList.insert_node(rand()%1000000, "zhou");
    skipList.insert_node(rand()%1000000,"mou");
    skipList.insert_node(rand()%1000000, "want");
    skipList.insert_node(rand()%1000000,"to");
    skipList.insert_node(rand()%1000000,"work");
    skipList.insert_node(50,"please");
    std::cout << std::endl;
    std::cout << "-------skipList size-------" <<std::endl << skipList.size() << std::endl;
    std::cout << std::endl;
    skipList.dump_file();

    //skipList.load_file();

    skipList.search_node(50);
    std::cout << std::endl;
    skipList.search_node(100);

    skipList.display_list();


    skipList.delete_node(50);
    skipList.delete_node(100);

    skipList.display_list();
}

