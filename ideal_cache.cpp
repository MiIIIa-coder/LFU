#include <iostream>

class ideal_cache
{
private:
    int size_cache;
    int *cache;
    struct cache_list {
        int data;
        int next_data_node;
        struct cache_list *next;
        struct cache_list *prev;
    };
    cache_list *front;  //first el in list
    cache_list *back;   //last  el in list
public:
    ideal_cache(int size_cache_);
    ~ideal_cache();
    int find_next_data_node (int data_this_node, const int *data, int position, int size_data) const;
    void create_cache_list(const int *data, int size_data);
    void add_node(int *data, int position, int size_data);
    void remove(cache_list *node);
    void remove_last();
    bool define_cache_hit(int *data, int position, int size_data);
    void decrement_cache_data();
};

ideal_cache::ideal_cache(int size_cache_)
{
    size_cache = size_cache_;
    cache = new int[size_cache_];
    front = nullptr;
    back = nullptr;
}

ideal_cache::~ideal_cache()
{
    //std::cout << "bye!" << std::endl; 
}

int ideal_cache::find_next_data_node(int data_this_node, const int *data, int position, int size_data) const {
    int next_data_node = 1;
    for (int i = position + 1; i < size_data; ++i, ++next_data_node) {
        if (data_this_node == *(data + i))
            break;
    }
    return next_data_node;
}

void ideal_cache::create_cache_list(const int *data, int size_data) {
    front = new cache_list[1];
    front->data = *data;
    front->next = nullptr;
    front->prev = nullptr;
    front->next_data_node = find_next_data_node(front->data, data, 0, size_data);
    back = front;
}

void ideal_cache::add_node(int *data, int position, int size_data) {
    cache_list *new_node = new cache_list[1];
    new_node->data = *(data + position);
    new_node->next_data_node = find_next_data_node(new_node->data, data, position, size_data);

    cache_list *node = front;
    while (node->next != nullptr && node->next_data_node < new_node->next_data_node) { 
        node = node->next;
    }
    if (node->next == nullptr) {
        if (node->next_data_node < new_node->next_data_node) {  //new_node becomes last
            new_node->next = nullptr;
            new_node->prev = node;
            node->next = new_node;
            back = new_node;
        } else {
            new_node->next = node;
            new_node->prev = node->prev;
            node->prev->next = new_node;
            node->prev = new_node;
            if (front == node)
                front = new_node;
        }
    } else {
        if (node->prev == nullptr) {
            new_node->prev = nullptr;
            front = new_node;
        } else {
            new_node->prev = node->prev;
            node->prev->next = new_node;
        }
        node->prev = new_node;
        new_node->next = node;
    }

}

void ideal_cache::decrement_cache_data() {
    cache_list *node = front;
    while (node != nullptr) {
        --(node->next_data_node);
        node = node->next;
    }
}

void ideal_cache::remove(cache_list *node) {
    if (node->next == nullptr) {
        node->prev->next = node->next;
        back = node->prev;  
    } else if (node->prev == nullptr) {
        node->next->prev = node->prev;
        front = node->next;
    } else {
        node->next->prev = node->prev;
        node->prev->next = node->next;
    }
    
    node->next = nullptr;
    node->prev = nullptr;

    delete(node);
}

void ideal_cache::remove_last() {
    remove(back);
}

bool ideal_cache::define_cache_hit(int *data, int position, int size_data) {
    if (front == nullptr)
        return false;
    if (*(data + position) == front->data) {
        remove(front);
        add_node(data, position, size_data);
        return true;
    }
    return false;
}

void get_data(int size_data, int** data) {

    int element;

    for (int i = 0; i < size_data; ++i) {
        std::cin >> element;
        *(*data + i) = element;    
    }
}

int main() {

int size_cache = 0;
std::cin >> size_cache;
int size_data = 0;
std::cin >> size_data;

int *data = nullptr;
data = new int[size_data];
get_data(size_data, &data);
//std::cout << data[size_data-1] << std::endl;

ideal_cache i_cache {size_cache};

int cache_hit = 0;
if (size_cache < size_data) {
    int i = 0; 
    int p = 0;
    i_cache.create_cache_list(data, size_data);
    for (i = 1, p = 1; p < size_cache; ++i) {
        i_cache.decrement_cache_data();
        if (i_cache.define_cache_hit(data, i, size_data))
            ++cache_hit;
        else {
            i_cache.add_node(data, i, size_data);
            ++p;
        }
    }

    for (; i < size_data; ++i) {
        i_cache.decrement_cache_data();
        if (i_cache.define_cache_hit(data, i, size_data))
            ++cache_hit;
        else {
            i_cache.remove_last();
            i_cache.add_node(data, i, size_data);
        }
    }

} 

std::cout << "cache_hit = " << cache_hit << std::endl;

return 0;

}