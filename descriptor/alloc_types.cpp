#include "alloc_types.h"
#include <iostream>
#include <sstream>



alloc_types::alloc_types(size_t const& size, memory* alloc, logger* logg, memory::method mode){
    
    if (alloc == nullptr){
        _memory = ::operator new(size + sizeof(size_t) + sizeof(void*) + sizeof(memory*) + sizeof(logger*));
    }
    else{
        _memory = alloc->allocate(size);
    }
    _method = mode;
    
    auto* with_size = reinterpret_cast<size_t*>(_memory);
    *with_size = size;

    void** ptr_next = reinterpret_cast<void**>(with_size + 1);
    *ptr_next = nullptr;

    auto** ptr_logger = reinterpret_cast<logger**>(reinterpret_cast<unsigned char*>(_memory) + sizeof(size_t) + sizeof(void*));
    *ptr_logger = logg;

    auto** mem = reinterpret_cast<memory**>(ptr_logger + 1);
    *mem = alloc;
    this->_log_with_guard("Allocator was created with size (" + this->to_str(size) + ") bytes", logger::severity::information);
}

alloc_types::~alloc_types(){
    memory* alc = this->_get_allocator();
//    this->_log_with_guard(this->_return_memory_condition_info(_memory), logger::severity::information);
    this->_log_with_guard("Allocator was destroyed", logger::severity::warning);
    if(alc != nullptr){
        alc->deallocate(_memory);
    }
    else{
        ::operator delete(_memory);
    }
}


// MEMORY CALLABLE
memory* alloc_types::_get_allocator() const noexcept{
    return *reinterpret_cast<memory**>(reinterpret_cast<unsigned char*>(_memory) + sizeof(size_t) + sizeof(void*) + sizeof(logger*));
}

void* alloc_types::allocate(size_t target_size){
    memory* alc = this->_get_allocator();
    void* block = alc == nullptr ? this->_get_memory(target_size) : alc->allocate(target_size);
    if (block == nullptr){
        this->_log_with_guard("Block of size " + this->to_str(target_size) +" wasn't allocated", logger::severity::critical);
    }
    else{
        this->_log_with_guard("Block of size " + this->to_str(target_size) + " was allocated on block " + this->to_str(block), logger::severity::critical);
    }
    return block;
}

void alloc_types::deallocate(void const * target_to_dealloc) const {
    if (target_to_dealloc == nullptr){
        return;
    }
    memory* alc = this->_get_allocator();

    if (alc == nullptr){
        void* our_block = reinterpret_cast<unsigned char*>(const_cast<void*>(target_to_dealloc)) - sizeof(size_t) - (sizeof(void*) << 1);
        auto* previous = *reinterpret_cast<void**>(reinterpret_cast<unsigned char*>(our_block) + sizeof(size_t) + sizeof(void*));
        auto* next = *reinterpret_cast<void**>(reinterpret_cast<unsigned char*>(our_block) + sizeof(size_t));
        if (previous != nullptr){
            *reinterpret_cast<void**>(reinterpret_cast<unsigned char*>(previous) + sizeof(size_t)) = next;

        }
        if (next != nullptr){
            *reinterpret_cast<void**>(reinterpret_cast<unsigned char*>(next) + sizeof(size_t) + sizeof(void*)) = previous;
        }
        else{
            *reinterpret_cast<void**>(reinterpret_cast<unsigned char*>(_memory) + sizeof(size_t)) = next;
        }
    }
    else{
        alc->deallocate(target_to_dealloc);
    }
    this->_log_with_guard("Block " + this->to_str(target_to_dealloc) + " was deallocated", logger::severity::information);
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
    return *reinterpret_cast<size_t*>(this->_key_memory(current));
}

void* alloc_types::_get_next_block(void* current) const noexcept{
    return *reinterpret_cast<void**>(reinterpret_cast<size_t*>(this->_key_memory(current)) + 1);
}

void* alloc_types::_key_memory(void* try_mem) const noexcept{
    return try_mem == nullptr ? _memory : try_mem;
}
// SERVICE_END

// MEMORY_METHOD BLOCK
void* alloc_types::_get_memory(size_t const &size){
    switch (_method)
    {
        case memory::method::first: return _method_first(size);

        case memory::method::best: return _method_best(size);

        case memory::method::worst: return _method_worst(size);

        default: break;
    }
    return nullptr;
}

