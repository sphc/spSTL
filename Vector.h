//Created by sphc on 2018/2/4
//Vector.h
//

#ifndef SP_VECTOR__H
#define SP_VECTOR__H

#include <cstddef> //size_t, ptrdiff_t
#include <climits> //UINT_MAX
#include <memory> //allocator, uninitialized_copy, uninitialized_fill_n
#include <initializer_list> //initializer_list
#include <iterator> //distance

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

            bool operator == (const_reverse_iterator other)
            { return data == other.data; }

            bool operator != (const_reverse_iterator other)
            { return !(*this == other); }

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
               const allocator_type &allocator = allocator_type{}); 
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
        template<typename InputIterator>
        void assign(InputIterator first, InputIterator last);
        void assign(std::initializer_list<value_type> ilist);

        //get allocator
        allocator_type get_allocator() const;

        //access element
        reference at(size_type index);
        const_reference at(size_type index) const;
        reference operator [] (size_type index);
        const_reference operator [] (size_type index) const;
        reference front();
        reference back();
        const_reference front() const;
        const_reference back() const;
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
        void reserve(size_type newCapacity);
        size_type capacity() const noexcept;
        void shrink_to_fit();

        //update
        void clear() noexcept;
        void push_back(const value_type &value);
        void push_back(value_type &&value);
        void pop_back();
        iterator insert(const_iterator pos, const value_type &value);
        iterator insert(const_iterator pos, value_type &&value);
        iterator insert(const_iterator pos, size_type count, const value_type &value);
        template<typename InputIterator>
        iterator insert(const_iterator pos, InputIterator first, InputIterator last);
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
        allocator_type alloc;

        template <typename InputIterator>
        void alloc_copy(InputIterator first, InputIterator last);
        void alloc_copy(size_type count, const value_type &value);
        void reallocate(size_type theCapacity);
        void free();
    };

    template <typename T, typename Allocator>
    Vector<T, Allocator>::Vector(const allocator_type &allocator) 
        : start{nullptr}, finish{nullptr}, termination{nullptr}, alloc{allocator}
    { }

    template <typename T, typename Allocator>
    Vector<T, Allocator>::Vector(size_type count) : Vector(count, value_type{})
    { }

    template <typename T, typename Allocator>
    Vector<T, Allocator>::Vector(size_type count, const value_type &value, const allocator_type &allocator)
        : alloc{allocator} 
    {  alloc_copy(count, value); }

    //重载冲突
    template <typename T, typename Allocator>
    template<typename InputIterator>
    Vector<T, Allocator>::Vector(InputIterator first, InputIterator last, const allocator_type &allocator) 
        : alloc{allocator}
    { alloc_copy(first, last); }

    template <typename T, typename Allocator>
    Vector<T, Allocator>::Vector(const Vector &other) 
        : alloc{std::allocator_traits<allocator_type>::
            select_on_container_copy_construction(other.get_allocator())} 
    { alloc_copy(other.begin(), other.end()); }

    template <typename T, typename Allocator>
    Vector<T, Allocator>::Vector(const Vector &other, const allocator_type &allocator) 
        : alloc{allocator}
    { alloc_copy(other.begin(), other.end()); }

    template <typename T, typename Allocator>
    Vector<T, Allocator>::Vector(Vector &&other) 
        : start{other.start}, finish{other.finish}, 
          termination{other.termination}, alloc{std::move(other.alloc)}
    { other.start = other.finish = other.termination = nullptr; }

    /*
    template <typename T, typename Allocator>
    Vector<T, Allocator>::Vector(Vector &&other, const allocator_type &allocator) 
        : alloc{allocator}
    {}
    */

    template <typename T, typename Allocator>
    Vector<T, Allocator>::Vector(std::initializer_list<value_type> ilist, const allocator_type &allocator) 
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
            alloc_copy(other.begin(), other.end());
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
        alloc_copy(count, value);
    }

    //重载冲突
    template <typename T, typename Allocator>
    template <typename InputIterator>
    void Vector<T, Allocator>::assign(InputIterator first, InputIterator last)
    {
        free();
        alloc_copy(first, last);
    }

    template <typename T, typename Allocator>
    void Vector<T, Allocator>::assign(std::initializer_list<value_type> ilist)
    { assign(ilist.begin(), ilist.end()); }

    template <typename T, typename Allocator>
    typename Vector<T, Allocator>::allocator_type Vector<T, Allocator>::get_allocator() const
    { return alloc; }

    /*
    reference at(size_type index);
    const_reference at(size_type index) const;
    */

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
    void Vector<T, Allocator>::reserve(size_type newCapacity)
    { 
        if (newCapacity > capacity()) {
            reallocate(newCapacity);
        }
    }

    template <typename T, typename Allocator>
    typename Vector<T, Allocator>::size_type Vector<T, Allocator>::capacity() const noexcept
    { return static_cast<size_type>(termination - start); }

    template <typename T, typename Allocator>
    bool Vector<T, Allocator>::empty() const noexcept
    { return start == finish; }

    template <typename T, typename Allocator>
    void Vector<T, Allocator>::clear() noexcept
    {
        while (end() != begin()) {
            pop_back();
        }
    }

    template <typename T, typename Allocator>
    void Vector<T, Allocator>::push_back(const value_type &value)
    {
        if (finish >= termination) {
            reallocate(capacity() ? 2 * capacity() : 1);
        }

        alloc.construct(finish++, value);
    }

    template <typename T, typename Allocator>
    void Vector<T, Allocator>::push_back(value_type &&value)
    {
        if (finish >= termination) {
            reallocate(capacity() ? 2 * capacity() : 1);
        }

        alloc.construct(finish++, std::move(value));
    }

    template <typename T, typename Allocator>
    void Vector<T, Allocator>::pop_back()
    { alloc.destroy(--finish); }

    template <typename T, typename Allocator>
    typename Vector<T, Allocator>::iterator Vector<T, Allocator>::insert(const_iterator pos, const value_type &value)
    {
        difference_type diff = pos - start;
        push_back(value_type{});
        pos = start + diff;
        iterator it = end() - 1; 
        while (it != pos) {
            *it = *(it - 1);
            --it;
        }
        *it = value;

        return it;
    }

    template <typename T, typename Allocator>
    typename Vector<T, Allocator>::iterator Vector<T, Allocator>::insert(const_iterator pos, value_type &&value)
    {
        difference_type diff = std::distance(cbegin(), pos);
        push_back(value_type{});
        pos = start + diff;
        iterator it = end() - 1; 
        while (it != pos) {
            *it = *(it - 1);
            --it;
        }
        *it = std::move(value);

        return it;
    }

    template <typename T, typename Allocator>
    typename Vector<T, Allocator>::iterator Vector<T, Allocator>::insert(const_iterator pos, size_type count, const value_type &value)
    {
        difference_type diff = std::distance(cbegin(), pos);

        for (difference_type i = 0; i < count; ++i) {
            push_back(value_type{});
        }
        iterator it = end() - 1;
        while (it - count + 1!= begin() + diff) {
            *it = *(it - count);
            --it;
        }
        while (count--) {
            *it-- = value;
        }

        return it;
    }

    template <typename T, typename Allocator>
    template<typename InputIterator>
    typename Vector<T, Allocator>::iterator Vector<T, Allocator>::insert(const_iterator pos, InputIterator first, InputIterator last)
    {
        difference_type count = std::distance(first, last);
        difference_type diff = std::distance(cbegin(), pos);

        for (difference_type i = 0; i < count; ++i) {
            push_back(value_type{});
        }
        iterator it = end() - 1;
        while (it - count + 1 != begin() + diff) {
            *it = *(it - count);
            --it;
        }
        while (last != first) {
            *it-- = *--last;
        }

        return it;
    }

    template <typename T, typename Allocator>
    typename Vector<T, Allocator>::iterator Vector<T, Allocator>::insert(const_iterator pos, std::initializer_list<value_type> ilist)
    { return insert(pos, ilist.begin(), ilist.end()); }

    template <typename T, typename Allocator>
    typename Vector<T, Allocator>::iterator Vector<T, Allocator>::erase(const_iterator pos)
    {
        iterator it = const_cast<iterator>(pos), result = it;

        while (it + 1 != end()) {
            *it = *(it + 1);
            ++it;
        }
        pop_back();

        return result;
    }

    template <typename T, typename Allocator>
    typename Vector<T, Allocator>::iterator Vector<T, Allocator>::erase(const_iterator first, const_iterator last)
    {
        difference_type count = std::distance(first, last);
        iterator it = const_cast<iterator>(first), result = it;

        while (it + count != end()) {
            *it = *(it + count);
            ++it;
        }
        while (count--) {
            pop_back();
        }

        return result;
    }

    template <typename T, typename Allocator>
    void Vector<T, Allocator>::resize(size_type count)
    { resize(count, value_type{}); }

    template <typename T, typename Allocator>
    void Vector<T, Allocator>::resize(size_type count, const value_type &value)
    {
        if (count > capacity()) {
            reallocate(count);
        }
        if (count > size()) {
            finish = std::uninitialized_fill_n(finish, count - size(), value);
        }
        while (size() > count) {
            pop_back();
        }
    }

    template <typename T, typename Allocator>
    void Vector<T, Allocator>::shrink_to_fit()
    { reallocate(size()); }

    template <typename T, typename Allocator>
    void Vector<T, Allocator>::swap(Vector &other)
    {
        std::swap(start, other.start);
        std::swap(finish, other.finish);
        std::swap(termination, other.termination);
        std::swap(alloc, other.alloc);
    }

    template <typename T, typename Allocator>
    template <typename InputIterator>
    void Vector<T, Allocator>::alloc_copy(InputIterator first, InputIterator last)
    {
        finish = start = alloc.allocate(static_cast<size_type>(last - first));
        termination = start + (last - first);
        finish = std::uninitialized_copy(first, last, start);
    }
    
    template <typename T, typename Allocator>
    void Vector<T, Allocator>::alloc_copy(size_type count, const value_type &value)
    {
        start = alloc.allocate(count);
        termination = start + count;
        finish = std::uninitialized_fill_n(start, count, value);
    }

    template <typename T, typename Allocator>
    void Vector<T, Allocator>::reallocate(size_type theCapacity)
    {
        value_type *newData = alloc.allocate(theCapacity);
        value_type *tmp = std::uninitialized_copy(begin(), end(), newData);
        termination = newData + theCapacity;
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

        return i != lhs.end() && j != rhs.end() ? *i < *j : j != rhs.end();
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
