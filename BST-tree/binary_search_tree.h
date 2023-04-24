#ifndef SANDBOX_CPP_BINARY_SEARCH_TREE_H
#define SANDBOX_CPP_BINARY_SEARCH_TREE_H

#include <stack>
#include "../../../Downloads/associative_container.h"
#include "../exception/not_implemented.h"
#include "../logger/logger.h"
#include "../logger/logger_holder.h"
#include "../memory/memory.h"
#include "traversable.h"
#include <vector>

template<
    typename tkey,
    typename tvalue,
    typename tkey_comparer>
class binary_search_tree:
    public associative_container<tkey, tvalue>,
    protected logger_holder
{

protected:

    struct node
    {

        tkey key;

        tvalue value;

        node *left_subtree_address;

        node *right_subtree_address;

    };

public:

    class prefix_iterator final
    {

    private:

        node *_tree_root;
        std::stack<node *> _way;

    public:

        explicit prefix_iterator(
            node *tree_root);

    public:

        bool operator==(
            prefix_iterator const &other) const;

        prefix_iterator& operator++();

        prefix_iterator operator++(
            int not_used);

        std::tuple<unsigned int, tkey const&, tvalue const&> operator*() const;

    };

    class infix_iterator final
    {

    private:

        node *_tree_root;
        std::stack<node *> _way;

    public:

        explicit infix_iterator(
            node *tree_root);

    public:

        bool operator==(
            infix_iterator const &other) const;

        infix_iterator& operator++();

        infix_iterator operator++(
            int not_used);

        std::tuple<unsigned int, tkey const&, tvalue const&> operator*() const;

    };

    class postfix_iterator final
    {

    private:

        node *_tree_root;
        std::stack<node *> _way;

    public:

        explicit postfix_iterator(
            node *tree_root);

    public:

        bool operator==(
            postfix_iterator const &other) const;

        postfix_iterator &operator++();

        postfix_iterator operator++(
            int not_used);

        std::tuple<unsigned int, tkey const&, tvalue const&> operator*() const;

    };

protected:

    class insertion_template_method
    {

    public:

        virtual ~insertion_template_method() = default;

    public:

        void insert(
            tkey const &key,
            tvalue &&value,
            node *&tree_root_address);

    private:

        void insert_inner(
            tkey const &key,
            tvalue &&value,
            node *&subtree_root_address,
            std::stack<node **> &path_to_subtree_root_exclusive);

    protected:

        virtual void before_insert_inner(
            tkey const &key,
            tvalue &&value,
            node *&subtree_root_address,
            std::stack<node **> &path_to_subtree_root_exclusive);

        virtual void after_insert_inner(
            tkey const &key,
            tvalue &&value,
            node *&subtree_root_address,
            std::stack<node **> &path_to_subtree_root_exclusive);

    };

    class reading_template_method
    {

    public:

        virtual ~reading_template_method() = default;

    public:

        tvalue const &read(
            tkey const &key,
            node *&tree_root_address);

    private:

        tvalue const &read_inner(
            tkey const &key,
            node *&subtree_root_address,
            std::stack<node **> &path_to_subtree_root_exclusive);

    protected:

        virtual void before_read_inner(
            tkey const &key,
            node *&subtree_root_address,
            std::stack<node **> &path_to_subtree_root_exclusive);

        virtual void after_read_inner(
            tkey const &key,
            node *&subtree_root_address,
            std::stack<node **> &path_to_subtree_root_exclusive);

    };

    class removing_template_method
    {

    public:

        virtual ~removing_template_method() = default;

    public:

        tvalue &&remove(
            tkey const &key,
            node *&tree_root_address);

    private:

        tvalue &&remove_inner(
            tkey const &key,
            node *&subtree_root_address,
            std::stack<node **> &path_to_subtree_root_exclusive);

    protected:

        virtual void before_remove_inner(
            tkey const &key,
            node *&subtree_root_address,
            std::stack<node **> &path_to_subtree_root_exclusive);

        virtual void after_remove_inner(
            tkey const &key,
            node *&subtree_root_address,
            std::stack<node **> &path_to_subtree_root_exclusive);

    };

private:

    node *_root;
    memory *_allocator;
    logger *_logger;
    insertion_template_method *_insertion;
    reading_template_method *_reading;
    removing_template_method *_removing;

protected:

    explicit binary_search_tree(
        insertion_template_method *insertion,
        reading_template_method *reading,
        removing_template_method *removing,
        memory *allocator = nullptr,
        logger *logger = nullptr);

public:

    explicit binary_search_tree(
        memory *allocator = nullptr,
        logger *logger = nullptr);

    binary_search_tree(
        binary_search_tree const &other);

    binary_search_tree(
        binary_search_tree &&other) noexcept;

    binary_search_tree &operator=(
        binary_search_tree const &other);

    binary_search_tree &operator=(
        binary_search_tree &&other) noexcept;

    ~binary_search_tree();

public:

    void insert(
        tkey const &key,
        tvalue &&value) final;

    tvalue const &get(
        tkey const &key) final;

    tvalue &&remove(
        tkey const &key) final;

private:

    logger *get_logger() const override;

public:

    prefix_iterator begin_prefix() const noexcept;

    prefix_iterator end_prefix() const noexcept;

    infix_iterator begin_infix() const noexcept;

    infix_iterator end_infix() const noexcept;

    postfix_iterator begin_postfix() const noexcept;

    postfix_iterator end_postfix() const noexcept;

};

