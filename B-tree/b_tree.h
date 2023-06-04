#ifndef MAI_LABS_B_TREE_H
#define MAI_LABS_B_TREE_H

#include "../associative_container/associative_container.h"
#include "../usefull_files/logger_holder.h"
#include "../logger/logger.h"
#include "../logger/logger_builder_concrete.h"
#include "../logger/logger_builder.h"
#include "../memory/memory_concrete.h"
#include "../usefull_files/memory_holder.h"
#include <iostream>
#include <stack>
#include <list>
#include <vector>



template<
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
class b_tree:
        public associative_container<tkey, tvalue>,
        private logger_holder,
        private memory_holder
        {
protected: // struct
            struct node{
                typename associative_container<tkey, tvalue>::key_value_pair** keys_and_values;
                node** subtrees;
                unsigned int size;
            };

public: // rule of five
            explicit b_tree(size_t t, memory* allocator = nullptr, logger* logger = nullptr);
            b_tree(b_tree const& other);
            b_tree(b_tree &&other) noexcept;
            b_tree &operator=(b_tree const& other);
            b_tree &operator=(b_tree && other) noexcept;
            ~b_tree() override;

public: // get-set-methods
            void insert(
                    tkey const &key,
                    tvalue const &&value) override;

            tvalue const &get(
                    tkey const &key) override;

            tvalue &&remove(
                    tkey const &key) override;

            tvalue& find_diap(tkey const &start, tkey const &end) override;

            void traverse_start();

private: // logger-mem getter
            logger* get_logger() const noexcept override;

            memory* get_memory() const noexcept override;

            void traverse(node** root);

private: // allocation and deletion methods
            node** safe_allocation_node();

            typename associative_container<tkey, tvalue>::key_value_pair** safe_allocation_key_val();

            typename associative_container<tkey, tvalue>::key_value_pair* safe_alloc_pair(tkey const &key, tvalue const &&value);

            node* safe_initialize();

            void delete_leaf(node* leaf);

            void delete_node(node* root);

private: // helper_methods
            void split_child(node** root, int index);

            void insert_non_full(node** parent, typename associative_container<tkey, tvalue>::key_value_pair*);

            std::tuple
            <
            bool, size_t, typename b_tree<tkey, tvalue, tkey_comparer>::node**,
            std::stack<typename b_tree<tkey, tvalue, tkey_comparer>::node **>
            >
            search(tkey const &key) const;

            std::tuple<bool, size_t, node**, std::stack<node**>> find_place(tkey const &key) const;

    class infix_iter{
        std::stack<std::pair<node*, int>> way;
        node* root, *cur;
        int curr_index;
    public:
        explicit infix_iter(b_tree::node** start){
            root = start;
            cur = start;
            curr_index = 0;
        }
        void begin(){
            way.push(std::make_pair(root, 0));
            node* curr = root->subtrees[0];
            while (curr != nullptr){
                way.push(std::make_pair(curr, 0));
                curr = curr->subtrees[0];
            }
            cur = way.top().first;
        }

        tvalue& next(){
//            if(curr_index < cur->size && cur->subtrees[0] == nullptr){
//                return cur->keys_and_values[curr_index++];
//            }
//            while(!way.empty()){
//                way.pop();
//                auto now = way.top();
//                cur = now.first;
//                int index = now.second;
//
//                if (index > cur->size + 1){
//                    way.pop();
//                }
//            }
        }
        node& end(){
            node* curr = *root;
            while(curr->subtrees[0] != nullptr){
                curr = curr->subtrees[curr->size];
            }
            return curr;
        }
    };

private: // fields
    size_t _t; // min degree
    node* _root;
    memory* _allocator;
    logger* _logger;
    tkey_comparer comparer;

    infix_iter begin_traverse() {
        return infix_iter(_root);
    }


};

