#include <iostream>
#include <typeinfo>
#include <typeindex>

class InfoA{

};
class InfoB{
    
};


int main(){


    InfoA a;
    InfoB b;
    auto it=std::type_index(typeid(a));
    std::cout<<it.name()<<std::endl;
    std::cout<<typeid(b).name()<<std::endl;


    return 0;
}