// region iterators implementation

//region prefix_iterator implementation

template<
    typename tkey,
    typename tvalue,
    typename tkey_comparer>
binary_search_tree<tkey, tvalue, tkey_comparer>::prefix_iterator::prefix_iterator(
    binary_search_tree<tkey, tvalue, tkey_comparer>::node *tree_root)
{

}

template<
    typename tkey,
    typename tvalue,
    typename tkey_comparer>
bool binary_search_tree<tkey, tvalue, tkey_comparer>::prefix_iterator::operator==(
    binary_search_tree<tkey, tvalue, tkey_comparer>::prefix_iterator const &other) const
{

}

template<
    typename tkey,
    typename tvalue,
    typename tkey_comparer>
typename binary_search_tree<tkey, tvalue, tkey_comparer>::prefix_iterator &binary_search_tree<tkey, tvalue, tkey_comparer>::prefix_iterator::operator++()
{

}

template<
    typename tkey,
    typename tvalue,
    typename tkey_comparer>
typename binary_search_tree<tkey, tvalue, tkey_comparer>::prefix_iterator binary_search_tree<tkey, tvalue, tkey_comparer>::prefix_iterator::operator++(
    int not_used)
{

}

template<
    typename tkey,
    typename tvalue,
    typename tkey_comparer>
std::tuple<unsigned int, tkey const&, tvalue const&> binary_search_tree<tkey, tvalue, tkey_comparer>::prefix_iterator::operator*() const
{

}

//endregion prefix_iterator implementation

//region infix_iterator implementation

template<
    typename tkey,
    typename tvalue,
    typename tkey_comparer>
binary_search_tree<tkey, tvalue, tkey_comparer>::infix_iterator::infix_iterator(
    binary_search_tree<tkey, tvalue, tkey_comparer>::node *tree_root)
{

}

template<
    typename tkey,
    typename tvalue,
    typename tkey_comparer>
bool binary_search_tree<tkey, tvalue, tkey_comparer>::infix_iterator::operator==(
    binary_search_tree<tkey, tvalue, tkey_comparer>::infix_iterator const &other) const
{

}

template<
    typename tkey,
    typename tvalue,
    typename tkey_comparer>
typename binary_search_tree<tkey, tvalue, tkey_comparer>::infix_iterator &binary_search_tree<tkey, tvalue, tkey_comparer>::infix_iterator::operator++()
{

}

template<
    typename tkey,
    typename tvalue,
    typename tkey_comparer>
typename binary_search_tree<tkey, tvalue, tkey_comparer>::infix_iterator binary_search_tree<tkey, tvalue, tkey_comparer>::infix_iterator::operator++(
    int not_used)
{

}

template<
    typename tkey,
    typename tvalue,
    typename tkey_comparer>
std::tuple<unsigned int, tkey const&, tvalue const&> binary_search_tree<tkey, tvalue, tkey_comparer>::infix_iterator::operator*() const
{

}

//endregion infix_iterator implementation

//region postfix_iterator implementation