void* alloc_types::_method_first(size_t const &size){
   void* curr = this->_get_next_block(), *block = nullptr;
   if(curr == nullptr){
       if (size + sizeof(size_t) + (sizeof(void*) << 1) <= this->_get_block_size()){
           block = reinterpret_cast<void*>(reinterpret_cast<unsigned char*>(_memory) + sizeof(size_t) + sizeof(void*) + sizeof(logger*) + sizeof(memory*));
           auto* now = reinterpret_cast<size_t*>(block);
           *now = size;

           *reinterpret_cast<void**>(now + 1) = nullptr;
           *reinterpret_cast<void**>(reinterpret_cast<unsigned char*>(now + 1) + sizeof(void*)) = nullptr;
           *reinterpret_cast<void**>(reinterpret_cast<unsigned char*>(_memory) + sizeof(size_t)) = block;
           return reinterpret_cast<unsigned char*>(block) + sizeof(size_t) + (sizeof(void*) << 1);
       }
   }
   else{
       while(curr != nullptr){
           void* nxt = this->_get_next_block(curr);
           auto * left = reinterpret_cast<unsigned char*>(curr) + sizeof(size_t) + (sizeof(void*) << 1) + this->_get_block_size(curr);
           if (nxt == nullptr){
               auto * right = reinterpret_cast<unsigned char*>(_memory) + sizeof(logger*) + sizeof(void*) + sizeof(size_t) + sizeof(memory*) + this->_get_block_size();
               size_t dist = right - left;
               if (size + sizeof(size_t) + (sizeof(void*) << 1) <= dist){
                   block = left;
                   *reinterpret_cast<size_t*>(block) = size;
                   *reinterpret_cast<void**>(reinterpret_cast<unsigned char*>(block) + sizeof(size_t)) = nullptr;
                   *reinterpret_cast<void**>(reinterpret_cast<unsigned char*>(block) + sizeof(size_t) + sizeof(void*)) = curr;
                   *reinterpret_cast<void**>(reinterpret_cast<unsigned char*>(curr) + sizeof(size_t)) = block;
                   return reinterpret_cast<unsigned char*>(block) + sizeof(size_t) + (sizeof(void*) << 1);
               }
           }
           else{
               auto* right = reinterpret_cast<unsigned char*>(this->_get_next_block(curr));
               size_t dist = right - left;
               if(size + sizeof(size_t) + (sizeof(void*) << 1) <= dist){
                   block = left;
                   *reinterpret_cast<size_t*>(block) = size;
                   *reinterpret_cast<void**>(reinterpret_cast<unsigned char*>(curr) + sizeof(size_t)) = block;
                   *reinterpret_cast<void**>(reinterpret_cast<unsigned char*>(block) + sizeof(size_t) + sizeof(void*)) = curr;
                   *reinterpret_cast<void**>(reinterpret_cast<unsigned char*>(block) + sizeof(size_t)) = nxt;
                   *reinterpret_cast<void**>(reinterpret_cast<unsigned char*>(nxt) + sizeof(size_t) + sizeof(void*)) = block;

                   return reinterpret_cast<unsigned char*>(block) + sizeof(size_t) + (sizeof(void*) << 1);
               }
           }
           curr = nxt;
       }
   }
   return nullptr;
}