template<typename tkey, typename tvalue, typename tkey_comparer>
tvalue &b_tree<tkey, tvalue, tkey_comparer>::find_diap(const tkey &start, const tkey &end) {
//    node* curr = _root, *tmp = _root;
//
//    if (curr == nullptr){
//        throw std::runtime_error("no tree");
//    }
//    std::stack<node**> min_way;
//    std::list<tvalue&> values;
//    min_way.push(&_root);
//    while (curr->subtrees[0] != nullptr && comparer(curr->keys_and_values[0]->key, start) > 0){
//        min_way.push(curr);
//        curr = curr->subtrees[0];
//    }
//    int flag = 0;
//    tmp = min_way.top();
//    min_way.pop();
//    while(!min_way.empty() && tmp != _root){
//        for (int i = 0; i < tmp->size; ++i) {
//            values.insert(tmp->keys_and_values[i]->value);
//        }
//        tmp = min_way.top();
//        min_way.pop();
//    }
}

template<typename tkey, typename tvalue, typename tkey_comparer>
void b_tree<tkey, tvalue, tkey_comparer>::traverse_start() {
    traverse(&_root);
}

template<typename tkey, typename tvalue, typename tkey_comparer>
void b_tree<tkey, tvalue, tkey_comparer>::traverse(node** root) {
    if ((*root)->subtrees[0] == nullptr){
        for (int i = 0; i < (*root)->size; ++i) {
            std::cout << (*root)->keys_and_values[i]->key << " ";
        }
        std::cout << std::endl;
    }
    if((*root)->subtrees[0] != nullptr){
        std::string vals;
        for (int i = 0; i < (*root)->size; ++i) {
            traverse(&((*root)->subtrees[i]));
            vals += std::to_string((*root)->keys_and_values[i]->key) + " ";
        }
        traverse(&((*root)->subtrees[(*root)->size]));
        std::cout << vals << std::endl;
    }

}

template<typename tkey, typename tvalue, typename tkey_comparer>
void b_tree<tkey, tvalue, tkey_comparer>::delete_node(b_tree::node *root) {
    if(root != nullptr && root->subtrees[0] == nullptr){
        delete_leaf(root);
    }
    else if (root != nullptr){
        for (int i = 0; i < root->size; ++i) {
            delete_node(root->subtrees[i]);
            root->subtrees[i] = nullptr;
        }
        delete_leaf(root);
    }
}

template<typename tkey, typename tvalue, typename tkey_comparer>
void b_tree<tkey, tvalue, tkey_comparer>::delete_leaf(b_tree::node *leaf) {
    for (int i = 0; i < leaf->size; ++i) {
        leaf->keys_and_values[i]->~key_value_pair();
        deallocate_with_guard(reinterpret_cast<void*>(leaf->keys_and_values[i]));
        leaf->keys_and_values[i] = nullptr;
    }
    deallocate_with_guard(reinterpret_cast<void*>(leaf->keys_and_values));
    leaf->keys_and_values = nullptr;
    deallocate_with_guard(reinterpret_cast<void*>(leaf->subtrees));
    leaf->subtrees = nullptr;
    leaf->~node();
    deallocate_with_guard(reinterpret_cast<void*>(leaf));
}

template<typename tkey, typename tvalue, typename tkey_comparer>
typename associative_container<tkey, tvalue>::key_value_pair *b_tree<tkey, tvalue, tkey_comparer>::safe_alloc_pair(tkey const &key, tvalue const &&value) {
    auto mem = reinterpret_cast<typename associative_container<tkey, tvalue>::key_value_pair*>(allocate_with_guard(sizeof(typename associative_container<tkey, tvalue>::key_value_pair)));
    new (mem) typename associative_container<tkey, tvalue>::key_value_pair{key, std::move(value)};
//    auto n_mem = dynamic_cast<typename associative_container<tkey, tvalue>::key_value_pair*>(mem);
//    mem->key = nullptr;
//    mem->value = nullptr;
    return mem;
}




