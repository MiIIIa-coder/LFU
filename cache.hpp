#include <iostream>
#include <list>
#include <unordered_map>
#include <iterator>

namespace caches {

    template <typename T, typename keyT = int> class cache_t {
        private:
            size_t sz_;
            T (*slow_get_page)(keyT key);
            struct elt_cache_t {
                T data;
                keyT key_elt;
                int freq;
            };
            std::list<elt_cache_t> cache_;
            using ListIt = typename std::list<elt_cache_t>::iterator;
            std::unordered_map<keyT, ListIt> hash_;
        public:
            cache_t(size_t size_cache, T (*func)(keyT key)) : sz_(size_cache), slow_get_page(func) {}

            bool full() const { return (cache_.size() == sz_); }

            bool lookup_update(keyT key) {
                auto hit = hash_.find(key);

                if (hit == hash_.end()) { //misshit
                    if (full()) {
                        hash_.erase(cache_.begin()->key_elt);
                        cache_.pop_front();
                    }
                    elt_cache_t elt {slow_get_page(key), key, 1};
                    cache_.emplace_front(elt);
                    hash_.emplace(key, cache_.begin());
                    return false;
                }

                auto eltit = hit->second;  //iterator of hit elt
                (eltit->freq)++;
                if (eltit != std::prev(cache_.end())) {
                    while (eltit->freq >= (std::next(eltit))->freq) {
                        //std::iter_swap(eltit, std::next(eltit));
                        cache_.splice(std::next(eltit, 2), cache_, eltit, std::next(eltit));
                    }
                }
                return true;
            }

            void show_cache() const {
                std::cout << "Cache: " << std::endl;
                for (auto it = cache_.begin(); it != cache_.end(); it++)
                    std::cout << it->key_elt << " " << it->freq << std::endl; 
                std::cout << std::endl;
            }

    }; 

};
