#include <iostream>
#include <chrono>
#include <cstdlib>
#include <pthread.h>
#include <time.h>
#include "skiplist_implement.h"

#define NUM_THREADS 1
#define TEST_COUNT 10000000
SkipList<int, std::string> skipList(20);

void *insertnode(void* threadid) {

    auto tid = (long long)threadid;
    std::cout << tid << std::endl;
    int tmp = TEST_COUNT/NUM_THREADS;
    for (int count=0,i = tid*tmp; count<tmp; count++,i++)
        skipList.insert_node(rand() % TEST_COUNT, "a");
    pthread_exit(NULL);
}

void *getnode(void* threadid) {
    auto tid = (long long )threadid;
    std::cout << tid << std::endl;
    int tmp = TEST_COUNT/NUM_THREADS;
    for (int count=0; count<tmp; count++)
        skipList.search_node(rand() % TEST_COUNT);
    pthread_exit(NULL);
}

int main() {
//    srand (time(NULL));
//    {
//        pthread_t threads[NUM_THREADS];
//        int rc;
//        int i;
//
//        auto start = std::chrono::high_resolution_clock::now();//高分辨率时钟,精确到纳秒
//
//        for( i = 0; i < NUM_THREADS; i++ ) {
//            std::cout << "main() : creating thread, " << i << std::endl;
//            rc = pthread_create(&threads[i], NULL, insertnode, (void *)i);
//
//            if (rc) {
//                std::cout << "Error:unable to create thread," << rc << std::endl;
//                exit(-1);
//            }
//        }
//
//        void *ret;
//        for( i = 0; i < NUM_THREADS; i++ ) {
//            if (pthread_join(threads[i], &ret) !=0 )  {
//                perror("pthread_create() error");
//                exit(3);
//            }
//        }
//        auto finish = std::chrono::high_resolution_clock::now();
//        std::chrono::duration<double> elapsed = finish - start;
//        std::cout << "insert elapsed:" << elapsed.count() << std::endl;
//    }
    // skipList.displayList();

     {
         pthread_t threads[NUM_THREADS];
         int rc;
         int i;
         auto start = std::chrono::high_resolution_clock::now();

         for( i = 0; i < NUM_THREADS; i++ ) {
             std::cout << "main() : creating thread, " << i << std::endl;
             rc = pthread_create(&threads[i], NULL, getnode, (void *)i);

             if (rc) {
                 std::cout << "Error:unable to create thread," << rc << std::endl;
                 exit(-1);
             }
         }

         void *ret;
         for( i = 0; i < NUM_THREADS; i++ ) {
             if (pthread_join(threads[i], &ret) !=0 )  {
                 perror("pthread_create() error");
                 exit(3);
             }
         }

         auto finish = std::chrono::high_resolution_clock::now();
         std::chrono::duration<double> elapsed = finish - start;
         std::cout << "get elapsed:" << elapsed.count() << std::endl;
     }

    pthread_exit(NULL);
    return 0;

}