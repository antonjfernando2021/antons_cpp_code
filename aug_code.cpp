#include <iostream>
#include <vector>
#include <sstream>
#include <map>
#include <memory>

template <typename T>
class mySP
{
public:
    mySP()
    {
        refcount = 1;
        std::cout << __func__ << ": inside default constructor:  refcount is: " << refcount << std::endl;
        ptr = new T;
    } //; //= default;

    mySP(T args)
    {
        refcount = 1;
        std::cout << __func__ << ": inside default constructor: refcount is: " << refcount << std::endl;
        ptr = new T(args);
    }


    mySP<T>& operator=(mySP<T>& a)
    {

        refcount = a.refcount + 1;
        std::cout << __func__ << ": inside assign constructor:  refcount is: " <<  refcount << std::endl;
        delete ptr;
        ptr = a.ptr;
        return *this;
    }


    mySP (const mySP<T>& a) {
        
        refcount = a.refcount + 1;
        std::cout << __func__ << ": inside copy constructor:  refcount is: " <<  refcount << std::endl;
        ptr = a.ptr;
    }

    ~mySP()
    {
        refcount--;
        if ((refcount == 0) && (ptr != nullptr))
        {
            std::cout << __func__ << ": inside default destructor: delete ptr, refcount is: " << refcount << std::endl;
            delete ptr;
            ptr = nullptr;
        }
        else
        {
            std::cout << __func__ << ": inside default destructor: refcount is: " << refcount << std::endl;
        }
    } //= default;
    T operator*()
    {
        return *ptr;
    }
    T *operator->()
    {
        return ptr;
    }

private:
    uint32_t refcount;
    T *ptr{nullptr};
};

template<typename T1, typename T2>
auto add(T1 a, T2 b) -> decltype (a+b)
{
    return a + b;
}

using namespace std;
void dosomething();
void testTraits();
void testSP();
void testmemoryleak();
void testdecltype();

int main(int argc, char *argv[])
{
    dosomething();
    // std::cout << "*up is again " << *up << std::endl;

    testTraits();

    testSP();

    testmemoryleak();

    testdecltype();
}

void testdecltype()
{
std::cout << __func__ << ": 1   + 2 : " << add (1,2) << std::endl;
std::cout << __func__ << ": 1.2 + 2 : " << add (1.2,2) << std::endl;
std::cout << __func__ << ": 1 + 2.2 : " << add (1,2.2) << std::endl;
}

//
//
//
void testSP()
{

    mySP<int> sp = mySP<int>(3.0);
    std::cout << __func__ << ": *sp is " << *sp << std::endl;

    typedef struct justint
    {
        int a{4};
    } justint_t;

    using mytypeT = justint_t;
    mytypeT t;
    t.a = 5;
    auto sps = mySP<mytypeT>(t);
    std::cout << __func__ << ": *sps->a: is " << sps->a << std::endl;

    
    mySP<int> sp2 = mySP<int>(4.0);
    std::cout << __func__ << ": default *sp2 is " << *sp2 << std::endl;

   
    std::cout << "about to call copy assignment operator for sp2" << std::endl;
    sp2 = sp;
    std::cout << __func__ << ": *sp2 is " << *sp2 << std::endl;

    std::cout << "about to call copy constructor for sp3" << std::endl;
    mySP<int> sp3 = sp;
    std::cout << __func__ << ": *sp3 is " << *sp3 << std::endl;
}
//
//
//
void dosomething()
{
    cout << "master" << endl;
    std::vector<int> vec = {1, 2, 3};
    for (auto const e : vec)
    {
    std:;
        cout << e << std::endl;
    }

    std::cout << __func__ << ": vec[0] is " << vec[0] << std::endl;
    // std::cout << "vec[5000] is " << vec[5000] << std::endl;

    auto up = make_unique<int>();
    // up = 5;
    std::cout << __func__ << ": *up is " << *up << std::endl;

    up = nullptr;
}
//
//
//
void testmemoryleak()
{
    // int* p = new int(1);
    //std::cout << __func__ << ": p is new int allocated but not deleted " << std::endl;
}
//
//
//
void testTraits()
{
    std::cout << __func__ << ": is float<int>: " << std::is_floating_point<int>::value << std::endl;
    std::cout << __func__ << ": is float<float>: " << std::is_floating_point<float>::value << std::endl;
}