// RULE OF FIVE
template<typename tkey, typename tvalue, typename tkey_comparer>
b_tree<tkey, tvalue, tkey_comparer>::b_tree(size_t t, memory* allocator, logger* logger) :
_root(nullptr), _allocator(allocator), _logger(logger)
{
    _t = t < 2 ? 2 : t;
    trace_with_guard("B-tree class have been created");
}



template<typename tkey, typename tvalue, typename tkey_comparer>
b_tree<tkey, tvalue, tkey_comparer>::~b_tree(){
    try {
        delete_node(_root);
        warning_with_guard("done");
    }
    catch (std::exception exception) {
        std::cerr << exception.what();
    }
}

template<typename tkey, typename tvalue, typename tkey_comparer>
b_tree<tkey, tvalue, tkey_comparer>::b_tree(b_tree const& other){
    comparer = other.comparer;
    _root = nullptr;
    _allocator = other._allocator;
    _logger = other._logger;
    _t = other._t;
//    TODO COPY TREE
}

template<typename tkey, typename tvalue, typename tkey_comparer>
b_tree<tkey, tvalue, tkey_comparer>::b_tree(b_tree &&other) noexcept{
    _root = std::move(other._root);
    comparer = std::move(other.comparer);
    _t = std::move(other._t);
    _logger = std::move(other._logger);
    _allocator = std::move(other._allocator);
}

template<typename tkey, typename tvalue, typename tkey_comparer>
b_tree<tkey, tvalue, tkey_comparer> &b_tree<tkey, tvalue, tkey_comparer>::operator=(b_tree const& other){
    if(this == &other){
        return *this;
    }
    _root = nullptr;
    comparer = other.comparer;
    _logger = other._logger;
    _allocator = other._allocator;
    _t = other._t;
//    TODO COPY

    return *this;
}

