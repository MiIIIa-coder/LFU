#include "cache.hpp"

int slow_get_page_int(int key) { return key; }

int main() 
{
    int hits = 0;
    size_t sz;
    int sz_data;

    std::cin >> sz >> sz_data;

    caches::id_cache<int> ideal_cache{sz_data, sz, slow_get_page_int};
    std::cout << "Hits of ideal cache: " << ideal_cache.count_hits() << std::endl;

    caches::cache_t<int> cache{sz, slow_get_page_int};

    for (int i = 0; i < sz_data; ++i) {
        int key;
        //std::cin >> key;

        key = ideal_cache.get_key(i);
        if (cache.lookup_update(key))
            ++hits;
    }

    cache.show_cache();

    std::cout << "Hits of LFU: " << hits << std::endl;
    
    return 0;
}