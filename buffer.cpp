#include <cstdio>
#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <thread>
#include <mutex>
#include <future>
#include <chrono>
#include <future>
#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds
#include <iostream>
#include <thread>
#include <chrono>
using namespace std;
//jan 20-2

class ringBuffer
{
    public:
        ringBuffer() = default;
        void Reader();
        void Writer();
        void startThreads();

    private:
        std::mutex mu;
        std::condition_variable consumer_waiting_till_not_empty; // not empty means, consumer can take this 
        std::condition_variable producer_waiting_till_not_full;  // not full, means producer can add things 
        std::array<int,4> ring; 
        int  count{0};
        int  tail{0}; // tail of the queue
        int  head{0}; // head of the queue
        int  number{100};
        const int BUF_SIZE{4};
};

void ringBuffer::Writer() // consumer 
{
    std::unique_lock <std::mutex> mylock(mu, std::defer_lock);
    ///std::cout << "Entering....consumer "<< std::endl;
    while (1)
    {
        mylock.lock();
        consumer_waiting_till_not_empty.wait( mylock, [this]() { return count > 0; } ); // predicate which returns false if the waiting should be continued 
        std::this_thread::sleep_for (std::chrono::seconds(5));
        // consumer a buffer
        printf("    zipWriter: consuming: ring[head:%d] = %d, count:%d\n", head, ring.at(head), count) ; 
        head =  (head + 1) % BUF_SIZE;
        count--;
        mylock.unlock();

        producer_waiting_till_not_full.notify_one(); // not full, ask producer to produce more 
        std::this_thread::sleep_for (std::chrono::seconds(1));
    }
}
void ringBuffer::startThreads()
{
    std::thread tcons( [this] () { Writer(); } );
    std::thread tprod( [this] () { Reader(); } );
    tprod.join();
    tcons.join();
}

void ringBuffer::Reader()  // producer 
{
    std::unique_lock <std::mutex> mylock(mu, std::defer_lock);
    //std::cout << "Entering....producer "<< std::endl;
    while (1)
    {
        mylock.lock();
        producer_waiting_till_not_full.wait( mylock, [this]() { return count < BUF_SIZE; } ); // predicate which returns false if the waiting should be continued 
        std::this_thread::sleep_for (std::chrono::seconds(1));
        // add a new buffer
        ring.at(tail) = number++;
        printf("zipReader: producing: ring[tail:%d] = %d, count:%d\n", tail, ring.at(tail), count) ; 
        tail = (tail + 1) % BUF_SIZE;
        count++;
        mylock.unlock();

        consumer_waiting_till_not_empty.notify_one(); // go consumer it, there is at least 1 in the ring buffer 
        std::this_thread::sleep_for (std::chrono::seconds(1));
    }
}

int main () 
{
    std::cout << "circular buffering ...." << std::endl;
    ringBuffer rb;
    //std::thread t1(ringBuffer::Reader, rb);
    //std::thread t2(ringBuffer::Writer, rb);
    rb.startThreads();
    return 0;
}
