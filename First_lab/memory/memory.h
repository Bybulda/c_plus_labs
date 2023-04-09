#ifndef MEMORY
#define MEMORY
#include "../logger/logger.h"
#include "../logger/logger_builder_concrete.h"
#include "../logger/logger_builder.h"

class memory{
    public:
        virtual ~memory();

        virtual void * allocate(size_t target_size) const = 0;
        
        virtual void deallocate(void const * const target_to_dealloc) const = 0;

        memory() = default;

        memory(memory const&) = delete;

        memory& operator=(memory const&) = delete;

        void* operator+=(size_t const&);

        void operator-=(void const * const object);

        virtual void set_logger(logger* &lg) noexcept = 0;
};
#endif