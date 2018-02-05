//Created by sphc on 2018/2/4
//Vector.h
//

#ifndef SP_VECTOR__H
#define SP_VECTOR__H

#include <cstddef> //size_t, ptrdiff_t
#include <climits> //UINT_MAX
#include <memory> //allocator

namespace sp {

    template <typename T, typename Allocator = std::allocator<T>>
    class Vector {
    public:
        typedef T value_type; 
        typedef Allocator allocator_type;
        typedef value_type &reference;
        typedef const value_type &const_reference;
        typedef typename std::allocator_traits<Allocator>::pointer pointer;
        typedef typename std::allocator_traits<Allocator>::const_pointer const_pointer;
        typedef value_type *iterator;
        typedef const value_type *const_iterator;
        typedef ptrdiff_t difference_type;
        typedef size_t size_type;

        explicit Vector(size_type theCapacity = 0) 
        { 
            finish = start = new value_type[theCapacity + initCapacity]; 
            termination = start + theCapacity + initCapacity;
        }

        Vector(const Vector &other)
        { alloc_copy(other); }

        Vector(Vector &&other) 
            : start{other.start}, finish{other.finish}, termination{other.termination}
        { other.start = other.finish = other.termination = nullptr; }

        ~Vector()
        { delete []start; }

        Vector &operator = (const Vector &other)
        {
            if (this != &other) {
                delete []start;
                alloc_copy(other);
            }
        }

        Vector &operator = (Vector &&other)
        {
            if (this != &other) {
                delete []start;
                start = other.start;
                finish = other.finish;
                termination = other.termination;
                other.start = other.finish = other.termination = nullptr; 
            }
        }

        void assign(size_type count, const value_type &value)
        {
            finish = start;
            while (count-- && finish != termination) {
                push_back(value);
            }
        }

        reference operator [] (size_type index)
        { return start[index]; }

        const_reference operator [] (size_type index) const
        { return start[index]; }

        value_type *data() noexcept
        { return start; }

        const value_type *data() const noexcept
        { return start; }

        reference front()
        { return *start; }

        const_reference front() const
        { return *start; }

        reference back()
        { return *(finish - 1); }

        const_reference back() const 
        { return *(finish - 1); }

        iterator begin() noexcept
        { return start; }

        const_iterator begin() const noexcept
        { return start; }

        iterator end()
        { return finish; }

        const_iterator end() const
        { return finish; }

        const_iterator cbegin() const noexcept
        { return static_cast<const_iterator>(start); }

        const_iterator cend() const noexcept
        { return static_cast<const_iterator>(finish); }

        size_type size() const noexcept
        { return static_cast<size_type>(finish - start); }

        size_type max_size() const noexcept
        { return static_cast<size_type>(UINT_MAX / sizeof(value_type)); }

        size_type capacity() const noexcept
        { return static_cast<size_type>(termination - start); }

        bool empty() const noexcept
        { return start == finish; }

        void push_back(const value_type &value)
        {
            if (finish >= termination) {
                realloc();
            }

            *finish++ = value;
        }

        void push_back(value_type &&value)
        {
            if (finish >= termination) {
                realloc();
            }

            *finish++ = std::move(value);
        }

        void pop_back()
        { --finish; }

        iterator insert(const_iterator pos, const value_type &value)
        {
            if (finish >= termination) {
                difference_type diff = pos - start;
                realloc();
                pos = start + diff;
            }

            iterator tmp = end();

            while (tmp != pos) {
                *tmp-- == *(tmp - 1);
            }
            *tmp = value;

            return tmp;
        }

        iterator insert(const_iterator pos, value_type &&value)
        {
            if (finish >= termination) {
                difference_type diff = pos - start;
                realloc();
                pos = start + diff;
            }

            value_type tmp = end();

            while (tmp != pos) {
                *tmp-- == *(tmp - 1);
            }
            *tmp = std::move(value);

            return tmp;
        }

        iterator insert(const_iterator pos, size_type count, const value_type &value)
        {
            difference_type diff = pos - start;

            while (size() + count > capacity) {
                realloc();
            }

            pos = cbegin() + diff;
            iterator tmp = begin() + diff + count;
            while (tmp != pos) {
                *tmp-- = *(tmp - count);
            }
            while (count--) {
                *tmp-- = value;
            }

            return count ? tmp + 1 : tmp;
        }

        iterator insert(const_iterator pos, std::initializer_list<value_type> ilist)
        {
            difference_type diff = pos - start;

            while (size() + ilist.size() > capacity) {
                realloc();
            }

            pos = cbegin() + diff;
            iterator tmp = begin() + diff + ilist.size();
            while (tmp != pos) {
                *tmp-- = *(tmp - ilist.size());
            }
            for (auto i = ilist.crbegin(); i != ilist.crend(); ++i) {
                *tmp-- = *i;
            }

            return ilist.size() ? tmp + 1 : tmp;
        }

    private:
        value_type *start; //start of memory
        value_type *finish; //next position of last element
        value_type *termination; //next position of last memory
        const static size_type initCapacity = 1;

        void alloc_copy(const Vector &other)
        {
            finish = start = new value_type[other.capacity()]; 
            termination = start + other.capacity();
            for (const auto &x : other) {
                push_back(x);
            }
        }

        void realloc()
        {
            value_type *newData = new value_type[2 * capacity()];
            value_type *tmp = newData;

            termination = newData + 2 * capacity();
            for (const auto &x : *this) {
                *tmp++ = std::move(x);
            }
            delete []start;
            start = newData;
            finish = tmp;
        }
    };

} //namespace sp

#endif //SP_VECTOR__H
