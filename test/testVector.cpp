#include "../Vector.h"
#include <iostream>
#include <iomanip>
#include <memory>

using namespace std;
using namespace sp;

template <typename T>
void printContent(const Vector<T> &v, const string &s)
{
    cout << "the capacity of " << s << " : " << setw(2) << v.capacity(); 
    cout << " | content : ";
    for (const auto &x : v) {
        cout << x << " ";
    }
    if (v.size() == 0) {
        cout << "null";
    }
    cout << endl;
}

int main()
{
    cout << "=======================test constructor=======================" << endl;
    Vector<int> a, b(10), c{0, 2, 4, 6, 8}, d(static_cast<Vector<int>::size_type>(5), 10), e{c.begin(), c.end()};
    Vector<int> hehe{static_cast<Vector<int>::size_type>(2), 10, std::allocator<int>{}};
    printContent(hehe, "hehe");
    printContent(a, "a");
    printContent(b, "b");
    printContent(c, "c");
    printContent(d, "d");
    printContent(e, "e");
    cout << "===========================================================" << endl;

    cout << "===================test push_back() size() capacity()===================" << endl;
    cout << "push sequence: [0, 20)" << endl;
    for (int i = 0; i < 20; ++i) {
        a.push_back(i);
        cout << setw(2) << i << " pushed | size:" 
             << setw(2) << a.size() << " capacity:" 
             << setw(2) << a.capacity() << endl;
    }
    printContent(a, "a");
    cout << "===========================================================" << endl;

    cout << "=======================test copyConstructor copyAssign=======================" << endl;
    Vector<int> h(a);
    b = a;
    printContent(h, "h");
    printContent(b, "b");
    cout << "===========================================================" << endl;

    cout << "=======================test moveConstructor moveAssign=======================" << endl;
    Vector<int> i(std::move(h));
    Vector<int> j;
    j = std::move(c);
    printContent(i, "i");
    printContent(j, "j");
    cout << "===========================================================" << endl;

    return 0;
}