template<
    typename tkey,
    typename tvalue,
    typename tkey_comparer>
binary_search_tree<tkey, tvalue, tkey_comparer>::postfix_iterator::postfix_iterator(
    binary_search_tree<tkey, tvalue, tkey_comparer>::node *tree_root)
{

}

template<
    typename tkey,
    typename tvalue,
    typename tkey_comparer>
bool binary_search_tree<tkey, tvalue, tkey_comparer>::postfix_iterator::operator==(
    binary_search_tree<tkey, tvalue, tkey_comparer>::postfix_iterator const &other) const
{

}

template<
    typename tkey,
    typename tvalue,
    typename tkey_comparer>
typename binary_search_tree<tkey, tvalue, tkey_comparer>::postfix_iterator &binary_search_tree<tkey, tvalue, tkey_comparer>::postfix_iterator::operator++()
{

}

template<
    typename tkey,
    typename tvalue,
    typename tkey_comparer>
typename binary_search_tree<tkey, tvalue, tkey_comparer>::postfix_iterator binary_search_tree<tkey, tvalue, tkey_comparer>::postfix_iterator::operator++(
    int not_used)
{

}

template<
    typename tkey,
    typename tvalue,
    typename tkey_comparer>
std::tuple<unsigned int, tkey const&, tvalue const&> binary_search_tree<tkey, tvalue, tkey_comparer>::postfix_iterator::operator*() const
{

}

//endregion prefix_iterator implementation

// endregion iterators implementation

// region template methods implementation

// region insertion implementation

template<
    typename tkey,
    typename tvalue,
    typename tkey_comparer>
void binary_search_tree<tkey, tvalue, tkey_comparer>::insertion_template_method::insert(
    tkey const &key,
    tvalue &&value,
    binary_search_tree<tkey, tvalue, tkey_comparer>::node *&tree_root_address)
{
    return insert_inner(key, value, tree_root_address, std::stack<binary_search_tree<tkey, tvalue, tkey_comparer>::node **>());
}

template<
    typename tkey,
    typename tvalue,
    typename tkey_comparer>
void binary_search_tree<tkey, tvalue, tkey_comparer>::insertion_template_method::insert_inner(
    tkey const &key,
    tvalue &&value,
    binary_search_tree<tkey, tvalue, tkey_comparer>::node *&subtree_root_address,
    std::stack<binary_search_tree<tkey, tvalue, tkey_comparer>::node **> &path_to_subtree_root_exclusive)
{
    before_insert_inner(key, value, subtree_root_address, path_to_subtree_root_exclusive);



    after_insert_inner(key, value, subtree_root_address, path_to_subtree_root_exclusive);
}

template<
    typename tkey,
    typename tvalue,
    typename tkey_comparer>
void binary_search_tree<tkey, tvalue, tkey_comparer>::insertion_template_method::before_insert_inner(
    tkey const &key,
    tvalue &&value,
    binary_search_tree<tkey, tvalue, tkey_comparer>::node *&subtree_root_address,
    std::stack<binary_search_tree<tkey, tvalue, tkey_comparer>::node **> &path_to_subtree_root_exclusive)
{

}

template<
    typename tkey,
    typename tvalue,
    typename tkey_comparer>
void binary_search_tree<tkey, tvalue, tkey_comparer>::insertion_template_method::after_insert_inner(
    tkey const &key,
    tvalue &&value,
    binary_search_tree<tkey, tvalue, tkey_comparer>::node *&subtree_root_address,
    std::stack<binary_search_tree<tkey, tvalue, tkey_comparer>::node **> &path_to_subtree_root_exclusive)
{

}

// endregion insertion implementation

// region reading implementation

template<
    typename tkey,
    typename tvalue,
    typename tkey_comparer>
tvalue const &binary_search_tree<tkey, tvalue, tkey_comparer>::reading_template_method::read(
    tkey const &key,
    node *&tree_root_address)
{
    return read_inner(key, tree_root_address, std::stack<node **>());
}

template<
    typename tkey,
    typename tvalue,
    typename tkey_comparer>
