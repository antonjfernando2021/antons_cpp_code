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
using namespace std;


class ConsumerProducer {

public:
   ConsumerProducer() = default;
   void consumer ();
   void producer ();
   void getresults(void);
   void test() {
      f = p.get_future();
      std::thread getRes( [this] () {getresults();} );
      std::thread cons(   [this] () {consumer  ();} );
      std::thread prod(   [this] () {producer  ();} );
      cons.join();
      prod.join();
      getRes.join();
      std::cout << "test: leaving " << std::endl;
   }

private:
   std::mutex amutex;
   std::condition_variable cv;
   static constexpr int empty = 0;
   static constexpr int full = 10;
   int tank = full;
   int enough{};
   std::promise<int> p;
   std::future<int> f;
   bool done{false};
};
// nov 29th comments
int main () 
{
  ConsumerProducer a;
  a.test();
  return 0;
}

void ConsumerProducer::getresults (void) {
   f.wait(); 
   std::cout << "getResults: get() " << f.get() << std::endl;
   std::cout << "getResutls: closing bar: Done... " << std::endl;
   cv.notify_all();
}

void ConsumerProducer::consumer () {
    std::unique_lock <std::mutex> mylock(amutex, std::defer_lock);
    std::cout << "consumer: entering... " << std::endl;
	while (true) {
		mylock.lock();
		while (tank == empty) {
           std::cout << "consumer: tank is empty, waiting: tank: " << tank << std::endl;
		   cv.wait (mylock);
		}
        tank--;
        std::cout << "consumer: what's left: tank: " << tank << std::endl;
        std::this_thread::sleep_for (std::chrono::seconds(1));
        mylock.unlock();
        
        enough++;
        if(enough == 50) {
          done = true;
          std::cout << "consumer: leaving: drank enough Done... " << std::endl;
          p.set_value(50); // how much did the consumer consume? 
          break;
        }
        cv.notify_all();
	}
}

void ConsumerProducer::producer () {
    std::unique_lock <std::mutex> mylock(amutex, std::defer_lock);
    std::cout << "producer: entering... " << std::endl;
	while (true) {
		mylock.lock();
		while (tank != empty) {
           std::cout << "producer: tank is NOT empty, waiting: tank: " << tank << std::endl;
		   cv.wait (mylock);
           if(done){
              std::cout << "producer: bar closed: Done... " << std::endl;
              return;
           }
		}
  
        tank = full;
        std::cout << "producer: a full tank: tank: " << tank << std::endl;
        std::this_thread::sleep_for (std::chrono::seconds(1));
        mylock.unlock();
        cv.notify_all();
	}
}
