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
    
    auto* with_size = reinterpret_cast<size_t*>(_memory);
    *with_size = size;

    void** ptr_next = reinterpret_cast<void**>(with_size + 1);
    *ptr_next = reinterpret_cast<void*>(reinterpret_cast<unsigned char*>(_memory) + sizeof(void*) + sizeof(size_t) + sizeof(logger*) + sizeof(memory*));

    auto** ptr_logger = reinterpret_cast<logger**>(reinterpret_cast<unsigned char*>(_memory) + sizeof(size_t) + sizeof(void*));
    *ptr_logger = logg;

    auto** mem = reinterpret_cast<memory**>(ptr_logger + 1);
    *mem = alloc;

    auto* free_block = reinterpret_cast<size_t*>(*ptr_next);
    *free_block = size - sizeof(size_t) - sizeof(void*) - sizeof(logger*) - sizeof(memory*) - sizeof(size_t) - sizeof(void*);
    this->_log_with_guard("Current free block have such size " + this->to_str(*free_block), logger::severity::information);

    *reinterpret_cast<void**>(free_block + 1) = nullptr;
    this->_log_with_guard("Allocator was created with size " + to_str(size) + " on block " + to_str(_memory), logger::severity::information);
    this->_log_with_guard("Size of mem " + to_str(*with_size) + " next block " + to_str(*ptr_next) + 
    " logger ptr " + to_str(ptr_logger) + " mem allocator " + to_str(*mem) + "\n first block len " + to_str(*free_block), logger::severity::information);
    this->_log_with_guard("Ptr of next mem reffering to _memory block " + 
    to_str(reinterpret_cast<unsigned char*>(*ptr_next) - reinterpret_cast<unsigned char*>(_memory)), logger::severity::critical);
    this->_log_with_guard("Size_t size " + this->to_str(sizeof(size_t)) + " void* size " + this->to_str(sizeof(void*)) + 
    " allocator size " + this->to_str(sizeof(memory*)) + " logger size " + this->to_str(sizeof(logger*)), logger::severity::information);
    

}

