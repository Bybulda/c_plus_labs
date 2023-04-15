#ifndef ALLOC_TYPES
#define ALLOC_TYPES

#include "memory.h"

class alloc_types: public memory{
    public:

        alloc_types(size_t const& size, memory* alloc=nullptr, logger* logg=nullptr, memory::method mode=memory::method::first);
        
        ~alloc_types();
        
        void* allocate(size_t target_size) override;

        void deallocate(void const * const target_to_dealloc) const override;

        void* get_mem(size_t size);
        
        void set_logger(logger* &lg) noexcept override;
    
        template<typename T>
        std::string to_str(T const &object) const noexcept;
    
    private:
        void* _memory;
        
        memory::method _method;

    private:
        logger* _get_logger() const;

        void _log_with_guard(const std::string& str, logger::severity level) const noexcept override;

        std::string _return_memory_condition_info(void* memory) const noexcept;

    private:
        void* _get_next_block(void* current=nullptr) const noexcept;

        size_t _get_block_size(void* current=nullptr) const noexcept;

        void* _key_memory(void* try_mem) const noexcept;

        
    private:
        void* _get_memory(size_t const &size, void** previous, void** next);

        void* _method_first(size_t const &size, void** previous, void** next);

        void* _method_best(size_t const &size, void** previous, void** next);

        void* _method_worst(size_t const &size, void** previous, void** next);

    private:
        void _pull_together();
};

#endif