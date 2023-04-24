#ifndef MAI_LABS_B_TREE_H
#define MAI_LABS_B_TREE_H

#include "../associative_container/associative_container.h"
#include "../usefull_files/logger_holder.h"
#include "../logger/logger.h"
#include "../logger/logger_builder_concrete.h"
#include "../logger/logger_builder.h"
#include "../memory/memory_concrete.h"

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
                tkey* key; // min: t - 1, max: 2(t-1)
                tvalue* value;
                node* pointers; // key_count + 1
            };
private:
            int t; // min degree
            node* _root;
            memory* _allocator;
            logger* _logger;
//            insertion_template_method *_insertion;
//            reading_template_method *_reading;
//            removing_template_method *_removing;
public:
            explicit b_tree(memory* allocator = nullptr, logger* logger = nullptr);
            b_tree(b_tree const& other);
            b_tree(b_tree &&other) noexcept;
            b_tree &operator=(b_tree const& other);
            b_tree &operator=(b_tree const&& other) noexcept;
            ~b_tree();
public:
            void insert(
                    tkey const &key,
                    tvalue &&value) final;

            tvalue const &get(
                    tkey const &key) final;

            tvalue &&remove(
                    tkey const &key) final;
private:
            logger* get_logger() const noexcept override;
//public:
//
//    prefix_iterator begin_prefix() const noexcept;
//
//    prefix_iterator end_prefix() const noexcept;
//
//    infix_iterator begin_infix() const noexcept;
//
//    infix_iterator end_infix() const noexcept;
//
//    postfix_iterator begin_postfix() const noexcept;
//
//    postfix_iterator end_postfix() const noexcept;

};

#endif //MAI_LABS_B_TREE_H
