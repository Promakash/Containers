#include <algorithm>
#include <iostream>
#include <vector>
#include <list>
#include <cstddef>
#include "iterator"


template <typename Key, typename Value>
class HashTable {
public:
    struct Entry {
        Key key;
        Value value;
    };

    using Bucket = std::list<Entry>;
    
private:
    std::vector<Bucket> storage;
    size_t size_of_storage;
    size_t cur_size;
public:
    class Iterator {
    private:
        typename std::vector<Bucket>::iterator cur_entry;
        typename Bucket::iterator cur_bucket;
        typename std::vector<Bucket>::iterator end_of_storage;
        void SkipEmptyFields(){
            while (cur_entry->begin() == cur_entry->end() && cur_entry != end_of_storage){
                cur_entry++;
            }
        }
    public:
        Iterator(typename std::vector<Bucket>::iterator it_of_storage, typename std::vector<Bucket>::iterator it_end_storage)
                : cur_entry(it_of_storage),
                  cur_bucket(cur_entry->begin()),
                  end_of_storage(it_end_storage)
        {
            SkipEmptyFields();
            if (cur_entry == end_of_storage){
                return;
            }
            cur_bucket = cur_entry->begin();
        }

        Iterator& operator++(){
            cur_bucket++;
            if (cur_bucket == cur_entry->end()){
                cur_entry++;
                SkipEmptyFields();
                if (cur_entry == end_of_storage){
                    return *this;
                }
                cur_bucket = cur_entry->begin();
            }
            return *this;
        }

        Entry& operator*(){
            return *cur_bucket;
        }

        bool operator!=(const Iterator& rhs){
            return cur_entry != rhs.cur_entry;
        }
    };

private:
    size_t hash_func(Key key_) const{
        return (size_t)std::hash<Key>{}(key_) % size_of_storage;
    }

    void resize(){
        size_of_storage *= 2;
        std::vector<Bucket> rehashed_storage(size_of_storage);
        for (auto i: storage){
            for (auto j: i){
                size_t hash_result = hash_func(j.key);
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
        size_t hash_result = hash_func(key);
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
        size_t hash_result = hash_func(key);
        for (auto it: storage[hash_result]){
            if (it.key == key){
                return {true, it.value};
            }
        }
        return {false, Value()};
    }

    bool Remove(const Key& key){
        uint64_t hash_result = hash_func(key);
        auto& Entries = storage[hash_result];
        for (auto it = Entries.begin(); it != Entries.end(); ++it){
            if (it->key == key){
                Entries.erase(it);
                cur_size--;
                return true;
            }
        }
        return false;
    }


    Value& operator[](const Key& key){
        size_t hash_result = hash_func(key);
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
        return Iterator(storage.begin(), storage.end());
    }

    auto end(){
        return Iterator(storage.end(), storage.end());
    }

};
