#ifndef MAI_LABS_B_TREE_H
#define MAI_LABS_B_TREE_H

#include "../associative_container/associative_container.h"
#include "../usefull_files/logger_holder.h"
#include "../logger/logger.h"
#include "../logger/logger_builder_concrete.h"
#include "../logger/logger_builder.h"
#include "../memory/memory_concrete.h"
#include <iostream>
#include <stack>

template<
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
class b_tree:
        public associative_container<tkey, tvalue>,
        private logger_holder
        {
protected:
            struct node{
            public:
                explicit node(node *pNode): _tree(this) {
                    leaf = pNode->leaf;
                    key = std::move<tkey*>(pNode->key);
                    value = std::move<tvalue*>(pNode->value);
                    size = pNode->size;
                    pointers = std::move<node**>(pNode->pointers);
                }

                tkey* key; // min: t - 1, max: 2(t-1)
                tvalue* value;
                node** pointers; // key_count + 1
                size_t size;
                b_tree* _tree;
                bool leaf;
            public:
                node(tkey* keys, tvalue* val): _tree(this){
                    key = _tree->safe_allocation_keys();
                    value = _tree->safe_allocation_values();
                    pointers = _tree->safe_allocation_node();
                    size = 0;
                    key[0] = keys;
                    value[0] = val;
                    leaf = true;
                }
                ~node(){
//                    TODO: safe deallocation
                    if (_tree->get_allocator() != nullptr){
                        _tree->get_allocator()->deallocate(key);
                        _tree->get_allocator()->deallocate(value);
                        _tree->get_allocator()->deallocate(pointers);
                    }
                    else{
                        delete(key);
                        delete(value);
                        delete(pointers);
                    }
                }

            };
public:
            explicit b_tree(size_t t, memory* allocator = nullptr, logger* logger = nullptr);
            b_tree(b_tree const& other);
            b_tree(b_tree &&other) noexcept;
            b_tree &operator=(b_tree const& other);
            b_tree &operator=(b_tree && other) noexcept;
            ~b_tree() override;
public:
            void insert(
                    tkey const &key,
                    tvalue const &&value) final;

            tvalue const &get(
                    tkey const &key) final;

            tvalue &&remove(
                    tkey const &key) final;
private:
            logger* get_logger() const noexcept override;

            memory* get_allocator() const noexcept;

            node** safe_allocation_node() noexcept;

            tkey* safe_allocation_keys() noexcept;

            tvalue* safe_allocation_values() noexcept;

            node* safe_initialize(tkey const &key, tvalue const &&value) noexcept;

            node const &search(tkey const &key, node const &root) const;
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

        const prefix_iterator operator++(
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

        const infix_iterator operator++(
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

        const postfix_iterator operator++(
                int not_used);

        std::tuple<unsigned int, tkey const&, tvalue const&> operator*() const;

    };
private:
    size_t _t; // min degree
    node* _root;
    memory* _allocator;
    logger* _logger;
    insertion_template_method *_insertion;
    reading_template_method *_reading;
    removing_template_method *_removing;
public:

    prefix_iterator begin_prefix() const noexcept;

    prefix_iterator end_prefix() const noexcept;

    infix_iterator begin_infix() const noexcept;

    infix_iterator end_infix() const noexcept;

    postfix_iterator begin_postfix() const noexcept;

    postfix_iterator end_postfix() const noexcept;

};


// RULE OF FIVE
template<typename tkey, typename tvalue, typename tkey_comparer>
b_tree<tkey, tvalue, tkey_comparer>::b_tree(size_t t, memory* allocator, logger* logger){
    _allocator = allocator;
    _logger = logger;
    _root = nullptr;
    this->_t = t < 2 ? 2 : t;
    this->trace_with_guard("B-tree class have been created");
}

template<typename tkey, typename tvalue, typename tkey_comparer>
b_tree<tkey, tvalue, tkey_comparer>::~b_tree(){

}

template<typename tkey, typename tvalue, typename tkey_comparer>
b_tree<tkey, tvalue, tkey_comparer>::b_tree(b_tree const& other){

}

template<typename tkey, typename tvalue, typename tkey_comparer>
b_tree<tkey, tvalue, tkey_comparer>::b_tree(b_tree &&other) noexcept{

}

template<typename tkey, typename tvalue, typename tkey_comparer>
b_tree<tkey, tvalue, tkey_comparer> &b_tree<tkey, tvalue, tkey_comparer>::operator=(b_tree const& other){

}

template<typename tkey, typename tvalue, typename tkey_comparer>
b_tree<tkey, tvalue, tkey_comparer> &b_tree<tkey, tvalue, tkey_comparer>::operator=(b_tree && other) noexcept{

}

// RULE OF FIVE END

// LOGGER
template<typename tkey, typename tvalue, typename tkey_comparer>
logger* b_tree<tkey, tvalue, tkey_comparer>::get_logger() const noexcept{
    return _logger;
}
// LOGGER END

// SAVE ALLOC
template<typename tkey, typename tvalue, typename tkey_comparer>
memory* b_tree<tkey, tvalue, tkey_comparer>::get_allocator() const noexcept{
    return _allocator;
}

template<typename tkey, typename tvalue, typename tkey_comparer>
typename b_tree<tkey, tvalue, tkey_comparer>::node** b_tree<tkey, tvalue, tkey_comparer>::safe_allocation_node() noexcept{
    memory* allocator = get_allocator();
    if (allocator != nullptr){
        auto** mem = reinterpret_cast<void**>(allocator->allocate(sizeof(node*)*2*_t));
        if (mem == nullptr){
            throw std::runtime_error("OUT OF MEMORY");
        }
        node** root = reinterpret_cast<node**>(mem);
        return root;
    }
    node** root = new node*[2*_t];
    return root;
}

template<typename tkey, typename tvalue, typename tkey_comparer>
tkey* b_tree<tkey, tvalue, tkey_comparer>::safe_allocation_keys()  noexcept{
    memory* allocator = get_allocator();
    if (allocator != nullptr){
        void* mem = allocator->allocate(sizeof(tvalue)*(2*_t-1));
        if (mem == nullptr){
            throw std::runtime_error("OUT OF MEMORY");
        }
        tkey* root = reinterpret_cast<tvalue*>(mem);
        return root;
    }
    return new tkey[2*_t-1];
}

template<typename tkey, typename tvalue, typename tkey_comparer>
tvalue* b_tree<tkey, tvalue, tkey_comparer>::safe_allocation_values()  noexcept{
    memory* allocator = get_allocator();
    if (allocator != nullptr){
        void* mem = allocator->allocate(sizeof(tvalue)*(2*_t));
        if (mem == nullptr){
            throw std::runtime_error("OUT OF MEMORY");
        }
        auto* root = reinterpret_cast<tvalue*>(mem);
        return root;
    }
    return new tvalue[2*_t];
}

template<typename tkey, typename tvalue, typename tkey_comparer>
typename b_tree<tkey, tvalue, tkey_comparer>::node* b_tree<tkey, tvalue, tkey_comparer>::safe_initialize(tkey const &key, tvalue const &&value) noexcept {
    memory* allocator = get_allocator();
    if(allocator == nullptr){
        return new node(this, key, std::forward<tvalue const>(value));
    }
    return new (allocator->allocate(sizeof(node))) node(key, std::forward<tvalue const>(value));
}

// SAVE ALLOC ENDS

//B-tree METHODS
template<typename tkey, typename tvalue, typename tkey_comparer>
void b_tree<tkey, tvalue, tkey_comparer>::insert(tkey const &key, tvalue const &&value){
    if(_root == nullptr){
        _root = safe_initialize(key, std::forward<const tvalue>(value));
        return;
    }
    


}

template<typename tkey, typename tvalue, typename tkey_comparer>
typename b_tree<tkey, tvalue, tkey_comparer>::node const &b_tree<tkey, tvalue, tkey_comparer>::search(tkey const &key, node const &root) const{
    size_t i = 0;
    while(i < root.size && key > root.key[i]){
        i++;
    }
    if(root.key[i] == key){
        return root;
    }
    if (root.leaf == true)
        throw std::runtime_error("NOT FOUND");
//    node const & next = root.pointers[i];
    return search(key, std::forward<node const &>(root.pointers[i]));
}

template<typename tkey, typename tvalue, typename tkey_comparer>
tvalue const &b_tree<tkey, tvalue, tkey_comparer>::get(tkey const &key){
    if(_root == nullptr){
        critical_with_guard("Root have not been initialized!");
        return static_cast<tvalue>(nullptr);
    }
    node* root = this->search(key, _root);

    if (root == nullptr){
        return static_cast<tvalue>(nullptr);
    }
    return root->value[0];
}

template<typename tkey, typename tvalue, typename tkey_comparer>
tvalue &&b_tree<tkey, tvalue, tkey_comparer>::remove(tkey const &key){
    if(_root == nullptr){
        critical_with_guard("Root have not been initialized!");
        return;
    }
}
//B-tree METHODS END

// INSERTING METHOD
template<typename tkey, typename tvalue, typename tkey_comparer>
void b_tree<tkey, tvalue, tkey_comparer>::insertion_template_method::insert(const tkey &key, tvalue &&value,
                                                                            b_tree::node *&tree_root_address) {

}

template<typename tkey, typename tvalue, typename tkey_comparer>
void b_tree<tkey, tvalue, tkey_comparer>::insertion_template_method::insert_inner(const tkey &key, tvalue &&value,
                                                                                  b_tree::node *&subtree_root_address,
                                                                                  std::stack<node **> &path_to_subtree_root_exclusive) {

}

template<typename tkey, typename tvalue, typename tkey_comparer>
void
b_tree<tkey, tvalue, tkey_comparer>::insertion_template_method::before_insert_inner(const tkey &key, tvalue &&value,
                                                                                    b_tree::node *&subtree_root_address,
                                                                                    std::stack<node **> &path_to_subtree_root_exclusive) {

}

template<typename tkey, typename tvalue, typename tkey_comparer>
void b_tree<tkey, tvalue, tkey_comparer>::insertion_template_method::after_insert_inner(const tkey &key, tvalue &&value,
                                                                                        b_tree::node *&subtree_root_address,
                                                                                        std::stack<node **> &path_to_subtree_root_exclusive) {

}
// INSERTING END

// READING END
template<typename tkey, typename tvalue, typename tkey_comparer>
tvalue const &
b_tree<tkey, tvalue, tkey_comparer>::reading_template_method::read(const tkey &key, b_tree::node *&tree_root_address) {
    return <#initializer#>;
}

template<typename tkey, typename tvalue, typename tkey_comparer>
tvalue const &b_tree<tkey, tvalue, tkey_comparer>::reading_template_method::read_inner(const tkey &key,
                                                                                       b_tree::node *&subtree_root_address,
                                                                                       std::stack<node **> &path_to_subtree_root_exclusive) {
    return <#initializer#>;
}

template<typename tkey, typename tvalue, typename tkey_comparer>
void b_tree<tkey, tvalue, tkey_comparer>::reading_template_method::before_read_inner(const tkey &key,
                                                                                     b_tree::node *&subtree_root_address,
                                                                                     std::stack<node **> &path_to_subtree_root_exclusive) {

}

template<typename tkey, typename tvalue, typename tkey_comparer>
void b_tree<tkey, tvalue, tkey_comparer>::reading_template_method::after_read_inner(const tkey &key,
                                                                                    b_tree::node *&subtree_root_address,
                                                                                    std::stack<node **> &path_to_subtree_root_exclusive) {

}
// READING END

// REMOVING METHOD
template<typename tkey, typename tvalue, typename tkey_comparer>
tvalue &&b_tree<tkey, tvalue, tkey_comparer>::removing_template_method::remove(const tkey &key,
                                                                               b_tree::node *&tree_root_address) {
    return <#initializer#>;
}

template<typename tkey, typename tvalue, typename tkey_comparer>
tvalue &&b_tree<tkey, tvalue, tkey_comparer>::removing_template_method::remove_inner(const tkey &key,
                                                                                     b_tree::node *&subtree_root_address,
                                                                                     std::stack<node **> &path_to_subtree_root_exclusive) {
    return <#initializer#>;
}

template<typename tkey, typename tvalue, typename tkey_comparer>
void b_tree<tkey, tvalue, tkey_comparer>::removing_template_method::before_remove_inner(const tkey &key,
                                                                                        b_tree::node *&subtree_root_address,
                                                                                        std::stack<node **> &path_to_subtree_root_exclusive) {

}

template<typename tkey, typename tvalue, typename tkey_comparer>
void b_tree<tkey, tvalue, tkey_comparer>::removing_template_method::after_remove_inner(const tkey &key,
                                                                                       b_tree::node *&subtree_root_address,
                                                                                       std::stack<node **> &path_to_subtree_root_exclusive) {

}
// REMOVING END


// PREFIX ITERATOR IMPLEMENTATION
template<typename tkey, typename tvalue, typename tkey_comparer>
b_tree<tkey, tvalue, tkey_comparer>::prefix_iterator::prefix_iterator(b_tree::node *tree_root) {

}

template<typename tkey, typename tvalue, typename tkey_comparer>
bool b_tree<tkey, tvalue, tkey_comparer>::prefix_iterator::operator==(const b_tree::prefix_iterator &other) const {
    return false;
}

template<typename tkey, typename tvalue, typename tkey_comparer>
typename b_tree<tkey, tvalue, tkey_comparer>::prefix_iterator &b_tree<tkey, tvalue, tkey_comparer>::prefix_iterator::operator++() {
    return <#initializer#>;
}

template<typename tkey, typename tvalue, typename tkey_comparer>
const typename b_tree<tkey, tvalue, tkey_comparer>::prefix_iterator b_tree<tkey, tvalue, tkey_comparer>::prefix_iterator::operator++(int not_used) {
    return b_tree::prefix_iterator(nullptr);
}

template<typename tkey, typename tvalue, typename tkey_comparer>
std::tuple<unsigned int, tkey const &, tvalue const &>
b_tree<tkey, tvalue, tkey_comparer>::prefix_iterator::operator*() const {
    return std::tuple<unsigned int, const tkey &, const tvalue &>();
}
// PREFIX ITERATOR END

// INFIX ITERATOR IMPLEMENTATION
template<typename tkey, typename tvalue, typename tkey_comparer>
b_tree<tkey, tvalue, tkey_comparer>::infix_iterator::infix_iterator(b_tree::node *tree_root) {

}

template<typename tkey, typename tvalue, typename tkey_comparer>
bool b_tree<tkey, tvalue, tkey_comparer>::infix_iterator::operator==(const b_tree::infix_iterator &other) const {
    return false;
}

template<typename tkey, typename tvalue, typename tkey_comparer>
typename b_tree<tkey, tvalue, tkey_comparer>::infix_iterator &b_tree<tkey, tvalue, tkey_comparer>::infix_iterator::operator++() {
    return <#initializer#>;
}

template<typename tkey, typename tvalue, typename tkey_comparer>
const typename b_tree<tkey, tvalue, tkey_comparer>::infix_iterator b_tree<tkey, tvalue, tkey_comparer>::infix_iterator::operator++(int not_used) {
    return b_tree::infix_iterator(nullptr);
}

template<typename tkey, typename tvalue, typename tkey_comparer>
std::tuple<unsigned int, tkey const &, tvalue const &>
b_tree<tkey, tvalue, tkey_comparer>::infix_iterator::operator*() const {
    return std::tuple<unsigned int, const tkey &, const tvalue &>();
}
// INFIX ITERATOR END

// POSTFIX ITERATOR IMPLEMENTATION
template<typename tkey, typename tvalue, typename tkey_comparer>
b_tree<tkey, tvalue, tkey_comparer>::postfix_iterator::postfix_iterator(b_tree::node *tree_root) {

}

template<typename tkey, typename tvalue, typename tkey_comparer>
bool b_tree<tkey, tvalue, tkey_comparer>::postfix_iterator::operator==(const b_tree::postfix_iterator &other) const {
    return false;
}

template<typename tkey, typename tvalue, typename tkey_comparer>
typename b_tree<tkey, tvalue, tkey_comparer>::postfix_iterator &b_tree<tkey, tvalue, tkey_comparer>::postfix_iterator::operator++() {
    return <#initializer#>;
}

template<typename tkey, typename tvalue, typename tkey_comparer>
const typename b_tree<tkey, tvalue, tkey_comparer>::postfix_iterator b_tree<tkey, tvalue, tkey_comparer>::postfix_iterator::operator++(int not_used) {
    return b_tree::postfix_iterator(nullptr);
}

template<typename tkey, typename tvalue, typename tkey_comparer>
std::tuple<unsigned int, tkey const &, tvalue const &>
b_tree<tkey, tvalue, tkey_comparer>::postfix_iterator::operator*() const {
    return std::tuple<unsigned int, const tkey &, const tvalue &>();
}
// POSTFIX ITERATOR END

// ITERATOR METHODS
template<typename tkey, typename tvalue, typename tkey_comparer>
typename b_tree<tkey, tvalue, tkey_comparer>::prefix_iterator b_tree<tkey, tvalue, tkey_comparer>::begin_prefix() const noexcept {
    return b_tree::prefix_iterator(nullptr);
}

template<typename tkey, typename tvalue, typename tkey_comparer>
typename b_tree<tkey, tvalue, tkey_comparer>::prefix_iterator b_tree<tkey, tvalue, tkey_comparer>::end_prefix() const noexcept {
    return b_tree::prefix_iterator(nullptr);
}

template<typename tkey, typename tvalue, typename tkey_comparer>
typename b_tree<tkey, tvalue, tkey_comparer>::infix_iterator b_tree<tkey, tvalue, tkey_comparer>::begin_infix() const noexcept {
    return b_tree::infix_iterator(nullptr);
}

template<typename tkey, typename tvalue, typename tkey_comparer>
typename b_tree<tkey, tvalue, tkey_comparer>::infix_iterator b_tree<tkey, tvalue, tkey_comparer>::end_infix() const noexcept {
    return b_tree::infix_iterator(nullptr);
}

template<typename tkey, typename tvalue, typename tkey_comparer>
typename b_tree<tkey, tvalue, tkey_comparer>::postfix_iterator b_tree<tkey, tvalue, tkey_comparer>::begin_postfix() const noexcept {
    return b_tree::postfix_iterator(nullptr);
}

template<typename tkey, typename tvalue, typename tkey_comparer>
typename b_tree<tkey, tvalue, tkey_comparer>::postfix_iterator b_tree<tkey, tvalue, tkey_comparer>::end_postfix() const noexcept {
    return b_tree::postfix_iterator(nullptr);
}
// ITERATORS END
#endif //MAI_LABS_B_TREE_H
