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
    using Entry_iterator = typename std::vector<Bucket>::iterator;
    using Bucket_iterator = typename Bucket::iterator;
    using Storage_iterator = typename std::vector<Bucket>::iterator;

private:
    std::vector<Bucket> storage;
    size_t size_of_storage;
    size_t cur_size;
public:
    class Iterator {
    private:

        Entry_iterator cur_entry;
        Bucket_iterator cur_bucket;
        Storage_iterator end_of_storage;

        void SkipEmptyFields(){
            while (cur_entry->begin() == cur_entry->end() && cur_entry != end_of_storage){
                cur_entry++;
            }
        }

    public:
        Iterator(Storage_iterator it_of_storage, Entry_iterator it_end_storage)
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

    size_t getBucketIndex(Key key_) const{
        return (size_t)std::hash<Key>{}(key_) % size_of_storage;
    }

    void resize(){
        size_of_storage *= 2;
        std::vector<Bucket> rehashed_storage(size_of_storage);

        for (auto i: storage){
            for (auto j: i){
                size_t hash_result = getBucketIndex(j.key);
                rehashed_storage[hash_result].push_back({j.key, j.value});
            }
        }

        std::swap(storage, rehashed_storage);
    }

public:
    HashTable(){
        size_of_storage = 20;
        storage.resize(size_of_storage);
        cur_size = 0;
    }

    bool Insert(const Key& key, const Value& value){
        auto& bucket = storage[getBucketIndex(key)];
        auto it = std::find_if(bucket.begin(), bucket.end(), [&](const Entry& entry){ return entry.key == key; });

        if (it != bucket.end()){
            return false;
        }

        bucket.push_back({key,value});
        cur_size++;
        if (cur_size >= size_of_storage){
            resize();
        }

        return true;
    }

    std::pair<bool, Value> Find(const Key& key) const{
        auto& bucket = storage[getBucketIndex(key)];
        auto it = std::find_if(bucket.begin(), bucket.end(), [&](const Entry& entry){ return entry.key == key; });

        std::pair<bool, Value> find_value;

        find_value.first = (it != bucket.end());

        if (find_value.first){
            find_value.second = it->value;
        }

        return find_value;
    }

    bool Remove(const Key& key){
        auto& bucket = storage[getBucketIndex(key)];
        auto it = std::find_if(bucket.begin(), bucket.end(), [&](const Entry& entry){ return entry.key == key; });

        if (it != bucket.end()){
            bucket.erase(it);
            cur_size--;
            return true;
        }

        return false;
    }


    Value& operator[](const Key& key){
        auto& bucket = storage[getBucketIndex(key)];
        auto it = std::find_if(bucket.begin(), bucket.end(), [&](const Entry& entry){ return entry.key == key; });

        if (it == bucket.end()){
            bucket.push_back({key, Value()});
            cur_size++;
            return bucket.back().value;
        }

        return it->value;
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