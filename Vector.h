//Created by sphc on 2018/2/4
//Vector.h
//

#ifndef SP_VECTOR__H
#define SP_VECTOR__H

#include <cstddef> //size_t, ptrdiff_t
#include <climits> //UINT_MAX
#include <memory> //allocator, uninitializer_copy()
#include <initializer_list> //initializer_list

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
        typedef std::ptrdiff_t difference_type;
        typedef std::size_t size_type;
        class const_reverse_iterator {
        public:
            const_reverse_iterator(value_type *pvalue) 
                : data{pvalue}
            { }

            const_reverse_iterator &operator ++ ()
            {
                --data; 
                return *this;
            }

            const_reverse_iterator operator ++ (int)
            {
                const_reverse_iterator tmp = *this;
                --data;
                return tmp;
            }

            const_reverse_iterator &operator -- ()
            {
                ++data;
                return *this;
            }

            const_reverse_iterator operator -- (int)
            {
                const_reverse_iterator tmp = *this;
                ++data;
                return tmp;
            }

            const_reverse_iterator operator + (int step) const
            { return const_reverse_iterator{data - step}; }

            const_reverse_iterator operator - (int step) const
            { return const_reverse_iterator{data + step}; }

            const_reference operator * () const
            { return *data; }

        private:
            value_type *data;
        };

        class reverse_iterator : public const_reverse_iterator {
        public:
            reverse_iterator(value_type *pvalue) 
                : const_reverse_iterator(pvalue)
            { }

            reverse_iterator &operator ++ ()
            {
                --data; 
                return *this;
            }

            reverse_iterator operator ++ (int)
            {
                reverse_iterator tmp = *this;
                --data;
                return tmp;
            }

            reverse_iterator &operator -- ()
            {
                ++data;
                return *this;
            }

            reverse_iterator operator -- (int)
            {
                reverse_iterator tmp = *this;
                ++data;
                return tmp;
            }

            reverse_iterator operator + (int step) const
            { return reverse_iterator{data - step}; }

            reverse_iterator operator - (int step) const
            { return reverse_iterator{data + step}; }

            reference operator * () const
            { return *data; }
        };

        //constructor
        explicit Vector(const allocator_type &allocator = allocator_type{});
        explicit Vector(size_type count);
        Vector(size_type count, const value_type &value, 
                const allocator_type &allocator = allocator_type{});
        template<typename InputIterator>
        Vector(InputIterator first, InputIterator last,
               const Allocator &allocator = Allocator{}); 
        Vector(const Vector &other);
        Vector(const Vector &other, const allocator_type &allocator);
        Vector(Vector &&other); 
        Vector(Vector &&other, const allocator_type &allocator); 
        Vector(std::initializer_list<value_type> ilist, 
                const allocator_type &allocator = allocator_type{});
        ~Vector();

        //assign
        Vector &operator = (const Vector &other);
        Vector &operator = (Vector &&other);
        void assign(size_type count, const value_type &value);

        //get allocator
        allocator_type get_allocator() const;

        //access element
        reference operator [] (size_type index);
        const_reference operator [] (size_type index) const;
        reference front();
        const_reference front() const;
        reference back();
        const_reference back() const ;
        value_type *data() noexcept;
        const value_type *data() const noexcept;

        //iterator
        iterator begin() noexcept;
        iterator end() noexcept;
        const_iterator begin() const noexcept;
        const_iterator end() const noexcept;
        const_iterator cbegin() const noexcept;
        const_iterator cend() const noexcept;
        reverse_iterator rbegin() noexcept;
        reverse_iterator rend() noexcept;
        const_reverse_iterator rbegin() const noexcept;
        const_reverse_iterator rend() const noexcept;
        const_reverse_iterator crbegin() const noexcept;
        const_reverse_iterator crend() const noexcept;

        //capacity
        bool empty() const noexcept;
        size_type size() const noexcept;
        size_type max_size() const noexcept;
        size_type capacity() const noexcept;
        void sharink_to_fit();

        //update
        void push_back(const value_type &value);
        void push_back(value_type &&value);
        void pop_back();
        iterator insert(const_iterator pos, const value_type &value);
        iterator insert(const_iterator pos, value_type &&value);
        iterator insert(const_iterator pos, size_type count, const value_type &value);
        iterator insert(const_iterator pos, std::initializer_list<value_type> ilist);
        iterator erase(const_iterator pos);
        iterator erase(const_iterator first, const_iterator last);
        void resize(size_type count);
        void resize(size_type count, const value_type &value);
        void swap(Vector &other);

    private:
        value_type *start; //start of memory
        value_type *finish; //next position of last element
        value_type *termination; //next position of last memory
        Allocator alloc;
        const static size_type initCapacity = 1;

        void alloc_copy(const Vector &other);
        void reallocate();
        void free();
    };

    template <typename T, typename Allocator>
    Vector<T, Allocator>::Vector(const allocator_type &allocator) : Vector(0)
    { }

    template <typename T, typename Allocator>
    Vector<T, Allocator>::Vector(size_type count) : Vector(count, value_type{})
    { }

    template <typename T, typename Allocator>
    Vector<T, Allocator>::Vector(size_type count, const value_type &value, 
            const allocator_type &allocator)
        : alloc{allocator} 
    { 
        finish = start = alloc.allocate(count + initCapacity);
        termination = start + count + initCapacity;
    }

    template <typename T, typename Allocator>
    template<typename InputIterator>
    Vector<T, Allocator>::Vector(InputIterator first, InputIterator last,
           const Allocator &allocator) 
        : alloc{allocator}
    {
        start = alloc.allocate(static_cast<size_type>(last - first));
        termination = finish = std::uninitialized_copy(first, last, start);
    }

    template <typename T, typename Allocator>
    Vector<T, Allocator>::Vector(const Vector &other) 
        : alloc{std::allocator_traits<allocator_type>::
            select_on_container_copy_construction(other.get_allocator())} 
    { alloc_copy(other); }

    template <typename T, typename Allocator>
    Vector<T, Allocator>::Vector(const Vector &other, const allocator_type &allocator) 
        : alloc{allocator}
    { alloc_copy(other); }

    template <typename T, typename Allocator>
    Vector<T, Allocator>::Vector(Vector &&other) 
        : start{other.start}, finish{other.finish}, 
          termination{other.termination}, alloc{std::move(other.alloc)}
    { other.start = other.finish = other.termination = nullptr; }

    template <typename T, typename Allocator>
    Vector<T, Allocator>::Vector(Vector &&other, const allocator_type &allocator) 
        : alloc{allocator}
    {}

    template <typename T, typename Allocator>
    Vector<T, Allocator>::Vector(std::initializer_list<value_type> ilist, 
            const allocator_type &allocator) 
        : Vector(ilist.begin(), ilist.end(), allocator)
    { }

    template <typename T, typename Allocator>
    Vector<T, Allocator>::~Vector() 
    { free(); }

    template <typename T, typename Allocator>
    Vector<T, Allocator> &Vector<T, Allocator>::operator = (const Vector &other)
    {
        if (this != &other) {
            free();
            alloc_copy(other);
        }
    }

    template <typename T, typename Allocator>
    Vector<T, Allocator> &Vector<T, Allocator>::operator = (Vector &&other)
    {
        if (this != &other) {
            free();
            start = other.start;
            finish = other.finish;
            termination = other.termination;
            other.start = other.finish = other.termination = nullptr; 
        }
    }

    template <typename T, typename Allocator>
    void Vector<T, Allocator>::assign(size_type count, const value_type &value)
    {
        free();
        start = alloc.allocate(count);
        while (count--) {
            alloc.construct(finish++, value);
        }
    }

    template <typename T, typename Allocator>
    typename Vector<T, Allocator>::allocator_type Vector<T, Allocator>::get_allocator() const
    { return alloc; }

    template <typename T, typename Allocator>
    typename Vector<T, Allocator>::reference Vector<T, Allocator>::operator [] (size_type index)
    { return start[index]; }

    template <typename T, typename Allocator>
    typename Vector<T, Allocator>::const_reference Vector<T, Allocator>::operator [] (size_type index) const
    { return start[index]; }

    template <typename T, typename Allocator>
    typename Vector<T, Allocator>::value_type *Vector<T, Allocator>::data() noexcept
    { return start; }

    template <typename T, typename Allocator>
    const typename Vector<T, Allocator>::value_type *Vector<T, Allocator>::data() const noexcept
    { return start; }

    template <typename T, typename Allocator>
    typename Vector<T, Allocator>::reference Vector<T, Allocator>::front()
    { return *start; }

    template <typename T, typename Allocator>
    typename Vector<T, Allocator>::const_reference Vector<T, Allocator>::front() const
    { return *start; }

    template <typename T, typename Allocator>
    typename Vector<T, Allocator>::reference Vector<T, Allocator>::back()
    { return *(finish - 1); }

    template <typename T, typename Allocator>
    typename Vector<T, Allocator>::const_reference Vector<T, Allocator>::back() const 
    { return *(finish - 1); }

    template <typename T, typename Allocator>
    typename Vector<T, Allocator>::iterator Vector<T, Allocator>::begin() noexcept
    { return start; }

    template <typename T, typename Allocator>
    typename Vector<T, Allocator>::iterator Vector<T, Allocator>::end() noexcept
    { return finish; }

    template <typename T, typename Allocator>
    typename Vector<T, Allocator>::const_iterator Vector<T, Allocator>::begin() const noexcept
    { return start; }

    template <typename T, typename Allocator>
    typename Vector<T, Allocator>::const_iterator Vector<T, Allocator>::end() const noexcept
    { return finish; }

    template <typename T, typename Allocator>
    typename Vector<T, Allocator>::const_iterator Vector<T, Allocator>::cbegin() const noexcept
    { return static_cast<const_iterator>(start); }

    template <typename T, typename Allocator>
    typename Vector<T, Allocator>::const_iterator Vector<T, Allocator>::cend() const noexcept
    { return static_cast<const_iterator>(finish); }

    template <typename T, typename Allocator>
    typename Vector<T, Allocator>::reverse_iterator Vector<T, Allocator>::rbegin() noexcept
    { return reverse_iterator{finish - 1}; }

    template <typename T, typename Allocator>
    typename Vector<T, Allocator>::reverse_iterator Vector<T, Allocator>::rend() noexcept
    { return reverse_iterator{start - 1}; }

    template <typename T, typename Allocator>
    typename Vector<T, Allocator>::const_reverse_iterator Vector<T, Allocator>::rbegin() const noexcept
    { return const_reverse_iterator{finish - 1}; }

    template <typename T, typename Allocator>
    typename Vector<T, Allocator>::const_reverse_iterator Vector<T, Allocator>::rend() const noexcept
    { return const_reverse_iterator{start - 1}; }

    template <typename T, typename Allocator>
    typename Vector<T, Allocator>::const_reverse_iterator Vector<T, Allocator>::crbegin() const noexcept
    { return const_reverse_iterator{finish - 1}; }

    template <typename T, typename Allocator>
    typename Vector<T, Allocator>::const_reverse_iterator Vector<T, Allocator>::crend() const noexcept
    { return const_reverse_iterator{start - 1}; }

    template <typename T, typename Allocator>
    typename Vector<T, Allocator>::size_type Vector<T, Allocator>::size() const noexcept
    { return static_cast<size_type>(finish - start); }

    template <typename T, typename Allocator>
    typename Vector<T, Allocator>::size_type Vector<T, Allocator>::max_size() const noexcept
    { return static_cast<size_type>(UINT_MAX / sizeof(value_type)); }

    template <typename T, typename Allocator>
    typename Vector<T, Allocator>::size_type Vector<T, Allocator>::capacity() const noexcept
    { return static_cast<size_type>(termination - start); }

    template <typename T, typename Allocator>
    bool Vector<T, Allocator>::empty() const noexcept
    { return start == finish; }

    template <typename T, typename Allocator>
    void Vector<T, Allocator>::push_back(const value_type &value)
    {
        if (finish >= termination) {
            reallocate();
        }

        alloc.construct(finish++, value);
    }

    template <typename T, typename Allocator>
    void Vector<T, Allocator>::push_back(value_type &&value)
    {
        if (finish >= termination) {
            reallocate();
        }

        alloc.construct(finish++, std::move(value));
    }

    template <typename T, typename Allocator>
    void Vector<T, Allocator>::pop_back()
    { alloc.destroy(--finish); }

    /*
    iterator insert(const_iterator pos, const value_type &value)
    {
        if (finish >= termination) {
            difference_type diff = pos - start;
            reallocate();
            pos = start + diff;
        }
        push_back(value);

        iterator tmp = end() - 1;
        while (tmp != pos) {
            *tmp == std::move(*(tmp - 1));
            --tmp;
        }
        *tmp = value;

        return tmp;
    }

    iterator insert(const_iterator pos, value_type &&value)
    {
        if (finish >= termination) {
            difference_type diff = pos - start;
            reallocate();
            pos = start + diff;
        }
        push_back(value);

        iterator tmp = end() - 1;
        while (tmp != pos) {
            *tmp == std::move(*(tmp - 1));
            --tmp;
        }
        *tmp = std::move(value);

        return tmp;
    }

    iterator insert(const_iterator pos, size_type count, const value_type &value)
    {
        difference_type diff = pos - start;

        while (size() + count > capacity) {
            reallocate();
        }

        pos = cbegin() + diff;
        iterator tmp = begin() + diff + count;
        while (tmp - count >= pos) {
            *tmp = *(tmp - count);
            --tmp;
        }
        while (count--) {
            *tmp-- = value;
        }
        finish += count;

        return  tmp + 1;
    }

    iterator insert(const_iterator pos, std::initializer_list<value_type> ilist)
    {
        difference_type diff = pos - start;

        while (size() + ilist.size() > capacity) {
            reallocate();
        }

        pos = cbegin() + diff;
        iterator tmp = begin() + diff + ilist.size();
        while (tmp >= pos) {
            *tmp = *(tmp - ilist.size());
            --tmp;
        }
        for (auto i = ilist.crbegin(); i != ilist.crend(); ++i) {
            *tmp-- = *i;
        }
        finish += ilist.size();

        return tmp + 1;
    }

    iterator erase(const_iterator pos)
    {
        const_iterator tmp = pos;

        while (tmp + 1 != end()) {
            *tmp = *(tmp + 1);
            ++tmp;
        }
        --finish;

        return pos;
    }

    iterator erase(const_iterator first, const_iterator last)
    {
        size_type count = last - first;

        if (count == 0) {
            return first;
        }

        while (first + count != end()) {
            *first = *(first + count);
            ++first;
        }
        finish -= count;

        return first;
    }

    void resize(size_type count)
    { resize(count, value_type{}); }

    void resize(size_type count, const value_type &value)
    {
    }
    */

    template <typename T, typename Allocator>
    void Vector<T, Allocator>::sharink_to_fit()
    {
        value_type *newData = alloc.allocate(size());
        termination = finish = uninitialized_copy(begin(), end(), newData);
        free();
        start = newData;
    }

    template <typename T, typename Allocator>
    void Vector<T, Allocator>::swap(Vector &other)
    {
        std::swap(start, other.start);
        std::swap(finish, other.finish);
        std::swap(termination, other.termination);
    }

    template <typename T, typename Allocator>
    void Vector<T, Allocator>::alloc_copy(const Vector &other)
    {
        start = alloc.allocate(other.capacity());
        termination = start + other.capacity();
        finish = std::uninitialized_copy(other.begin(), other.end(), start);
    }

    template <typename T, typename Allocator>
    void Vector<T, Allocator>::reallocate()
    {
        value_type *newData = alloc.allocate(2 * capacity());
        value_type *tmp = std::uninitialized_copy(begin(), end(), newData);
        termination = newData + 2 * capacity();
        free();
        start = newData;
        finish = tmp;
    }

    template <typename T, typename Allocator>
    void Vector<T, Allocator>::free()
    {
        while (finish != start) {
            alloc.destroy(--finish);
        }
        alloc.deallocate(start, capacity());
    }

    template<typename T, typename Allocator>
    bool operator == (const Vector<T, Allocator> &lhs, const Vector<T, Allocator> &rhs)
    {
        auto i = lhs.begin();
        auto j = rhs.begin();

        while (i != lhs.end() && j != rhs.end() && *i == *j) {
            ++i;
            ++j;
        }

        return i == lhs.end() && j == rhs.end();
    }

    template<typename T, typename Allocator>
    bool operator != (const Vector<T, Allocator> &lhs, const Vector<T, Allocator> &rhs)
    { return !(lhs == rhs); }

    template<typename T, typename Allocator>
    bool operator < (const Vector<T, Allocator> &lhs, const Vector<T, Allocator> &rhs)
    {
        auto i = lhs.begin();
        auto j = rhs.begin();

        while (i != lhs.end() && j != rhs.end() && *i == *j) {
            ++i;
            ++j;
        }

        return i != lhs.end() && j != rhs.end() ? *i > *j : i != lhs.end();
    }

    template<typename T, typename Allocator>
    bool operator <= (const Vector<T, Allocator> &lhs, const Vector<T, Allocator> &rhs)
    { return lhs < rhs || lhs == rhs; }

    template<typename T, typename Allocator>
    bool operator > (const Vector<T, Allocator> &lhs, const Vector<T, Allocator> &rhs)
    { return !(lhs <= rhs); }

    template<typename T, typename Allocator>
    bool operator >= (const Vector<T, Allocator> &lhs, const Vector<T, Allocator> &rhs)
    { return !(lhs < rhs); }

} //namespace sp

#endif //SP_VECTOR__H
