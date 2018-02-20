//Created by sphc on 2018/2/6
//List.h
//

#ifndef LIST__H
#define LIST__H

#include <cstddef> //size_t ptrdiff_t
#include <memory> //allocator
#include <initializer_list> //initializer_list
#include <climits> //UINT_MAX

namespace sp {

    template <typename T, typename Allocator = std::allocator<T>>
    class List {
    public:
        typedef T value_type;
        typedef Allocator allocator_type;
        typedef std::size_t size_type;
        typedef std::ptrdiff_t difference_type;
        typedef value_type &reference;
        typedef const value_type &const_reference;
        typedef std::allocator_traits<Allocator>::pointer pointer;
        typedef std::allocator_traits<Allocator>::const_pointer const_pointer;

        class const_iterator {
        public:
            const_iterator(List *theList, Node *content) 
                : this->theList{theList}, this->content{content} { }

            const_iterator operator ++ ()
            { 
                content = content->next;
                return *this; 
            }

            const_iterator operator ++ (int)
            { 
                const_iterator old = *this;
                content = content->next;
                return old; 
            }

            const_iterator operator -- ()
            {
                content = content->prior;
                return *this; 
            }

            const_iterator operator -- (int)
            { 
                const_iterator old = *this;
                content = content->prior;
                return old; 
            }

            const_reference operator * () const
            { return content->data; }

            const_pointer operator -> () const
            { return &content->data; }

        private:
            Node *content;
            List *theList;
        };

        class iterator : public const_iterator {
        public:
            iterator(List *theList, Node *content) 
                : const_iterator{theList, content} { }

            iterator operator ++ ()
            { 
                content = content->next;
                return *this; 
            }

            iterator operator ++ (int)
            { 
                const_iterator old = *this;
                content = content->next;
                return old; 
            }

            iterator operator -- ()
            { 
                content = content->prior;
                return *this; 
            }

            iterator operator -- (int)
            { 
                iterator old = *this;
                content = content->prior;
                return old; 
            }

            reference operator * () const
            { return content->data; }

            pointer operator -> () const
            { return &content->data; }
        };

        class const_reverse_iterator {
        public:
            const_reverse_iterator(List *theList, Node *content) 
                : this->theList{theList}, this->content{content} { }

            const_reverse_iterator operator ++ ()
            { 
                content = content->prior;
                return *this; 
            }

            const_reverse_iterator operator ++ (int)
            { 
                const_reverse_iterator old = *this;
                content = content->prior;
                return old; 
            }

            const_reverse_iterator operator -- ()
            {
                content = content->next;
                return *this; 
            }

            const_reverse_iterator operator -- (int)
            { 
                const_reverse_iterator old = *this;
                content = content->next;
                return old; 
            }

            const_reference operator * () const
            { return content->data; }

            const_pointer operator -> () const
            { return &content->data; }

        private:
            Node *content;
            List *theList;
        };

        class reverse_iterator : public const_reverse_iterator {
        public:
            reverse_iterator(List *theList, Node *content) 
                : const_reverse_iterator{theList, content} { }

            reverse_iterator operator ++ ()
            { 
                content = content->prior;
                return *this; 
            }

            reverse_iterator operator ++ (int)
            { 
                reverse_iterator old = *this;
                content = content->prior;
                return old; 
            }

            reverse_iterator operator -- ()
            {
                content = content->next;
                return *this; 
            }

            reverse_iterator operator -- (int)
            { 
                reverse_iterator old = *this;
                content = content->next;
                return old; 
            }

            reference operator * () const
            { return content->data; }

            pointer operator -> () const
            { return &content->data; }

        private:
            Node *content;
            List *theList;
        };

        //constructor
        explicit List(const Allocator &alloc = Allocator());
        List(size_type count, const value_type &value, const allocator_type &alloc = allocator_type{});
        explicit List(size_type count);
        template <typename InputIterator>
        List(InputIterator first, InputIterator last, const allocator_type &alloc = allocator_type{});
        List(const List &other);
        List(const List &other, const allocator_type alloc);
        List(List &&other);
        List(List &&other, const allocator_type alloc);
        List(std::initializer_list<value_type> init, const allocator_type &alloc = allocator_type{});
        ~List();

        //assign
        List &operator = (const List &other);
        List &operator = (List &&other);
        List &operator = (std::initializer_list<value_type> ilist);
        void assign(size_type count, const value_type &value);
        template <typename InputIterator>
        void assign(InputIterator first, InputIterator last);
        void assign(std::initializer_list<value_type> ilist);

        //getallocator
        allocator_type get_allocator() const;

        //access
        reference front();
        const_reference front() const;
        reference back();
        const_reference back() const;

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

