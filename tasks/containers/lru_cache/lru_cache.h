#pragma once

#include <cstdlib>
#include <utility>
#include <string>
#include "list"
#include "unordered_map"

class LRUCache {
private:
    std::list<std::string> queue_;
    size_t cur_size_;
    size_t maxSize_;
    std::unordered_map<std::string, std::pair<std::list<std::string>::const_iterator, std::string>> hash_map;
public:
    LRUCache(size_t maxSize){
        maxSize_ = maxSize;
        cur_size_ = 0;
        hash_map.reserve(maxSize);
    }

    void Insert(const std::string& key, const std::string& value){
        if (cur_size_ >= maxSize_){
            auto it = queue_.begin();
            hash_map.erase(*it);
            queue_.pop_front();
            queue_.push_back(key);
            hash_map[key] = {std::prev(queue_.end()), value};
        }
        else{
            queue_.push_back(key);
            hash_map[key] = {std::prev(queue_.end()), value};
            cur_size_++;
        }
    }

    std::pair<bool, std::string> Find(const std::string& key){
        if (hash_map.find(key) != hash_map.end()){
            auto pair_values_by_key = hash_map[key];
            queue_.erase(pair_values_by_key.first);
            queue_.push_back(key);
            hash_map[key] = {std::prev(queue_.end()), pair_values_by_key.second};
            return {true, pair_values_by_key.second};
        }
        else{
            return {false, ""};
        }
    }
};