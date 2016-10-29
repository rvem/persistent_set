#ifndef PERSISTENT_SET_H
#define PERSISTENT_SET_H

#include <iostream>
#include <vector>
#include <memory>

struct persistent_set
{

    // Вы можете определить этот тайпдеф по вашему усмотрению.
    typedef int value_type;

private:
    struct node : std::enable_shared_from_this<node>
    {
        value_type key;
        std::shared_ptr<node> left, right;

        node(value_type key);

        node(std::shared_ptr<node> const &);

        node& operator=(const node&);

        std::shared_ptr<node> next(std::shared_ptr<node> curr);

        std::shared_ptr<node> prev(std::shared_ptr<node> curr);

        std::shared_ptr<node> find(value_type key);

        std::shared_ptr<node> insert(value_type key);

        void erase(std::shared_ptr<node> curr, std::shared_ptr<node> p);
    };
    std::shared_ptr<node> root;

    // Bidirectional iterator.
public:
    struct iterator
    {
        friend struct persistent_set;
        iterator(std::shared_ptr<node> curr, std::shared_ptr<node> root);
        iterator(iterator const& rhs);
        iterator& operator=(iterator const& rhs);

        value_type const& operator*() const;

        iterator& operator++();
        iterator operator++(int);

        iterator& operator--();
        iterator operator--(int);

        friend bool operator==(iterator, iterator);
        friend bool operator!=(iterator, iterator);

    private:
        std::shared_ptr<node> curr;
        std::shared_ptr<node> root;
    };

    persistent_set();

    persistent_set(persistent_set const&);

    persistent_set& operator=(persistent_set const& rhs);

    ~persistent_set();

    iterator find(value_type key);

    std::pair<iterator, bool> insert(value_type key);

    void erase(iterator);

    iterator begin() const;
    iterator end() const;
};

bool operator==(persistent_set::iterator, persistent_set::iterator);
bool operator!=(persistent_set::iterator, persistent_set::iterator);


#endif //PERSISTENT_SET_H