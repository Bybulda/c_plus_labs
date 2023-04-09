#include "alloc_types.h"
#include <iostream>
#include <sstream>


alloc_types::alloc_types(size_t const& size, memory* alloc, logger* logg, memory::method mode){
    
    if (alloc == nullptr){
        _memory = ::operator new(size);
    }
    else{
        _memory = alloc->allocate(size);
    }
    _method = mode;
    
    size_t* with_size = reinterpret_cast<size_t*>(_memory);
    *with_size = size;

    void** ptr_next = reinterpret_cast<void**>(with_size + 1);
    *ptr_next = reinterpret_cast<void*>(reinterpret_cast<unsigned char*>(_memory) + sizeof(void*) + sizeof(size_t) + sizeof(logger*) + sizeof(memory*));

    logger** ptr_logger = reinterpret_cast<logger**>(reinterpret_cast<unsigned char*>(_memory) + sizeof(size_t) + sizeof(void*));
    *ptr_logger = logg;

    memory** mem = reinterpret_cast<memory**>(ptr_logger + 1);
    *mem = alloc;

    size_t* free_block = reinterpret_cast<size_t*>(*ptr_next);
    *free_block = size - sizeof(size_t) - sizeof(void*) - sizeof(logger*) - sizeof(memory*) - sizeof(size_t) - sizeof(void*);

    *reinterpret_cast<void**>(free_block + 1) = nullptr;
    this->_log_with_guard("Allocator was created with size " + to_str(size) + " on block " + to_str(_memory), logger::severity::information);
    get_mem(123);
    

}

alloc_types::~alloc_types(){
    logger* logg = this->_get_logger();
    memory* alc = *reinterpret_cast<memory**>(reinterpret_cast<unsigned char*>(_memory) + sizeof(size_t) + sizeof(void*) + sizeof(logger*));
    this->_log_with_guard(this->_return_memory_condition_info(_memory), logger::severity::information);
    this->_log_with_guard("Allocator was destroyed", logger::severity::warning);
    if(alc != nullptr){
        alc->deallocate(_memory);
    }
    else{
        ::operator delete(_memory);
    }
}

void* alloc_types::get_mem(size_t size){
    std::cout << _get_block_size(_memory) << "\n";
    std::cout << to_str(_get_logger()) << "\n";
    std::cout << to_str(_get_next_block()) << "\n";
    std::cout << to_str(_memory) << "\n";
    return nullptr;
    
}

void* alloc_types::allocate(size_t target_size) const{
    return nullptr;
}

void alloc_types::deallocate(void const * const target_to_dealloc) const {
    void *ptr = const_cast<void*>(target_to_dealloc);
}


// Logger methods
void alloc_types::set_logger(logger* &lg) noexcept{
    *reinterpret_cast<logger**>(reinterpret_cast<unsigned char*>(_memory) + sizeof(size_t) + sizeof(void*)) = lg;
}

logger* alloc_types::_get_logger() const{
    return *reinterpret_cast<logger**>(reinterpret_cast<unsigned char*>(_memory) + sizeof(size_t) + sizeof(void*));
}

std::string alloc_types::_return_memory_condition_info(void* mem_block) const noexcept{
    size_t size = *reinterpret_cast<size_t*>(reinterpret_cast<unsigned char*>(mem_block));
    std::string info = "Memory get this kind of information, like size " + this->to_str(size) + " with such data in it:\n"; 
    for(int i = 0; i < size; i++){
        info += this->to_str(static_cast<int>(*(reinterpret_cast<unsigned char*>(mem_block) + i))) + " ";
    }
    return info;
}

void alloc_types::_log_with_guard(const std::string& str, logger::severity level) const noexcept{
    logger* logg = _get_logger();
    if (logg != nullptr){
        logg->log(str, level);
    }
}

template<typename T>
std::string alloc_types::to_str(T const &object) const noexcept{
    std::stringstream stream;
    stream << object;

    return stream.str();
}
// LOGGER_END


// Service information methods
size_t alloc_types::_get_block_size(void* current) const noexcept{
    return *reinterpret_cast<size_t*>(this->_key_memory(current));
}

void* alloc_types::_get_next_block(void* current) const noexcept{
    return reinterpret_cast<void*>(reinterpret_cast<size_t*>(this->_key_memory(current)) + 1);
}

void* alloc_types::_key_memory(void* try_mem) const noexcept{
    return try_mem == nullptr ? _memory : try_mem;
}
// SERVICE_END

// MEMORY_METHOD BLOCK
void* alloc_types::_get_memory(size_t const &size, void** previous, void** next){
    switch (_method)
    {
    case memory::method::first: return _method_first(size, previous, next);
    
    case memory::method::best: return _method_best(size, previous, next);
    
    case memory::method::worst: return _method_worst(size, previous, next);
    
    default: break;
    }
}

void* alloc_types::_method_first(size_t const &size, void** previous, void** next){
    void* current = _get_next_block();
    while(current != nullptr){
        if (_get_block_size(current) >= size){
            *next = _get_next_block(current);
            return current;
        }
        current = _get_next_block(current);
    }
    return nullptr;
}

void* alloc_types::_method_best(size_t const &size, void** previous, void** next);

void* alloc_types::_method_worst(size_t const &size, void** previous, void** next);
// MEMORY_METHOD END