        //update
        void clear() noexcept;
        iterator insert(const_iterator pos, const value_type &value);
        iterator insert(const_iterator pos, value_type &&value);
        iterator insert(const_iterator pos, size_type count, const value_type &value);
        template <typename InputIterator>
        iterator insert(const_iterator pos, InputIterator first, InputIterator last);
        iterator insert(const_iterator pos, std::initializer_list<value_type> ilist);
        /*
        template <typename... Args>
        iterator emplace(const_iterator pos, Args &&... args);
        */
        iterator erase(const_iterator pos);
        iterator erase(const_iterator first, const_iterator last);
        void push_front(const value_type &value);
        void push_front(value_type &&value);
        /*
        template <typename... Args>
        void emplace_front(Args &&... args);
        */
        void pop_front();
        void push_back(const value_type &value);
        void push_back(value_type &&value);
        /*
        template <<typename... Args>
        void emplace_back(Args &&... args);
        */
        void pop_back();
        void resize(size_type count);
        void resize(size_type count, const value_type &value);
        void swap(List &other);

        //operation
        void merge(List &&other);
        template <typename Compare>
        void merge(List &&other, Compare comp);
        void splice(const_iterator pos, List &&other);
        void splice(const_iterator pos, List &&other, const_iterator it);
        void splice(const_iterator pos, List &&other, const_iterator first, const_iterator last);
        void remove(const value_type &value);
        template <typename UnaryPredicate>
        void remove_if(UnaryPredicate p);
        void reverse() noexcept;
        void unique();
        template <typename BinaryPredicate>
        void unique(BinaryPredicate p);
        void sort();
        template <typename Compare>
        void sort(Compare comp);

    private:
        class Node {
            value_type data;
            value_type *prior;
            value_type *next;

            Node(const value_type &data, value_type *prior, value_type *next) :
                this->data{data}, this->prior{prior}, this->next{next} { }

            Node(value_type &&data, value_type *prior, value_type *next) :
                this->data{std::move(data)}, this->prior{prior}, this->next{next} { }
        };
        
        Node *head;
        Node *tail;
        size_type theSize;
        allocator_type allocator;

        void free();
    };

    //constructor
    template <typename T, typename Allocator>
    explicit List<T, Allocator>::List(const Allocator &alloc = Allocator()) 
        : allocator{alloc}, head{new Node}, tail{new Node}, theSize{}
    { 
        head->next = tail;
        tail->prior = head;
    }

    template <typename T, typename Allocator>
    List<T, Allocator>::List(size_type count, const value_type &value, const allocator_type &alloc = allocator_type{})
        : List(alloc)//allocator{alloc}
    {
        while (count--) {
            push_back(value);
        }
    }

    template <typename T, typename Allocator>
    explicit List<T, Allocator>::List(size_type count) : List(count, value_type{})
    { }

    template <typename T, typename Allocator>
    template <typename InputIterator>
    List<T, Allocator>::List(InputIterator first, InputIterator last, const allocator_type &alloc = allocator_type{}) 
        : List(alloc)
    { insert(begin(), first, last); }

    template <typename T, typename Allocator>
    List<T, Allocator>::List(const List &other) 
        : List(other.begin(), other.end(), other.get_allocator())
    { }

    /*
    template <typename T, typename Allocator>
    List(const List &other, const allocator_type alloc);
    */

    template <typename T, typename Allocator>
    List<T, Allocator>::List(List &&other) 
        : List(other.get_allocator)
    {
        const_iterator first = other.cbegin();
        const_iterator last = other.cend();

        while (first != last) {
            push_back(std::move(*first++));
        }
    }

    /*
    template <typename T, typename Allocator>
    List(List &&other, const allocator_type alloc);
    */

    template <typename T, typename Allocator>
    List<T, Allocator>::List(std::initializer_list<value_type> init, const allocator_type &alloc = allocator_type{})
        : List(init.begin(), init.end(), alloc)
    { }

    template <typename T, typename Allocator>
    ~List<T, Allocator>::List()
    { free(); }

    //assign
    template <typename T, typename Allocator>
    List<T, Allocator>::List &List<T, Allocator>::operator = (const List &other)
    {
        if (this != &other) {
            assign(other.begin(), other.end());
        }
    }

    template <typename T, typename Allocator>
    List<T, Allocator>::List &List<T, Allocator>::operator = (List &&other)
    {
        if (this != &other) {
            clear();
            const_iterator first = other.cbegin();
            const_iterator last = other.cend();

            while (first != last) {
                push_back(std::move(*first++));
            }
        }
    }

    template <typename T, typename Allocator>
    List<T, Allocator>::List &List<T, Allocator>::operator = (std::initializer_list<value_type> ilist)
    { assign(ilist); }