alloc_types::~alloc_types(){
    memory* alc = *reinterpret_cast<memory**>(reinterpret_cast<unsigned char*>(_memory) + sizeof(size_t) + sizeof(void*) + sizeof(logger*));
//    this->_log_with_guard(this->_return_memory_condition_info(_memory), logger::severity::information);
    this->_log_with_guard("Allocator was destroyed", logger::severity::warning);
    void* curr = this->_get_next_block();
    while(curr != nullptr){
        this->_log_with_guard("Current block has such size " + this->to_str(this->_get_block_size(curr)) + " and such ptr " + this->to_str(curr), logger::severity::information);
        curr = this->_get_next_block(curr);
    }
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


// MEMORY CALLABLE
void* alloc_types::allocate(size_t target_size){
    void* previous = nullptr;
    void* next = nullptr;
    size_t size = 0, another = 0;
    void* our_block = this->_get_memory(target_size, &previous, &next);
    if (our_block != nullptr){
        size = _get_block_size(our_block);
        if (previous == nullptr){
            if (size - target_size - sizeof(void*) - sizeof(size_t) < sizeof(void*) + sizeof(size_t)){
                *reinterpret_cast<void**>(reinterpret_cast<size_t*>(_memory) + 1) = next;
            }
            else{
                *reinterpret_cast<void**>(reinterpret_cast<size_t*>(_memory) + 1) = reinterpret_cast<void*>(reinterpret_cast<unsigned char*>(our_block) + target_size + sizeof(size_t) + sizeof(void*));
                *reinterpret_cast<size_t*>(reinterpret_cast<unsigned char*>(our_block) + target_size + sizeof(size_t) + sizeof(void*)) = size - target_size - sizeof(size_t) - sizeof(void*);
                *reinterpret_cast<void**>(reinterpret_cast<unsigned char*>(our_block)  + target_size + sizeof(size_t) + sizeof(void*) + sizeof(size_t)) = next;
                *reinterpret_cast<size_t*>(our_block) = target_size;
            }
        }
        else{
            if (size - target_size - sizeof(void*) - sizeof(size_t) < sizeof(void*) + sizeof(size_t)){
                *reinterpret_cast<void**>(reinterpret_cast<size_t*>(previous) + 1) = next;
            }
            else{
                *reinterpret_cast<void**>(reinterpret_cast<size_t*>(previous) + 1) = reinterpret_cast<void*>(reinterpret_cast<unsigned char*>(our_block) + target_size + sizeof(size_t) + sizeof(void*));
                *reinterpret_cast<size_t*>(reinterpret_cast<unsigned char*>(our_block) + target_size + sizeof(size_t) + sizeof(void*)) = size - target_size - sizeof(size_t) - sizeof(void*);
                *reinterpret_cast<void**>(reinterpret_cast<unsigned char*>(our_block) + target_size + sizeof(size_t) + sizeof(void*) + sizeof(size_t)) = next;
                *reinterpret_cast<size_t*>(our_block) = target_size;

            }
        }
    }
    else{
        throw std::runtime_error("NO MEMORY");
    }
    void* first = this->_get_next_block();
    while(first != nullptr){
        this->_log_with_guard("Current block has such size " + this->to_str(this->_get_block_size(first)) + " and such ptr " + this->to_str(first), logger::severity::information);
        first = this->_get_next_block(first);
    }
    return reinterpret_cast<void*>(reinterpret_cast<unsigned char*>(our_block) + sizeof(size_t) + sizeof(void*));
}

void alloc_types::_pull_together(){
    return;
}


void alloc_types::deallocate(void const * const target_to_dealloc) const {
    void *ptr = const_cast<void*>(target_to_dealloc);
    this->_log_with_guard(this->_return_memory_condition_info(ptr), logger::severity::critical);
    void *curr = this->_get_next_block();
    void* left = nullptr, *right = nullptr, *info = nullptr;
    while(curr != nullptr){
        if(reinterpret_cast<unsigned char*>(curr) + sizeof(size_t) + sizeof(void*) < ptr){
            left = curr;
        }
        else{
            right = curr;
            break;
        }
        curr = this->_get_next_block(curr);
    }
    void* returnment = reinterpret_cast<unsigned char*>(ptr) - sizeof(void*) - sizeof(size_t);
    if (right != nullptr || left != nullptr) {
        if (left != nullptr) {
            if ((reinterpret_cast<unsigned char *>(left) + sizeof(size_t) + sizeof(void *) +
                 this->_get_block_size(left)) == returnment) {
                *reinterpret_cast<size_t *>(left) = this->_get_block_size(left) + this->_get_block_size(returnment) + sizeof(void*) + sizeof(size_t);
                *reinterpret_cast<void **>(reinterpret_cast<unsigned char *>(left) + sizeof(size_t)) = right;
                info = left;
                returnment = left;
                this->_log_with_guard("Block was united with the left one", logger::severity::information);
            } else {
                *reinterpret_cast<void **>(reinterpret_cast<unsigned char *>(left) + sizeof(size_t)) = returnment;
                info = returnment;
                this->_log_with_guard("Block was linked with the left one", logger::severity::information);
            }
        }
        if (right != nullptr) {
            if ((reinterpret_cast<unsigned char *>(returnment) + sizeof(size_t) + sizeof(void *) +
                 this->_get_block_size(returnment)) == right) {
                *reinterpret_cast<size_t *>(returnment) =
                        this->_get_block_size(returnment) + this->_get_block_size(right) + sizeof(void*) + sizeof(size_t);
                *reinterpret_cast<void **>(reinterpret_cast<unsigned char *>(returnment) + sizeof(size_t)) = this->_get_next_block(right);
                info = returnment;
                this->_log_with_guard("Block was united with the right one", logger::severity::information);
            }
            else{
                *reinterpret_cast<void **>(reinterpret_cast<unsigned char *>(returnment) + sizeof(size_t)) = right;
                info = returnment;
                this->_log_with_guard("Block was linked with the right one", logger::severity::information);
            }
            *reinterpret_cast<void**>(reinterpret_cast<unsigned char*>(_memory) + sizeof(size_t)) = returnment;
        }
    }
    void* first = this->_get_next_block();
    while(first != nullptr){
        this->_log_with_guard("Current block has such size " + this->to_str(this->_get_block_size(first)) + " and such ptr " + this->to_str(first), logger::severity::information);
        first = this->_get_next_block(first);
    }
    this->_log_with_guard("Block was deallocated, it had such adress " + this->to_str(ptr) + " now it have such adress " + this->to_str(info), logger::severity::information);
}
// MEMORY CALLABLE END

// Logger methods
void alloc_types::set_logger(logger* &lg) noexcept{
    *reinterpret_cast<logger**>(reinterpret_cast<unsigned char*>(_memory) + sizeof(size_t) + sizeof(void*)) = lg;
}

logger* alloc_types::_get_logger() const{
    return *reinterpret_cast<logger**>(reinterpret_cast<unsigned char*>(_memory) + sizeof(size_t) + sizeof(void*));
}

std::string alloc_types::_return_memory_condition_info(void* mem_block) const noexcept{
    size_t size = *reinterpret_cast<size_t*>(reinterpret_cast<unsigned char*>(reinterpret_cast<unsigned char *>(mem_block) - sizeof(size_t) - sizeof(void*)));
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
    stream << std::hex;

    return stream.str();
}
// LOGGER_END


// Service information methods
size_t alloc_types::_get_block_size(void* current) const noexcept{
    return current == nullptr ? 0 : *reinterpret_cast<size_t*>(this->_key_memory(current));
}

void* alloc_types::_get_next_block(void* current) const noexcept{
    return *reinterpret_cast<void**>(reinterpret_cast<size_t*>(this->_key_memory(current)) + 1);
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
    return nullptr;
}

void* alloc_types::_method_first(size_t const &size, void** previous, void** next){
    void* current = this->_get_next_block();
    *previous = *next = nullptr;
    while(current != nullptr){
        size_t sis= this->_get_block_size(current);
        if (sis >= size){
            *next = _get_next_block(current);
            return current;
        }
        *previous = current;
        current = this->_get_next_block(current);
        *next = this->_get_next_block(current);
    }
    return nullptr;
}

void* alloc_types::_method_best(size_t const &size, void** previous, void** next){
    void* current = this->_get_next_block();
    void* best_block = current;
    *previous = *next = nullptr;
    size_t min_divide = INT64_MAX, curr_block_size = 0;
    while(current != nullptr){
        curr_block_size = this->_get_block_size(current);
        if (curr_block_size == size)
        {
            *next = this->_get_next_block(current);
            return current;
        }
        else if (curr_block_size >= size && ((curr_block_size - size) < min_divide))
        {
            min_divide = curr_block_size - size;
            best_block = current;
        }
        if (this->_get_next_block(current) != nullptr){
            *previous = current;
        }
        *next = this->_get_next_block(current);
        current = _get_next_block(current);

    }
    return best_block;
}

void* alloc_types::_method_worst(size_t const &size, void** previous, void** next){
    void* current = this->_get_next_block();
    void* worst_block = nullptr;
    *previous = *next = nullptr;
    while(current != nullptr){
        if (this->_get_block_size(current) >= size && (this->_get_block_size(current) >= this->_get_block_size(worst_block))){
            worst_block = current;
        }
        if (this->_get_next_block(current) != nullptr){
            *previous = current;
        }
        *next = this->_get_next_block(current);
        current = this->_get_next_block(current);

    }
    return worst_block;
}
// MEMORY_METHOD END
