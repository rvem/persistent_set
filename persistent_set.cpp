#include "persistent_set.h"
#include <limits>
#include <assert.h>

persistent_set::persistent_set() : root(std::make_shared<node>(std::numeric_limits<value_type>::max())) {}


persistent_set::persistent_set(persistent_set const &other) : root(std::make_shared<node>(other.root)){}

persistent_set &persistent_set::operator=(persistent_set const &rhs) {
    root = std::make_shared<node>(rhs.root);
    return *this;
}

persistent_set::~persistent_set() {
    root.reset();
}

persistent_set::iterator::iterator(std::shared_ptr<node> curr, std::shared_ptr<node> root) : curr(curr), root(root) {}

persistent_set::iterator::iterator(iterator const &rhs) : curr(rhs.curr), root(rhs.root) {}

persistent_set::iterator persistent_set::find(value_type key) {
    std::shared_ptr<node> res = root->find(key);
    if (res == nullptr || res->key != key)
        return end();
    else
        return iterator(res, root);
}

std::pair<persistent_set::iterator, bool> persistent_set::insert(value_type key) {
    persistent_set::iterator iter = find(key);
    if (iter == end()) {
        return std::make_pair(iterator(root->insert(key), root), true);
    } else
        return std::make_pair(iter, false);
}

void persistent_set::erase(iterator iter) {
    assert(iter != end());
    root->erase(iter.curr, nullptr);
}

persistent_set::iterator &persistent_set::iterator::operator=(persistent_set::iterator const &rhs) {
    curr = rhs.curr;
    root = rhs.root;
    return *this;
};

persistent_set::value_type const &persistent_set::iterator::operator*() const {
    return curr->key;
};

persistent_set::iterator persistent_set::begin() const {
    std::shared_ptr<node> curr = root;
    while (curr->left) {
        curr = curr->left;
    }
    return iterator(curr, root);
}

persistent_set::iterator persistent_set::end() const {
    std::shared_ptr<node> curr = root;
    while (curr->right) {
        curr = curr->right;
    }
    return iterator(curr, root);
}

persistent_set::iterator &persistent_set::iterator::operator++() {
    if (curr->right) {
        curr = curr->right;
        while (curr->left) {
            curr = curr->left;
        }
    } else {
        curr = root->next(curr);
    }
    return *this;
}

persistent_set::iterator &persistent_set::iterator::operator--() {
    if (curr->left) {
        curr = curr->left;
        while (curr->right) {
            curr = curr->right;
        }
    } else {
        curr = root->prev(curr);
    }
    return *this;
}

persistent_set::iterator persistent_set::iterator::operator++(int) {
    iterator tmp = *this;
    ++*this;
    return tmp;
}

persistent_set::iterator persistent_set::iterator::operator--(int) {
    iterator tmp = *this;
    --*this;
    return tmp;
}

bool operator!=(persistent_set::iterator a, persistent_set::iterator b) {
    return a.curr != b.curr;
}


bool operator==(persistent_set::iterator a, persistent_set::iterator b) {
    return a.curr == b.curr;
}

persistent_set::node::node(value_type key) : key(key), left(nullptr), right(nullptr) {}

persistent_set::node::node(std::shared_ptr<node> const &other) : key(other->key), left(other->left),
                                                                 right(other->right) {}

persistent_set::node &persistent_set::node::operator=(const node &other) {
    key = other.key;
    left = other.left;
    right = other.right;
    return *this;
}


std::shared_ptr<persistent_set::node> persistent_set::node::next(std::shared_ptr<node> curr) {
    if (key < curr->key) {
        if (right)
            return right->next(curr);
        else
            return shared_from_this();
    } else {
        std::shared_ptr<node> tmp;
        if (left)
            tmp = left->next(curr);
        else
            tmp = shared_from_this();
        if (tmp->key > curr->key)
            return tmp;
        else
            return shared_from_this();
    }
}

std::shared_ptr<persistent_set::node> persistent_set::node::prev(std::shared_ptr<node> curr) {
    if (key > curr->key) {
        if (left)
            return left->prev(curr);
        else
            return shared_from_this();
    } else {
        std::shared_ptr<node> tmp;
        if (right)
            tmp = right->prev(curr);
        else
            tmp = shared_from_this();
        if (tmp->key < curr->key)
            return tmp;
        else
            return shared_from_this();
    }
}

std::shared_ptr<persistent_set::node> persistent_set::node::find(value_type x) {
    if (x == key) {
        return shared_from_this();
    }
    if (x < key) {
        if (left)
            return left->find(x);
        else
            return nullptr;
    } else {
        if (right)
            return right->find(x);
        else
            return nullptr;
    }
}

std::shared_ptr<persistent_set::node> persistent_set::node::insert(value_type x) {
    if (x == key) {
        return shared_from_this();
    }
    if (x < key) {
        if (left)
            return (left = std::make_shared<node>(left))->insert(x);
        else {
            return (left = std::make_shared<node>(x));
        }
    } else {
        if (right)
            return (right = std::make_shared<node>(right))->insert(x);
        else
            return (right = std::make_shared<node>(x));
    }
}

void persistent_set::node::erase(std::shared_ptr<node> curr, std::shared_ptr<node> p) {
    if (key == curr->key) {
        std::shared_ptr<node> tmp;
        if (left) {
            left = std::make_shared<node>(left);
            tmp = left;
            while (tmp->right) {
                tmp->right = std::make_shared<node>(tmp->right);
                tmp = tmp->right;
            }
            tmp->right = right;
            right = nullptr;
        }
        if (p->left == shared_from_this())
            if (left)
                p->left = left;
            else
                p->left = right;
        else if (left)
            p->right = left;
        else
            p->right = right;
    } else if (curr->key < key) {
        left = std::make_shared<node>(left);
        left->erase(curr, shared_from_this());
    } else {
        right = std::make_shared<node>(right);
        right->erase(curr, shared_from_this());
    }
}