void* alloc_types::_method_best(size_t const &size){
    void* curr = this->_get_next_block(), *block = nullptr, *previous = nullptr, *next_one = nullptr;
    if(curr == nullptr){
        if (size + sizeof(size_t) + (sizeof(void*) << 1) <= this->_get_block_size()){
            block = *reinterpret_cast<void**>(reinterpret_cast<unsigned char*>(_memory) + sizeof(size_t) + sizeof(void*) + sizeof(logger*) + sizeof(memory*));
            auto* now = reinterpret_cast<size_t*>(block);
            *now = size;

            *reinterpret_cast<void**>(now + 1) = nullptr;
            *reinterpret_cast<void**>(reinterpret_cast<unsigned char*>(now + 1) + sizeof(void*)) = nullptr;
            *reinterpret_cast<void**>(reinterpret_cast<unsigned char*>(_memory) + sizeof(size_t)) = block;
            return reinterpret_cast<unsigned char*>(block) + sizeof(size_t) + (sizeof(void*) << 1);
        }
    }else{
        size_t best_size = INT64_MAX;
        while(curr != nullptr){
            void* next = this->_get_next_block(curr);
            auto * left = reinterpret_cast<unsigned char*>(curr) + sizeof(size_t) + (sizeof(void*) << 1) + this->_get_block_size(curr);
            auto * right = next == nullptr ? reinterpret_cast<unsigned char*>(_memory) + FIRST_AFTER_SERVICE_ + this->_get_block_size() : reinterpret_cast<unsigned char*>(next);
            size_t dist = right - left;
            if(size + sizeof(size_t) + (sizeof(void*) << 1) <= size && dist < best_size){
                best_size = dist;
                block = left;
                previous = curr;
                next_one = next;
            }
            curr = next;
        }
        if (block != nullptr){
            *reinterpret_cast<size_t*>(block) = size;
            *reinterpret_cast<void**>(reinterpret_cast<unsigned char*>(block) + sizeof(size_t)) = next_one;
            *reinterpret_cast<void**>(reinterpret_cast<unsigned char*>(block) + sizeof(size_t) + sizeof(void*)) = previous;
            *reinterpret_cast<void**>(reinterpret_cast<unsigned char*>(previous) + sizeof(size_t)) = block;
            if (next_one != nullptr){
                *reinterpret_cast<void**>(reinterpret_cast<unsigned char*>(next_one) + sizeof(size_t) + sizeof(void*)) = block;
            }
            return reinterpret_cast<unsigned char*>(block) + sizeof(size_t) + (sizeof(void*) << 1);
        }
    }
    return nullptr;
}

void* alloc_types::_method_worst(size_t const &size){
    void* curr = this->_get_next_block(), *block = nullptr, *previous = nullptr, *next_one = nullptr;
    if(curr == nullptr){
        if (size + sizeof(size_t) + (sizeof(void*) << 1) <= this->_get_block_size()){
            block = *reinterpret_cast<void**>(reinterpret_cast<unsigned char*>(_memory) + sizeof(size_t) + sizeof(void*) + sizeof(logger*) + sizeof(memory*));
            auto* now = reinterpret_cast<size_t*>(block);
            *now = size;

            *reinterpret_cast<void**>(now + 1) = nullptr;
            *reinterpret_cast<void**>(reinterpret_cast<unsigned char*>(now + 1) + sizeof(void*)) = nullptr;
            *reinterpret_cast<void**>(reinterpret_cast<unsigned char*>(_memory) + sizeof(size_t)) = block;
            return reinterpret_cast<unsigned char*>(block) + sizeof(size_t) + (sizeof(void*) << 1);
        }
    }else{
        size_t best_size = 0;
        while(curr != nullptr){
            void* next = this->_get_next_block(curr);
            auto * left = reinterpret_cast<unsigned char*>(curr) + sizeof(size_t) + (sizeof(void*) << 1) + this->_get_block_size(curr);
            auto * right = next == nullptr ? reinterpret_cast<unsigned char*>(_memory) + FIRST_AFTER_SERVICE_ + this->_get_block_size() : reinterpret_cast<unsigned char*>(next);
            size_t dist = right - left;
            if(size + sizeof(size_t) + (sizeof(void*) << 1) <= size && dist > best_size){
                best_size = dist;
                block = left;
                previous = curr;
                next_one = next;
            }
            curr = next;
        }
        if (block != nullptr){
            *reinterpret_cast<size_t*>(block) = size;
            *reinterpret_cast<void**>(reinterpret_cast<unsigned char*>(block) + sizeof(size_t)) = next_one;
            *reinterpret_cast<void**>(reinterpret_cast<unsigned char*>(block) + sizeof(size_t) + sizeof(void*)) = previous;
            *reinterpret_cast<void**>(reinterpret_cast<unsigned char*>(previous) + sizeof(size_t)) = block;
            if (next_one != nullptr){
                *reinterpret_cast<void**>(reinterpret_cast<unsigned char*>(next_one) + sizeof(size_t) + sizeof(void*)) = block;
            }
            return reinterpret_cast<unsigned char*>(block) + sizeof(size_t) + (sizeof(void*) << 1);
        }
    }
    return nullptr;
}
// MEMORY_METHOD END