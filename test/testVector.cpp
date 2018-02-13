#include "../Vector.h"
#include <iostream>
#include <iomanip>
#include <memory>

using namespace std;
using namespace sp;

template <typename T>
void printContent(const Vector<T> &v, const string &op, const string &name)
{
    cout << setw(40) << op;
    cout << " | the capacity of " << name << " : " << setw(2) << v.capacity(); 
    cout << " | content : ";
    for (const auto &x : v) {
        cout << x << " ";
    }
    if (v.size() == 0) {
        cout << "null";
    }
    cout << endl;
}

int symbolCount;

void printHead(const string &title)
{
    string::size_type count = 140 - title.size();

    symbolCount = count / 2;
    string s(symbolCount, '=');
    symbolCount = symbolCount * 2 + title.size();
    cout << s << title << s << endl; 
}

void printTail()
{ cout << string(symbolCount, '=') << endl; }

int main()
{
    printHead("test constructor");
    Vector<int> a, b(10), c{0, 2, 4, 6, 8}, d(static_cast<Vector<int>::size_type>(5), 10), e{c.begin(), c.end()};
    Vector<int> hehe{static_cast<Vector<int>::size_type>(2), 10, std::allocator<int>{}};

    printContent(hehe, "hehe(2, 10, allocator<int>{})", "hehe");
    printContent(a, "a", "a");
    printContent(b, "b(10)", "b");
    printContent(c, "c{0, 2, 4, 6, 8}", "c");
    printContent(d, "d(5, 10)", "d");
    printContent(e, "e{c.begin(), c.end()}", "e");
    printTail();

    printHead("test reserve shrink_to_fit");
    cout << "a.capacity() : " << a.capacity() << endl;
    a.reserve(100);
    cout << "before a.reserve(100) | a.capacity() : " << a.capacity() << endl;
    a.reserve(80);
    cout << "before a.reserve(80) | a.capacity() : " << a.capacity() << endl;
    a.shrink_to_fit();
    cout << "before a.shrink_to_fit() | a.capacity() : " << a.capacity() << endl;
    cout << "a.empty() : " << a.empty() << endl;
    printTail();
    printHead("test push_back size capacity");
    cout << "push sequence: [0, 20)" << endl;
    for (int i = 0; i < 20; ++i) {
        a.push_back(i);
        cout << setw(33) << i << " pushed | size:" 
             << setw(2) << a.size() << " capacity:" 
             << setw(2) << a.capacity() << endl;
    }
    printContent(a, "after push", "a");
    cout << "a.empty() : " << a.empty() << endl;
    printTail();

    printHead("test operator[] front back");
    for (Vector<int>::size_type i = 0; i < a.size(); ++i) {
        cout << "a[" << i << "] = " << a[i] << endl;
    }
    cout << "a.front():" << a.front() << endl 
         << "a.back():" << a.back() << endl;
    printTail();

    printHead("test copyConstructor copyAssign");
    Vector<int> h(a);
    b = a;
    printContent(h, "h(a)", "h");
    printContent(b, "b = a", "b");
    printTail();

    printHead("test moveConstructor moveAssign swap");
    Vector<int> i(std::move(h)), j;
    j = std::move(c);
    printContent(i, "i(std::move(h))", "i");
    printContent(j, "j = std::move(c)", "j");
    swap(i, j);
    printContent(i, "swap(i, j)", "i");
    printContent(j, "swap(i, j)", "j");
    printTail();

    printHead("test assign");
    a.assign(static_cast<Vector<int>::size_type>(5), 99);
    printContent(a, "assign(5, 99)", "a");
    a.assign(i.begin(), i.end());
    printContent(a, "assign(i.begin(), i.end())", "a");
    a.assign({9, 8, 7, 6, 5, 4, 3, 2});
    printContent(a, "assign({9, 8, 7, 6, 5, 4, 3, 2}", "a");
    printTail();

    printHead("test iterator");
    cout << setw(40) << "begin() end() : ";
    for (Vector<int>::iterator i = a.begin(); i != a.end(); ++i) {
        cout << *i << " ";
    }
    cout << endl;
    cout << setw(40) << "cbegin() cend() : ";
    for (Vector<int>::const_iterator i = a.cbegin(); i != a.cend(); ++i) {
        cout << *i << " ";
    }
    cout << endl;
    cout << setw(40) << "rbegin() rend() : ";
    for (Vector<int>::reverse_iterator i = a.rbegin(); i != a.rend(); ++i) {
        cout << *i << " ";
    }
    cout << endl;
    cout << setw(40) << "crbegin() crend() : ";
    for (Vector<int>::const_reverse_iterator i = a.crbegin(); i != a.crend(); ++i) {
        cout << *i << " ";
    }
    cout << endl;
    printTail();

    printHead("test clear");
    a.clear();
    printContent(a, "clear", "a");
    cout << "a.empty() : " << a.empty() << endl;
    printTail();

    printHead("test insert resize");
    a.insert(a.begin(), 5);
    printContent(a, "insert(a.begin(), 5)", "a");
    a.insert(a.end(), static_cast<Vector<int>::size_type>(7), 6);
    printContent(a, "insert(a.end(), 7, 6)", "a");
    a.erase(a.begin() + 3);
    printContent(a, "erase(a.begin() + 3)", "a");
    a.insert(a.end(), b.begin(), b.end() - 9);
    printContent(a, "insert(a.end(), b.begin(), b.end() - 9)", "a");
    a.erase(a.begin() + 4, a.end() - 2);
    printContent(a, "erase(a.begin() + 4, a.end() - 2)", "a");
    a.insert(a.begin() + 2, {1, 2, 3, 7, 99});
    printContent(a, "insert(a.begin() + 2, {1, 2, 3, 7, 99})", "a");
    a.resize(5);
    printContent(a, "resize(5)", "a");
    a.resize(7, 99999);
    printContent(a, "resize(7, 99999)", "a");
    printTail();

    Vector<int> v{1, 2, 3, 4, 5}, w{v};
    Vector<int> x{1, 2, 3, 4};

    cout << "v == w " << (v == w) << endl;
    cout << "v != w " << (v != w) << endl;
    cout << "v > w " << (v > w) << endl;
    cout << "v < w " << (v < w) << endl;
    cout << "v >= w " << (v >= w) << endl;
    cout << "v <= w " << (v <= w) << endl;
    cout << endl;
    cout << "v == x " << (v == x) << endl;
    cout << "v != x " << (v != x) << endl;
    cout << "v > x " << (v > x) << endl;
    cout << "v < x " << (v < x) << endl;
    cout << "v >= x " << (v >= x) << endl;
    cout << "v <= x " << (v <= x) << endl;
    cout << endl;
    cout << "a == x " << (a == x) << endl;
    cout << "a != x " << (a != x) << endl;
    cout << "a > x " << (a > x) << endl;
    cout << "a < x " << (a < x) << endl;
    cout << "a >= x " << (a >= x) << endl;
    cout << "a <= x " << (a <= x) << endl;

    return 0;
}