    template <typename T, typename Allocator>
    void List<T, Allocator>::assign(size_type count, const value_type &value)
    {
        clear();
        insert(begin(), count, value);
    }

    template <typename T, typename Allocator>
    template <typename InputIterator>
    void List<T, Allocator>::assign(InputIterator first, InputIterator last)
    {
        claer();
        insert(begin(), first, last);
    }

    template <typename T, typename Allocator>
    void List<T, Allocator>::assign(std::initializer_list<value_type> ilist)
    { assign(ilist.begin(), ilist.end()); }

    //getallocator
    template <typename T, typename Allocator>
    typename List<T, Allocator>::allocator_type List<T, Allocator>::get_allocator() const
    { return allocator; }

    //access
    template <typename T, typename Allocator>
    typename List<T, Allocator>::reference List<T, Allocator>::front()
    { return *begin(); }

    template <typename T, typename Allocator>
    typename List<T, Allocator>::const_reference List<T, Allocator>::front() const
    { return *begin(); }

    template <typename T, typename Allocator>
    typename List<T, Allocator>::reference List<T, Allocator>::back()
    { return *rbegin(); }

    template <typename T, typename Allocator>
    typename List<T, Allocator>::const_reference List<T, Allocator>::back() const
    { return *rbegin(); }

    //iterator
    template <typename T, typename Allocator>
    typename List<T, Allocator>::iterator List<T, Allocator>::begin() noexcept
    { return iterator{this, head->next}; }

    template <typename T, typename Allocator>
    typename List<T, Allocator>::iterator List<T, Allocator>::end() noexcept
    { return iterator{this, tail}; }

    template <typename T, typename Allocator>
    typename List<T, Allocator>::const_iterator List<T, Allocator>::begin() const noexcept
    { return const_iterator{this, head->next}; }

