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
        std::cout << __func__ << ": inside default constructor" << std::endl;
        refcount = 1;
        ptr = new T;
    } //; //= default;

    explicit mySP(T args)
    {
        std::cout << __func__ << ": inside default constructor" << std::endl;
        refcount = 1;
        ptr = new T(args);
    }

    ~mySP()
    {
        std::cout << __func__ << ": inside default destructor" << std::endl;
        refcount--;
        if (refcount == 0)
        {
            std::cout << __func__ << ": inside default destructor: delete ptr, refcount is zero" << std::endl;
            delete ptr;
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
    T *ptr;
};

using namespace std;
void dosomething();
void testTraits();
void testSP();
void testmemoryleak();

int main(int argc, char *argv[])
{
    dosomething();
    // std::cout << "*up is again " << *up << std::endl;

    testTraits();

    testSP();

    testmemoryleak();
}

//
//
//
void testSP()
{
    // int x =3;
    auto sp = mySP<double>(3.0);
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
    std::cout << __func__ << ": p is new int allocated but not deleted " << std::endl;
}
//
//
//
void testTraits()
{
    std::cout << __func__ << ": is float<int>: " << std::is_floating_point<int>::value << std::endl;
    std::cout << __func__ << ": is float<float>: " << std::is_floating_point<float>::value << std::endl;
}
