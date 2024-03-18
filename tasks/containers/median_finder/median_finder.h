#pragma once

#include "set"

class MedianFinder {
private:
    std::multiset<int> ms;
    std::multiset<int>::iterator it_;
    size_t size_;
public:
    MedianFinder(){
        size_ = 0;
    }
    void Insert(int val){
        if (size_ == 0){
            auto i = ms.insert(val);
            it_ = i;
            size_++;
            return;
        }
        ms.insert(val);
        size_++;
        if (*it_ <= val){
            if (size_ % 2 != 0){
                it_++;
            }
        }
        else{
            if (size_ % 2 == 0){
                it_--;
            }
        }
    }
    void Remove(int val){
        auto find_it = ms.find(val);
        if (find_it == ms.end()){
            return;
        }
        if (find_it == it_){
            if (size_ % 2 != 0){
                it_--;
            }
            else{
                it_++;
            }
        }
        else{
            if (*it_ < val){
                if (size_ % 2 != 0){
                    it_--;
                }
            }
            else{
                if (size_ % 2 == 0){
                    it_++;
                }
            }
        }
        size_--;
        ms.erase(find_it);
    }

    int Find(){
        return *it_;
    }
};