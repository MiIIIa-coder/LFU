#include "cache.hpp"

int slow_get_page_int(int key) { return key; }

int main() 
{
    int hits = 0;
    size_t sz;
    int sz_data;

    std::cin >> sz >> sz_data;

    caches::cache_t<int> cache{sz};

    //cache.create_list();
    //cache.create_hash();

    for (int i = 0; i < sz_data; ++i) {
        int key;
        std::cin >> key;
        if (cache.lookup_update(key, slow_get_page_int))
            ++hits;
    }
    
    return 0;
}