#include <iostream>
#include "b_tree.h"
#include "../logger/logger_builder_concrete.h"

template<typename T>
class t_type_holder{
    T parameter;

    public:
        explicit t_type_holder(T parameter){
            this->parameter = parameter;
        }
        virtual ~t_type_holder()=default;
        T& get(){
            return parameter;
        }
        void set(T& arg){
            parameter = arg;
        }
};

template<typename T>
class compare{
private:
    int comparer(T const & a, T const & b) const{
        if(a > b){
            return 1;
        }
        if (a < b){
            return -1;
        }
        return 0;
    }
public:
    int operator ()(T const & a, T const &  b) const{
        if(a > b){
            return 1;
        }
        if (a < b){
            return -1;
        }
        return 0;
    }
};

struct name{
    std::string name;
    std::string surname;
};

struct name_holder{
    name* info;
};


int main(){
//    t_type_holder<int> ex = t_type_holder<int>(10);
//    std::cout << ex.get();
    logger_builder* builder = new logger_builder_concrete();
    logger* loggs = builder->add_stream("logs.log", logger::severity::information)->construct();
    associative_container<int, std::string>* bTree = new b_tree<int, std::string, compare<int>>(2, nullptr, loggs);
    std::string ask = "34", abd = "12", cd = "9";
    int a = 10, b = 12, c = 9;
    compare<std::string> comp;
    bTree->insert(1, "0");
//    for (int i = 0; i < 25; i++) {
//        bTree->insert(i, std::to_string(i));
//    }
//    std::cout << bTree->get(a) << std::endl;
//    dynamic_cast<b_tree<int, std::string, compare<int>>*>(bTree)->traverse_start();
    delete(bTree);
    delete(loggs);
    delete(builder);
//    auto mem_name = reinterpret_cast<name*>(::operator new(sizeof(name)));
//    new (mem_name) name{"apex", "legends"};
//    mem_name->~name();
//    ::operator delete(mem_name);
//    auto mem_hold = reinterpret_cast<name_holder*>(::operator new(sizeof(name_holder)));
//    auto name_m = reinterpret_cast<name*>(::operator new(sizeof(name)));
//    new (mem_hold) name_holder{name_m};
//    ::operator delete(mem_hold->info);
//    ::operator delete(mem_hold);
}