tvalue const &binary_search_tree<tkey, tvalue, tkey_comparer>::reading_template_method::read_inner(
    tkey const &key,
    node *&subtree_root_address,
    std::stack<node **> &path_to_subtree_root_exclusive)
{
    before_read_inner(key, subtree_root_address, path_to_subtree_root_exclusive);



    after_read_inner(key, subtree_root_address, path_to_subtree_root_exclusive);
}

template<
    typename tkey,
    typename tvalue,
    typename tkey_comparer>
void binary_search_tree<tkey, tvalue, tkey_comparer>::reading_template_method::before_read_inner(
    tkey const &key,
    binary_search_tree<tkey, tvalue, tkey_comparer>::node *&subtree_root_address,
    std::stack<binary_search_tree<tkey, tvalue, tkey_comparer>::node **> &path_to_subtree_root_exclusive)
{

}

template<
    typename tkey,
    typename tvalue,
    typename tkey_comparer>
void binary_search_tree<tkey, tvalue, tkey_comparer>::reading_template_method::after_read_inner(
    tkey const &key,
    binary_search_tree<tkey, tvalue, tkey_comparer>::node *&subtree_root_address,
    std::stack<binary_search_tree<tkey, tvalue, tkey_comparer>::node **> &path_to_subtree_root_exclusive)
{

}

// endregion reading implementation

// region removing implementation

template<
    typename tkey,
    typename tvalue,
    typename tkey_comparer>
tvalue &&binary_search_tree<tkey, tvalue, tkey_comparer>::removing_template_method::remove(
    tkey const &key,
    binary_search_tree<tkey, tvalue, tkey_comparer>::node *&tree_root_address)
{
    return std::move(remove_inner(key, tree_root_address, std::stack<binary_search_tree<tkey, tvalue, tkey_comparer>::node **>()));
}

template<
    typename tkey,
    typename tvalue,
    typename tkey_comparer>
tvalue &&binary_search_tree<tkey, tvalue, tkey_comparer>::removing_template_method::remove_inner(
    tkey const &key,
    binary_search_tree<tkey, tvalue, tkey_comparer>::node *&subtree_root_address,
    std::stack<binary_search_tree<tkey, tvalue, tkey_comparer>::node **> &path_to_subtree_root_exclusive)
{
    before_remove_inner(key, subtree_root_address, path_to_subtree_root_exclusive);



    after_remove_inner(key, subtree_root_address, path_to_subtree_root_exclusive);
}

template<
    typename tkey,
    typename tvalue,
    typename tkey_comparer>
void binary_search_tree<tkey, tvalue, tkey_comparer>::removing_template_method::before_remove_inner(
    tkey const &key,
    binary_search_tree<tkey, tvalue, tkey_comparer>::node *&subtree_root_address,
    std::stack<binary_search_tree<tkey, tvalue, tkey_comparer>::node **> &path_to_subtree_root_exclusive)
{

}

template<
    typename tkey,
    typename tvalue,
    typename tkey_comparer>
void binary_search_tree<tkey, tvalue, tkey_comparer>::removing_template_method::after_remove_inner(
    tkey const &key,
    binary_search_tree<tkey, tvalue, tkey_comparer>::node *&subtree_root_address,
    std::stack<binary_search_tree<tkey, tvalue, tkey_comparer>::node **> &path_to_subtree_root_exclusive)
{

}

// endregion implementation

// endregion template methods

// region construction, assignment, destruction implementation

template<
    typename tkey,
    typename tvalue,
    typename tkey_comparer>
binary_search_tree<tkey, tvalue, tkey_comparer>::binary_search_tree(
    binary_search_tree::insertion_template_method *insertion,
    binary_search_tree::reading_template_method *reading,
    binary_search_tree::removing_template_method *removing,
    memory *allocator,
    logger *logger)
    : _insertion(insertion),
      _reading(reading),
      _removing(removing),
      _allocator(allocator),
      _logger(logger)
{

}

template<
    typename tkey,
    typename tvalue,
    typename tkey_comparer>
binary_search_tree<tkey, tvalue, tkey_comparer>::binary_search_tree(
    memory *allocator,
    logger *logger):
    _insertion(new insertion_template_method()),
    _reading(new reading_template_method()),
    _removing(new removing_template_method()),
    _allocator(allocator),
    _logger(logger),
    _root(nullptr)
{

}

template<
    typename tkey,
    typename tvalue,
    typename tkey_comparer>
