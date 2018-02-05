#include "../Vector.h"
#include <iostream>
#include <iomanip>

using namespace std;
using namespace sp;

template <typename T>
void testCapacity(const Vector<T> &v, const string &s)
{ cout << "the capacity of " << s << " : " << v.capacity() << endl; }

int main()
{
    Vector<int> a, b(20);

    cout << "=======================test constructor=======================" << endl;
    testCapacity(a, "a");
    testCapacity(b, "b");
    cout << "===========================================================" << endl;

    cout << "===================test push_back() size() capacity()===================" << endl;
    cout << "push sequence: [0, 20)" << endl;
    for (int i = 0; i < 20; ++i) {
        a.push_back(i);
        cout << setw(2) << i << " pushed | size:" 
             << setw(2) << a.size() << " capacity:" 
             << setw(2) << a.capacity() << endl;
    }
    cout << "result:";
    for (const auto &x : a) {
        cout << x << " ";
    }
    cout << endl;
    cout << "===========================================================" << endl;

    cout << "=======================test copyConstructor copyAssign=======================" << endl;
    Vector<int> c(a);
    b = a;
    testCapacity(c, "c");
    cout << "content:";
    for (const auto &x : c) {
        cout << x << " ";
    }
    cout << endl;
    testCapacity(b, "b");
    cout << "content:";
    for (const auto &x : b) {
        cout << x << " ";
    }
    cout << endl;
    cout << "===========================================================" << endl;

    cout << "=======================test moveConstructor moveAssign=======================" << endl;
    Vector<int> d(std::move(b));
    Vector<int> e;
    e = std::move(c);
    testCapacity(d, "d");
    cout << "content:";
    for (const auto &x : d) {
        cout << x << " ";
    }
    cout << endl;
    testCapacity(e, "e");
    cout << "content:";
    for (const auto &x : e) {
        cout << x << " ";
    }
    cout << endl;
    cout << "===========================================================" << endl;

    return 0;
}