    template <typename T, typename Allocator>
    typename List<T, Allocator>::const_iterator List<T, Allocator>::end() const noexcept
    { return const_iterator{this, tail};

    template <typename T, typename Allocator>
    typename List<T, Allocator>::const_iterator List<T, Allocator>::cbegin() const noexcept
    { return const_iterator{this, head->next}; }
    
    template <typename T, typename Allocator>
    typename List<T, Allocator>::const_iterator List<T, Allocator>::cend() const noexcept
    { return const_iterator{this, tail}; }

    template <typename T, typename Allocator>
    typename List<T, Allocator>::reverse_iterator List<T, Allocator>::rbegin() noexcept
    { return reverse_iterator{this, tail->prior}; }

    template <typename T, typename Allocator>
    typename List<T, Allocator>::reverse_iterator List<T, Allocator>::rend() noexcept
    { return reverse_iterator{this, head}; }

    template <typename T, typename Allocator>
    typename List<T, Allocator>::const_reverse_iterator List<T, Allocator>::rbegin() const noexcept
    { return const_reverse_iterator{this, tail->prior}; }

    template <typename T, typename Allocator>
    typename List<T, Allocator>::const_reverse_iterator List<T, Allocator>::rend() const noexcept
    { return const_reverse_iterator{this, head}; }

    template <typename T, typename Allocator>
    typename List<T, Allocator>::const_reverse_iterator List<T, Allocator>::crbegin() const noexcept
    { return const_reverse_iterator{this, tail->prior}; }

    template <typename T, typename Allocator>
    typename List<T, Allocator>::const_reverse_iterator List<T, Allocator>::crend() const noexcept
    { return const_reverse_iterator{this, head}; }

    //capacity
    template <typename T, typename Allocator>
    bool List<T, Allocator>::empty() const noexcept
    { return theSize == 0; }

    template <typename T, typename Allocator>
    typename List<T, Allocator>::size_type List<T, Allocator>::size() const noexcept
    { return theSize; }

    template <typename T, typename Allocator>
    typename List<T, Allocator>::size_type List<T, Allocator>::max_size() const noexcept
    { return UINT_MAX / sizeof(value_type); }

    //update
    template <typename T, typename Allocator>
    void List<T, Allocator>::clear() noexcept
    {
        while (!empty()) {
            pop_back();
        }
    }

    template <typename T, typename Allocator>
    typename List<T, Allocator>::iterator List<T, Allocator>::insert(const_iterator pos, const value_type &value)
    {
        Node *p = pos.content;
        return iterator{this, p->prior = p->prior->next = new Node{value, p->prior, p}};
    }

    template <typename T, typename Allocator>
    typename List<T, Allocator>::iterator List<T, Allocator>::insert(const_iterator pos, value_type &&value)
    {
        Node *p = pos.content;
        return iterator{this, p->prior = p->prior->next = new Node{std::move(value), p->prior, p}};
    }

    template <typename T, typename Allocator>
    typename List<T, Allocator>::iterator List<T, Allocator>::insert(const_iterator pos, size_type count, const value_type &value)
    {
        while (count--) {
            pos = insert(pos, value);
        }
        return iterator{this, pos.content};
    }

    template <typename T, typename Allocator>
    template <typename InputIterator>
    typename List<T, Allocator>::iterator List<T, Allocator>::insert(const_iterator pos, InputIterator first, InputIterator last)
    {
        while (first != last) {
            pos = insert(pos, *--last);
        }
        return iterator{this, pos.content};
    }

    template <typename T, typename Allocator>
    typename List<T, Allocator>::iterator List<T, Allocator>::insert(const_iterator pos, std::initializer_list<value_type> ilist)
    { return insert(pos, ilist.begin(), ilist.end()); }

    /*
    template <typename... Args>
    iterator emplace(const_iterator pos, Args &&... args);
    */
    template <typename T, typename Allocator>
    typename List<T, Allocator>::iterator List<T, Allocator>::erase(const_iterator pos)
    {
        Node *p = pos.content;
        iterator it{this, p->next};

        p->prior->next = p->next;
        p->next->prior = p-prior;
        delete p;

        return it;
    }

    template <typename T, typename Allocator>
    typename List<T, Allocator>::iterator List<T, Allocator>::erase(const_iterator first, const_iterator last)
    {
        while (first != last) {
            first = erase(first);
        }
        return iterator{this, last.content};
    }

    template <typename T, typename Allocator>
    void List<T, Allocator>::push_front(const value_type &value)
    { insert(begin(), value); }

    template <typename T, typename Allocator>
    void List<T, Allocator>::push_front(value_type &&value)
    { insert(begin(), std::move(value); }

    /*
    template <typename... Args>
    void emplace_front(Args &&... args);
    */
    template <typename T, typename Allocator>
    void List<T, Allocator>::pop_front()
    { erase(begin()); }

    template <typename T, typename Allocator>
    void List<T, Allocator>::push_back(const value_type &value)
    { insert(end(), value); }

    template <typename T, typename Allocator>
    void List<T, Allocator>::push_back(value_type &&value)
    { insert(end(), std::move(value); }

    /*
    template <<typename... Args>
    void emplace_back(Args &&... args);
    */
    template <typename T, typename Allocator>
    void List<T, Allocator>::pop_back()
    { erase(--end()); }

    template <typename T, typename Allocator>
    void List<T, Allocator>::resize(size_type count)
    { resie(count, value_type{}); }

    template <typename T, typename Allocator>
    void List<T, Allocator>::resize(size_type count, const value_type &value)
    {
        while (size() > count) {
            pop_back(); 
        }
        while (size() < count) {
            push_back(value);
        }
    }

    template <typename T, typename Allocator>
    void List<T, Allocator>::swap(List &other)
    {
        std::swap(head, other.head);
        std::swap(tail, other.tail);
        std::swap(theSize, other.theSize);
        std::swap(allocator, other.allocator);
    }

    //operation
    template <typename T, typename Allocator>
    void List<T, Allocator>::merge(List &&other);

    template <typename T, typename Allocator>
    template <typename Compare>
    void List<T, Allocator>::merge(List &&other, Compare comp);

    template <typename T, typename Allocator>
    void List<T, Allocator>::splice(const_iterator pos, List &&other);

    template <typename T, typename Allocator>
    void List<T, Allocator>::splice(const_iterator pos, List &&other, const_iterator it);

    template <typename T, typename Allocator>
    void List<T, Allocator>::splice(const_iterator pos, List &&other, const_iterator first, const_iterator last);

    template <typename T, typename Allocator>
    void List<T, Allocator>::remove(const value_type &value);

    template <typename T, typename Allocator>
    template <typename UnaryPredicate>
    void List<T, Allocator>::remove_if(UnaryPredicate p);

    template <typename T, typename Allocator>
    void List<T, Allocator>::reverse() noexcept;

    template <typename T, typename Allocator>
    void List<T, Allocator>::unique();
    
    template <typename T, typename Allocator>
    template <typename BinaryPredicate>
    void List<T, Allocator>::unique(BinaryPredicate p);

    template <typename T, typename Allocator>
    void List<T, Allocator>::sort();

    template <typename T, typename Allocator>
    template <typename Compare>
    void List<T, Allocator>::sort(Compare comp);

    template <typename T, typename Allocator>
    void List<T, Allocator>::free()
    {
        clear();
        delete head;
        delete tail;
    }

} //namespace sp

#endif //LIST__H
