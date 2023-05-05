#include <iostream>
#include "b_tree.h"

int main(){
    auto* bTree = new b_tree<std::string, std::string, std::less<>>(2, nullptr, nullptr);
    bTree->insert(12, std::forward<std::string>("34"));
}