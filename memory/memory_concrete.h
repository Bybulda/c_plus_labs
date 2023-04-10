#include "memory.h"
#include "../logger/logger.h"
#include "../logger/logger_builder_concrete.h"
#include "../logger/logger_builder.h"

class memory_concrete: public memory{
private:
    logger* loggerr;
    void log_with_guard(const std::string& str, logger::severity level) const;
public:
    ~memory_concrete();
    
    memory_concrete();
    
    memory_concrete(memory_concrete const&) = delete;

    memory_concrete& operator=(memory_concrete const&) = delete;

    void * allocate(size_t target_size) const override;

    void set_logger(logger* &lg) noexcept override;
    
    void deallocate(void const * const target_to_dealloc) const override;
    
    template<typename T>
    std::string to_str(T const &object) const noexcept;
};