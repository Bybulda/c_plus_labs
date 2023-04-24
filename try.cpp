#include <list>
#include "memory/memory_concrete.h"
#include "the_good_the_bad_the_ugly/alloc_types.h"
#include "descriptor/alloc_types.h"

void testing_allocator()
{
    logger_builder* build = new logger_builder_concrete();


    logger *logger = build
            ->add_stream("logs.txt", logger::severity::trace)
            ->add_stream("console", logger::severity::trace)
            ->construct();

    memory *allocator2 = new alloc_types_list(1000000, nullptr, logger, memory::method::first);
    memory *allocator3 = new alloc_types_descriptor(999900, allocator2, logger, memory::method::best);

    std::list<void*> allocated_blocks;

    srand((unsigned)time(nullptr));

    for (size_t i = 0; i < 20000; ++i)
    {
        void * ptr;

        switch (rand() % 2)
        {
            case 0:
                try
                {
                    ptr = reinterpret_cast<void *>(allocator3->allocate(rand() % 81 + 20)); // разность макс и мин с включенными границами + минимальное
                    allocated_blocks.push_back(ptr);
                }
                catch (std::exception const &ex)
                {
                    std::cout << ex.what() << std::endl;
                }
                break;
            case 1:

                if (allocated_blocks.empty())
                {
                    break;
                }

                try
                {
                    auto iter = allocated_blocks.begin();
                    std::advance(iter, rand() % allocated_blocks.size());
                    allocator3->deallocate(*iter);
                    allocated_blocks.erase(iter);
                }
                catch (std::exception const &ex)
                {
                    std::cout << ex.what() << std::endl;
                }
                break;
        }

        //std::cout << "iter # " << i + 1 << " finish" << std::endl;
    }

    while (!allocated_blocks.empty())
    {
        try
        {
            auto iter = allocated_blocks.begin();
            allocator3->deallocate(*iter);
            allocated_blocks.erase(iter);
        }
        catch (std::exception const &ex)
        {
            std::cout << ex.what() << std::endl;
        }
    }

    delete allocator3;
    delete allocator2;
    delete logger;
    delete build;
}


int main(){
    testing_allocator();
    return 0;
}
