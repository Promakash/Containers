#include "cstdint"
#include "iterator"

template<typename It>
class EnumerateIt{
private:
    It iterator_;
    size_t idx;
    using value_type = typename std::iterator_traits<It>::value_type;
public:
    EnumerateIt(It iterator){
        iterator_ = iterator;
        idx = 0;
    }

    EnumerateIt& operator++(){
        iterator_++;
        idx += 1;
        return *this;
    };

    bool operator!=(const EnumerateIt& other){
        return iterator_ != other.iterator_;
    }

    std::pair<size_t, value_type> operator*() const {
        return {idx, *iterator_};
    }
};

template<typename It>
class Enumerate {
    It begin_it;
    It end_it;
public:
    Enumerate(It begin, It end){
        begin_it = begin;
        end_it = end;
    }

    EnumerateIt<It> begin(){
        return EnumerateIt<It>(begin_it);
    }
    EnumerateIt<It> end(){
        return EnumerateIt<It>(end_it);
    }
};