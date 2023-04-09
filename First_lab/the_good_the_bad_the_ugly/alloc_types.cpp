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
    get_mem(123);
    

}

alloc_types::~alloc_types(){
    logger* logg = this->_get_logger();
    memory* alc = *reinterpret_cast<memory**>(reinterpret_cast<unsigned char*>(_memory) + sizeof(size_t) + sizeof(void*) + sizeof(logger*));
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
    std::cout << _get_logger() << "\n";
    return nullptr;
    
}

void* alloc_types::allocate(size_t target_size) const{
    return nullptr;
}

void alloc_types::deallocate(void const * const target_to_dealloc) const {
    void *ptr = const_cast<void*>(target_to_dealloc);
}
        
void alloc_types::set_logger(logger* &lg) noexcept{
    logger** ptr_logger = reinterpret_cast<logger**>(reinterpret_cast<unsigned char*>(_memory) + sizeof(size_t) + sizeof(void*));
    *ptr_logger = lg;
}

logger* alloc_types::_get_logger() const{
    return *reinterpret_cast<logger**>(reinterpret_cast<unsigned char*>(_memory) + sizeof(size_t) + sizeof(void*));
    
}

size_t alloc_types::_get_block_size(void* current) const noexcept{
    return *reinterpret_cast<size_t*>(current);
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
