#include <iostream>
#include <list>
#include <unordered_map>
#include <iterator>

namespace caches {

    /*LFU cache*/
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

    /*ideal cache*/
    template<typename T, typename KeyT = int> class id_cache {
    private:
        int cache_hits;
        int size_data_;
        size_t sz_cache_;
        KeyT* data_array;
        T (*slow_get_page)(KeyT key);
        struct elt_id_cache
        {
            T data;
            KeyT key;
            int next_data_node;
        };

        std::list<elt_id_cache> id_cache_;
        using listIt = typename std::list<elt_id_cache>::iterator;
        
    public:
        id_cache(int size_data, size_t sz_cache, T (*func)(KeyT key)) : size_data_(size_data), sz_cache_(sz_cache), slow_get_page(func) {

            cache_hits = 0;
            data_array = new KeyT[size_data_];
            for (int i = 0; i < size_data_; ++i) {
                std::cin >> *(data_array + i);
                if(!std::cin) {
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //Выкидываем все что ввел пользователь до конца строки
                    std::cin.clear();  //Убираем флаг ошибки. Теперь состояние потока снова good
                    std::cout << "ERROR!" << std::endl;
                    break;
                }
            } 
        }
        ~id_cache() {
            delete [] data_array;
        }

        bool full() const { return (id_cache_.size() == sz_cache_); }

        KeyT get_key(int position) const { return *(data_array + position); }

        int find_next_data_node(int position, KeyT key) const {
            int next_data_node = 1;

            for (int i = position + 1; i < size_data_; ++i, ++next_data_node) {
                if (key == *(data_array + i))
                    break;
            } 

            return next_data_node;
        }

        /*decrement next_data_node in each elt in cache*/
        void update_cache_info() {   
            for (listIt It = id_cache_.begin(); It != id_cache_.end(); ++It) {
                --(It->next_data_node);
            }
        }

        /*remove first elt and add new elt in right position*/
        void update_cache(int position) {                       
            elt_id_cache elt {slow_get_page(*(data_array + position)), 
            *(data_array + position), find_next_data_node(position, *(data_array + position))};
            id_cache_.emplace_front(elt);
            //id_cache_.sort([](const elt_id_cache& first, const elt_id_cache& second)  
            //{return first.next_data_node >= second.next_data_node;});
            listIt new_elt = id_cache_.begin();
            listIt next_elt = std::next(new_elt); //for debugging
            while (new_elt->next_data_node <= std::next(new_elt)->next_data_node && new_elt != std::prev(id_cache_.end())) {
                id_cache_.splice(std::next(new_elt, 2), id_cache_, new_elt, std::next(new_elt));
            }     
        }

        int count_hits() {
            elt_id_cache elt {slow_get_page(*data_array), 
            *data_array, find_next_data_node(0, *data_array)}; 
            id_cache_.emplace_front(elt);   //add first elt in list

            for (int i = 1; i < size_data_; ++i) {
                update_cache_info();
                if (*(data_array + i) == std::prev(id_cache_.end())->key) {  //cache hit
                    ++cache_hits;
                    id_cache_.pop_back();
                    update_cache(i);
                } else {                                            //misshit
                    if (full()) {
                        id_cache_.pop_front();
                        update_cache(i);
                    } else {
                        update_cache(i);
                    }
                }
            }

            return cache_hits;
        }
    };
     

};
