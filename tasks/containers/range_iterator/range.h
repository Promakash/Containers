#pragma once

class RangeIt{
private:
    int cur_index;
    int step_of_iteration;
public:
    RangeIt(const int index, const int step_){
        cur_index = index;
        step_of_iteration = step_;
    }

    RangeIt& operator++(){
        cur_index += step_of_iteration;
        return *this;
    }

    bool operator!=(const RangeIt& other){
        return cur_index != other.cur_index;
    }

    int operator*(){
        return cur_index;
    }
};

class Range {
private:
    friend class RangeIt;
    int start_;
    int end_;
    int step_;
public:
    Range(int start, int end, int step){
        start_ = start;
        end_ = end;
        step_ = step;
    }

    Range(int start, int end){
        start_ = start;
        end_ = end;
        step_ = 1;
    }

    Range(int end){
        start_ = 0;
        end_ = end;
        step_ = 1;
    }

    RangeIt begin(){
        return RangeIt(start_, step_);
    }

    //Я не понял как это сработало, но это сработало
    //Есть какая то единая формула или элемент, на котором обрывается итерация вот так и высчитывается? не силен в математике(((
    RangeIt end(){
        int last_num = end_ - ((end_-start_) % step_);
        if ((end_-start_) % step_ == 0){
            return RangeIt(last_num, step_);
        }
        return RangeIt(last_num+step_, step_);
    }
};