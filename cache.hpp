#include <iostream>
#include <list>
#include <unordered_map>
#include <iterator>

namespace caches {

    template <typename T, typename keyT = int> class cache_t {
        private:
            size_t sz_;
            using freq = int;
            struct elt_cache_t {
                T data;
                keyT key;
                int freq;
            };
            
            std::list<elt_cache_t> cache_;
            using ListIt = typename std::list<elt_cache_t>::iterator;
            std::unordered_map<keyT, ListIt> hash_;
        public:
            cache_t(size_t size_cache) : sz_(size_cache) {}

            bool full() const { return (cache_.size() == sz_); }

            template <typename F, typename keyType = int> bool lookup_update(keyType key, F slow_get_page) {
                auto hit = hash_.find(key);

                if (hit == hash_.end()) { //misshit
                    if (full()) {
                        hash_.erase(cache_.front()->key);
                        cache_.pop_front();
                    }
                    cache_.emplace_front(slow_get_page(key), key, 1);
                    hash_.emplace(key, cache_.begin());
                    return false;
                }

                auto eltit = hit->second;  //iterator of hit elt
                //++(std::get<3>(*eltit));
                ++(eltit->freq);
                if (eltit != cache_.back()) {
                    while (eltit->third >= (std::next(eltit)->third)) {
                        cache_.splice(std::next(eltit), cache_, eltit, std::next(eltit));
                    }
                }
                return true;
            }

            /*
            void create_hash() {
                int i = 0;
                auto It = cache_.begin();
                for (i = 30; It != cache_.end(); ++i, ++It) {
                    hash_.emplace(i, It);
                }
            }

            
            void create_list() {
                int i = 0;
                for (i = 0; i < sz_; ++i) {
                    T data;
                    std::cin >> data;
                    cache_.emplace_back(slow_get_page_int(data), data, 1);
                }
            }

            void show_hash() const {
                std::cout << "Hash: " << std::endl;
                for (auto It = hash_.begin(); It != hash_.end(); ++It)
                    std::cout << It->first << std::endl;
                //std::cout << hash_[0] << std::endl;
            }

            void show_cache() const {
                std::cout << "Cache: " << std::endl;
                copy (cache_.begin(), cache_.end(), std::ostream_iterator<int>(std::cout, " ")); 
                std::cout << std::endl;
            };
            */

    }; 

};