binary_search_tree<tkey, tvalue, tkey_comparer>::binary_search_tree(
    const binary_search_tree &other)
{

}

template<
    typename tkey,
    typename tvalue,
    typename tkey_comparer>
binary_search_tree<tkey, tvalue, tkey_comparer>::binary_search_tree(
    binary_search_tree &&other) noexcept
{
    // TODO
}

template<
    typename tkey,
    typename tvalue,
    typename tkey_comparer>
binary_search_tree<tkey, tvalue, tkey_comparer> &binary_search_tree<tkey, tvalue, tkey_comparer>::operator=(
    const binary_search_tree &other)
{
    if (this == &other)
    {
        return *this;
    }



    return *this;
}

template<
    typename tkey,
    typename tvalue,
    typename tkey_comparer>
binary_search_tree<tkey, tvalue, tkey_comparer> &binary_search_tree<tkey, tvalue, tkey_comparer>::operator=(
    binary_search_tree &&other) noexcept
{
    if (this == &other)
    {
        return *this;
    }



    return *this;
}

template<
    typename tkey,
    typename tvalue,
    typename tkey_comparer>
binary_search_tree<tkey, tvalue, tkey_comparer>::~binary_search_tree()
{
    // TODO: remove all elements

    delete _insertion;
    delete _reading;
    delete _removing;
}

// endregion construction, assignment, destruction implementation

// region associative_container contract implementation

template<
    typename tkey,
    typename tvalue,
    typename tkey_comparer>
void binary_search_tree<tkey, tvalue, tkey_comparer>::insert(
    tkey const &key,
    tvalue &&value)
{
    return _insertion->insert(key, std::move(value), _root);
}

template<
    typename tkey,
    typename tvalue,
    typename tkey_comparer>
tvalue const &binary_search_tree<tkey, tvalue, tkey_comparer>::get(
    tkey const &key)
{
    return _reading->read(key, _root);
}

template<
    typename tkey,
    typename tvalue,
    typename tkey_comparer>
tvalue &&binary_search_tree<tkey, tvalue, tkey_comparer>::remove(
    tkey const &key)
{
    return std::move(_removing->remove(key, _root));
}

// endregion associative_container contract implementation

// region logger_holder contract implementation

template<
    typename tkey,
    typename tvalue,
    typename tkey_comparer>
logger *binary_search_tree<tkey, tvalue, tkey_comparer>::get_logger() const
{
    return _logger;
}

// endregion logger_holder contract implementation

// region iterators requesting implementation

template<
    typename tkey,
    typename tvalue,
    typename tkey_comparer>
typename binary_search_tree<tkey, tvalue, tkey_comparer>::prefix_iterator binary_search_tree<tkey, tvalue, tkey_comparer>::begin_prefix() const noexcept
{

}

template<
    typename tkey,
    typename tvalue,
    typename tkey_comparer>
typename binary_search_tree<tkey, tvalue, tkey_comparer>::prefix_iterator binary_search_tree<tkey, tvalue, tkey_comparer>::end_prefix() const noexcept
{

}

template<
    typename tkey,
    typename tvalue,
    typename tkey_comparer>
typename binary_search_tree<tkey, tvalue, tkey_comparer>::infix_iterator binary_search_tree<tkey, tvalue, tkey_comparer>::begin_infix() const noexcept
{

}

template<
    typename tkey,
    typename tvalue,
    typename tkey_comparer>
typename binary_search_tree<tkey, tvalue, tkey_comparer>::infix_iterator binary_search_tree<tkey, tvalue, tkey_comparer>::end_infix() const noexcept
{

}

template<
    typename tkey,
    typename tvalue,
    typename tkey_comparer>
typename binary_search_tree<tkey, tvalue, tkey_comparer>::postfix_iterator binary_search_tree<tkey, tvalue, tkey_comparer>::begin_postfix() const noexcept
{

}

template<
    typename tkey,
    typename tvalue,
    typename tkey_comparer>
typename binary_search_tree<tkey, tvalue, tkey_comparer>::postfix_iterator binary_search_tree<tkey, tvalue, tkey_comparer>::end_postfix() const noexcept
{

}

// endregion iterators requesting implementation

#endif //SANDBOX_CPP_BINARY_SEARCH_TREE_H
