#include <iostream>
#include "alloc_types.h"

int main()
{
    logger_builder* build = new logger_builder_concrete();
    logger* logg = build->add_stream("logs.log", logger::severity::trace)->construct();
    logger* &lg = logg;
    memory* all = new alloc_types(1000, nullptr, lg);
    all->set_logger(lg);
    // all->get_mem(123);
    delete all;
    logg->log("Logger is destroyed", logger::severity::information);
    delete logg;
    delete build;
    return 0;
}