#include <algorithm>
#include <iostream>
#include <vector>
#include <list>
#include <cstddef>
#include "iterator"


//Извини, что тут дико насрано - хотел почистить и довести до ума, но боюсь если чихну оно все сломается (
template <typename Key, typename Value>
class HashTable {
public:
    struct Entry {
        Key key;
        Value value;
    };
private:
    std::vector<std::list<Entry>> storage;
    size_t size_of_storage;
    size_t cur_size;
public:
    class Iterator {
        //Почему требует компилятор требует typename итераторов?
        //need 'typename' before 'std::__cxx11::list<HashTable<Key, Value>::Entry>::iterator' because 'std::__cxx11::list<HashTable<Key, Value>::Entry>' is a dependent scope
        //Не понял о каком зависимом поле видимости идет речь из этой ошибки
        typename std::vector<std::list<Entry>>::iterator storage_it;
        typename std::list<Entry>::iterator dbl_list_it;
        HashTable<Key, Value>& ht;
    public:
        Iterator(HashTable<Key, Value>& hashTable, typename std::vector<std::list<Entry>>::iterator it_stor)
                : storage_it(it_stor),
                  dbl_list_it(storage_it->begin()), ht(hashTable){
            while (storage_it->begin() == storage_it->end() && storage_it != ht.storage.end()){
                storage_it++;
            }
            if (storage_it == ht.storage.end()){
                return;
            }
            dbl_list_it = storage_it->begin();
        }

        Iterator& operator++(){
            dbl_list_it++;
            if (dbl_list_it == storage_it->end()){
                storage_it++;
                while (storage_it->begin() == storage_it->end() && storage_it != ht.storage.end()){
                    storage_it++;
                }
                if (storage_it == ht.storage.end()){
                    return *this;
                }
                dbl_list_it = storage_it->begin();
            }
            return *this;
        }

        Entry& operator*(){
            return *dbl_list_it;
        }

        bool operator!=(const Iterator& rhs){
            return storage_it != rhs.storage_it;
        }
    };

private:
    size_t hash_func(Key key_) const{
        return (size_t)std::hash<Key>{}(key_);
    }
    void resize(){
        size_of_storage *= 2;
        std::vector<std::list<Entry>> rehashed_storage(size_of_storage);
        for (auto i: storage){
            for (auto j: i){
                size_t hash_result = hash_func(j.key) % size_of_storage;
                rehashed_storage[hash_result].push_back({j.key, j.value});
            }
        }
        std::swap(storage, rehashed_storage);
    }
public:
    HashTable(){
        storage.resize(40);
        size_of_storage = 40;
        cur_size = 0;
    }
    bool Insert(const Key& key, const Value& value){
        size_t hash_result = hash_func(key) % size_of_storage;
        if (storage[hash_result].begin() != storage[hash_result].end()){
            for (auto it: storage[hash_result]){
                if (it.key == key){
                    return false;
                }
            }
        }
        storage[hash_result].push_back({key,value});
        cur_size++;
        if (cur_size >= size_of_storage){
            resize();
        }
        return true;
    }

    std::pair<bool, Value> Find(const Key& key) const{
        size_t hash_result = hash_func(key) % size_of_storage;
        for (auto it: storage[hash_result]){
            if (it.key == key){
                return {true, it.value};
            }
        }
        return {false, Value()};
    }

    bool Remove(const Key& key){
        uint64_t hash_result = hash_func(key) % size_of_storage;
        auto& dbl_list = storage[hash_result];
        for (auto it = dbl_list.begin(); it != dbl_list.end(); ++it){
            if (it->key == key){
                dbl_list.erase(it);
                cur_size--;
                return true;
            }
        }
        return false;
    }


    Value& operator[](const Key& key){
        size_t hash_result = hash_func(key) % size_of_storage;
        for (auto& it: storage[hash_result]){
            if (it.key == key){
                return it.value;
            }
        }
        storage[hash_result].push_back({key, Value()});
        cur_size++;
        return storage[hash_result].back().value;
    }

    size_t Size() const{
        return cur_size;
    }

    auto begin(){
        return Iterator(*this, storage.begin());
    }

    auto end(){
        return Iterator(*this, storage.end());
    }

};