template<typename tkey, typename tvalue, typename tkey_comparer>
b_tree<tkey, tvalue, tkey_comparer> &b_tree<tkey, tvalue, tkey_comparer>::operator=(b_tree && other) noexcept{
    if(this == &other){
        return *this;
    }
    _root = std::move(other._root);
    comparer = std::move(other.comparer);
    _t = std::move(other._t);
    _logger = std::move(other._logger);
    _allocator = std::move(other._allocator);

    return *this;
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
memory* b_tree<tkey, tvalue, tkey_comparer>::get_memory() const noexcept{
    return _allocator;
}

template<typename tkey, typename tvalue, typename tkey_comparer>
typename b_tree<tkey, tvalue, tkey_comparer>::node** b_tree<tkey, tvalue, tkey_comparer>::safe_allocation_node(){
    auto mem = allocate_with_guard(sizeof(typename b_tree<tkey, tvalue, tkey_comparer>::node*)*(2*_t));
    if (mem == nullptr){
        throw std::runtime_error("COLLAPSED");
    }
    auto steady = reinterpret_cast<typename b_tree<tkey, tvalue, tkey_comparer>::node**>(mem);
    for (int i = 0; i < (2*_t); ++i) {
        steady[i] = nullptr;
    }
    return steady;
}

template<typename tkey, typename tvalue, typename tkey_comparer>
typename associative_container<tkey, tvalue>::key_value_pair **b_tree<tkey, tvalue, tkey_comparer>::safe_allocation_key_val() {
    auto mem = reinterpret_cast<
            typename associative_container<tkey, tvalue>::key_value_pair**>
    (allocate_with_guard(sizeof(typename associative_container<tkey, tvalue>::key_value_pair*)*(2*_t - 1)));
    for (int i = 0; i < ((2*_t) - 1); ++i) {
        mem[i] = nullptr;
    }
    return mem;
}

template<typename tkey, typename tvalue, typename tkey_comparer>
typename b_tree<tkey, tvalue, tkey_comparer>::node* b_tree<tkey, tvalue, tkey_comparer>::safe_initialize() {
    auto mem = reinterpret_cast<node*>(allocate_with_guard(sizeof(typename b_tree<tkey, tvalue, tkey_comparer>::node*)));
    if (mem == nullptr){
        throw std::runtime_error("COLLAPSED");
    }
    auto keys = safe_allocation_key_val();
    auto roots = safe_allocation_node();
    node* root = new (mem) node{keys, roots, 0};
//    root->size = 0;
//    root->keys_and_values = safe_allocation_key_val();
//    root->subtrees = safe_allocation_node();
    return root;
}

// SAVE ALLOC ENDS

//B-tree METHODS
template<typename tkey, typename tvalue, typename tkey_comparer>
void b_tree<tkey, tvalue, tkey_comparer>::insert_non_full(b_tree::node **parent,
                                                          typename associative_container<tkey, tvalue>::key_value_pair * pair) {
    int n = (*parent)->size - 1;
    if ((*parent)->subtrees[0] == nullptr){
        while (n >= 0 && comparer(pair->key, (*parent)->keys_and_values[n]->key) < 0){
            (*parent)->keys_and_values[n + 1] = (*parent)->keys_and_values[n];
            n -= 1;
        }
        (*parent)->keys_and_values[n + 1] = pair;
        (*parent)->size += 1;
    }
    else{
        while (n >= 0 && comparer(pair->key, (*parent)->keys_and_values[n]->key) < 0){
            n -= 1;
        }
        n += 1;
        if ((*parent)->subtrees[n]->size == (2*_t - 1)){
            split_child(parent, n);
            if (comparer(pair->key, (*parent)->keys_and_values[n]->key) > 0){
                n += 1;
            }
        }
        insert_non_full(&(*parent)->subtrees[n], pair);
    }
}

template<
        typename tkey,
        typename tvalue,
        typename tkey_comparer>
void b_tree<tkey, tvalue, tkey_comparer>::split_child(
        b_tree::node **root,
        int index) {
    warning_with_guard("starting to split with head val ");
    node* z = safe_initialize();
    node* y = (*root)->subtrees[index];
    z->size = _t - 1;
    for (int i = 0; i < (_t - 1); ++i) {
//        _logger->log("splitting child, giving z keys on pos " + std::to_string(i + _t), logger::severity::critical);
        z->keys_and_values[i] = y->keys_and_values[i + _t];
    }
    if (y->subtrees[0] != nullptr){
        for (int i = 0; (i < _t); ++i) {
//            _logger->log("splitting child, giving z trees on pos " + std::to_string(i + _t), logger::severity::critical);
            z->subtrees[i] = y->subtrees[i + _t];
        }
    }
    y->size = _t - 1;
    for (int i = (*root)->size; i > index; --i) {
//        _logger->log("splitting child, giving parent trees on pos " + std::to_string(i + 1) + " from " + std::to_string(i), logger::severity::critical);
        (*root)->subtrees[i + 1] = (*root)->subtrees[i];
    }
//    _logger->log("splitting child, giving parent z subtree on pos " + std::to_string(index + 1), logger::severity::critical);
    (*root)->subtrees[index + 1] = z;
    for (int i = (*root)->size - 1; i > index; --i) {
//        _logger->log("splitting child, moving parent keys on pos " + std::to_string(i + 1) + " from " + std::to_string(i), logger::severity::critical);
        (*root)->keys_and_values[i+1] = (*root)->keys_and_values[i];
    }
//    warning_with_guard("Suka");
//    _logger->log("setting to parent y _t - 1 key" + std::to_string(index) + " from " + std::to_string(_t - 1), logger::severity::critical);
    (*root)->keys_and_values[index] = y->keys_and_values[_t - 1];
    (*root)->size += 1;
}


template<typename tkey, typename tvalue, typename tkey_comparer>
std::tuple<bool, size_t, typename b_tree<tkey, tvalue, tkey_comparer>::node**, std::stack<typename b_tree<tkey, tvalue, tkey_comparer>::node **>>
b_tree<tkey, tvalue, tkey_comparer>::find_place(const tkey &key) const{
    std::stack<node**> way;
    node* curr = _root;
    while(curr != nullptr){
        for(int i = 0; i < (2*_t-1); i++){
            if(curr->keys[i] == nullptr){
                return std::make_tuple<bool, size_t, node* , std::stack<node *>>(true, i, curr, way);
            }
            else if(comparer(curr->keys[i], key) > 0){
                way.push(&curr);
                curr = curr->pointers[i];
                break;
            }
        }
        if (curr->pointers[0] == nullptr){
            return std::tuple<bool, size_t, node* , std::stack<node *>>(false, 0, nullptr, way);
        }
    }
}




template<typename tkey, typename tvalue, typename tkey_comparer>
std::tuple<bool, size_t, typename b_tree<tkey, tvalue, tkey_comparer>::node**, std::stack<typename b_tree<tkey, tvalue, tkey_comparer>::node **>>
b_tree<tkey, tvalue, tkey_comparer>::search(tkey const &key) const{
    auto curr = _root;
    std::stack<typename b_tree<tkey, tvalue, tkey_comparer>::node **> trace;
    trace.push(nullptr);
    while(true){
        int i = 0;
        while(comparer(curr->keys_and_values[i]->key, key) < 0){
            i++;
        }
        if (comparer(curr->keys_and_values[i]->key, key) == 0){
            return std::tuple<bool, size_t, node**, std::stack<node **>>(curr->subtrees[0] == nullptr, i, &curr, trace);
        }
        if(curr->subtrees[0] == nullptr){
            return std::tuple<bool, size_t, node**, std::stack<node **>>(true, -1, nullptr, trace);
        }
        else{
            trace.push(&curr);
            curr = curr->subtrees[i];
        }
    }
}

template<typename tkey, typename tvalue, typename tkey_comparer>
tvalue const &b_tree<tkey, tvalue, tkey_comparer>::get(tkey const &key){
//    auto root = _reading->read(key, &_root);
    auto root = search(key);
    int index = std::get<1>(root);
    node** res_node = std::get<2>(root);

    if (res_node == nullptr){
        throw std::runtime_error("NO VALUE LIKE THIS");
    }
    return (*res_node)->keys_and_values[index]->value;
}

template<typename tkey, typename tvalue, typename tkey_comparer>
tvalue &&b_tree<tkey, tvalue, tkey_comparer>::remove(tkey const &key){
    if(_root == nullptr){
        critical_with_guard("Root have not been initialized!");
        throw std::runtime_error("critical");
    }
    auto removing = search(key);
    int index = std::get<1>(removing);
    if (index == -1){
        throw std::runtime_error("critical");
    }
    return std::move((*std::get<2>(removing))->keys_and_values[index]->value);
}


template<typename tkey, typename tvalue, typename tkey_comparer>
void b_tree<tkey, tvalue, tkey_comparer>::insert(tkey const &key, tvalue const &&value){
//    _insertion->insert(key, std::move(value));
    b_tree<tkey, tvalue, tkey_comparer>::node* r = this->_root;
    if(_root == nullptr){
        _root = safe_initialize();
        auto pair = safe_alloc_pair(key, std::move(value));
        _root->keys_and_values[0] = std::move(pair);
        _root->size += 1;
    }
    else{
        auto pair = safe_alloc_pair(key, std::move(value));
        if(r->size ==( 2*_t - 1)){
            b_tree<tkey, tvalue, tkey_comparer>::node* root = safe_initialize();
            root->subtrees[0] = r;
            root->size = 0;
            _root = root;

            split_child(&root, 0);
            insert_non_full(&root, pair);
        }
        else{
            insert_non_full(&r, pair);
        }
    }
}


#endif //MAI_LABS_B_TREE_H
