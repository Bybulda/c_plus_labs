#include <iostream>
#include "alloc_types.h"

int main()
{
    logger_builder* build = new logger_builder_concrete();
    logger* logg = build->add_stream("logs.log", logger::severity::trace)->construct();
    logger* &lg = logg;
    memory* all = new alloc_types(1000, nullptr, lg, memory::method::best);
    int* array = reinterpret_cast<int*>(all->allocate(sizeof(int) * 8));
    char* mass = reinterpret_cast<char *>(all->allocate(sizeof(char) * 100));
    all->deallocate(array);
    int* ar = reinterpret_cast<int*>(all->allocate(sizeof(int) * 100));

    all->deallocate(mass);
    all->deallocate(ar);
    delete all;
    logg->log("Logger is destroyed", logger::severity::information);
    delete logg;
    delete build;
    return 0;
}