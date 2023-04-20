#include "memory.h"

memory::~memory()= default;

void *memory::operator+=(size_t const& size){
    return allocate(size);
}

void memory::operator-=(void const * const object) const{
    deallocate(object);
}