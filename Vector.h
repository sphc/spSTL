//Created by sphc on 2018/2/4
//Vector.h
//

#ifndef SP_VECTOR__H
#define SP_VECTOR__H

#include <cstddef> //size_t, ptrdiff_t
#include <climits> //UINT_MAX

//T:CopyAssignable, CopyConstructible, Erasable
template <typename T, typename Alloc = std::allocator<T>>
class Vector {
public:
    typedef T value_type; 
    typedef Alloc allocator_type;
    typedef value_type &reference;
    typedef const value_type &const_reference;
    typedef std::allocator_traits<Allocator>::pointer pointer;
    typedef std::allocator_traits<Allocator>::const_pointer const_pointer;
    typedef value_type *iterator;
    typedef const value_type *const_iterator;
    typedef ptrdiff_t difference_type;
    typedef size_t size_type;

    explicit Vector(size_type theCapacity) 
    { 
        finish = start = new value_type[theCapacity]; 
        termination = start + theCapacity;
    }

    Vector(const Vector &other)
    {
        finish = start = new value_type[other.capacity()]; 
        termination = start + other.capacity();
        for (const auto &x : other) {
            push_back(x);
        }
    }

    Vector(Vector &&other) 
        : start{other.start}, finish{other.finish}, termination{other.termination}
    { other.start = other.finish = other.termination = nullptr; }

    ~Vector()
    { delete []start; }

    Vector &operator = (const Vector &other)
    {
        if (*this != other) {
            delete []start;
            finish = start = new value_type[other.capacity()];
            termination = start + other.capacity();
            for (const auto &x : other) {
                push_back(x);
            }
        }
    }

    Vector &operator = (Vector &&other)
    {
        if (*this != other) {
            delete []start;
            start = other.start;
            finish = other.finish;
            termination = other.termination;
            other.start = other.finish = other.termination = nullptr; 
        }
    }

    iterator begin()
    { return start; }

    iterator begin() const
    { return start; }

    iterator end()
    { return finish; }

    iterator end() const
    { return finish; }

    const_iterator cbegin() const
    { return static_cast<const_iterator>(start); }

    const_iterator cend() const
    { return static_cast<const_iterator>(finish); }

    size_type size() const
    { return static_cast<size_type>(finish - start); }

    size_type max_size() const
    { return static_cast<size_type>(UINT_MAX / sizeof(value_type)); }

    size_type capacity() const
    { return static_cast<size_type>(termination - start); }

    bool empty() const
    { return size() == 0; }

    reference operator [](size_type index)
    { return start[index]; }

    const_reference operator [](size_type index) const
    { return start[index]; }

    reference front()
    { return *start; }

    const_reference front() const
    { return *start; }

    reference back()
    { return *(finish - 1); }

    const_reference back() const 
    { return *(finish - 1); }

private:
    value_type *start; //start of memory
    value_type *finish; //next position of last element
    value_type *termination; //next position of last memory
};

#endif //SP_VECTOR__H
