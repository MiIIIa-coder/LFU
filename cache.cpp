#include "cache.hpp"

int slow_get_page_int(int key) { return key; }

int main() 
{
    int hits = 0;
    size_t sz;
    int sz_data;

    std::cin >> sz >> sz_data;

    caches::cache_t<int> cache{sz, slow_get_page_int};

    for (int i = 0; i < sz_data; ++i) {
        int key;
        std::cin >> key;
        if(!std::cin){
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //Выкидываем все что ввел пользователь до конца строки
            std::cin.clear();  //Убираем флаг ошибки. Теперь состояние потока снова good
            std::cout << "ERROR!" << std::endl;
            continue;
        }
        if (cache.lookup_update(key))
            ++hits;
    }

    cache.show_cache();
    
    return 0;
}