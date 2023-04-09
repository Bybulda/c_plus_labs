#include <iostream>
#include "alloc_types.h"

int main()
{
    logger_builder* build = new logger_builder_concrete();
    logger* logg = build->add_stream("file228.txt", logger::severity::trace)->construct();
    logger* &lg = logg;
    memory* all = new alloc_types(1000);
    all->set_logger(lg);
    // all->get_mem(123);
    delete all;
    delete logg;
    delete build;
    return 0;
}