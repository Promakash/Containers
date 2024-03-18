#pragma once

class RangeIt{
private:
    int cur_index;
    int end_of_iteration;
    int step_of_iteration;
public:
    RangeIt(const int index, const int step_, const int end_){
        cur_index = index;
        step_of_iteration = step_;
        end_of_iteration = end_;
    }

    RangeIt& operator++(){
        cur_index += step_of_iteration;
        if (cur_index - end_of_iteration > 0  && step_of_iteration > 0) {
            cur_index = end_of_iteration;
        }
        if (cur_index - end_of_iteration < 0 && step_of_iteration < 0) {
            cur_index = end_of_iteration;
        }
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
    int start_;
    int end_;
    int step_;
public:
    Range(int start, int end, int step){
        start_ = start;
        end_ = end;
        step_ = step;
    }

    Range(int start, int end) : Range(start, end, 1) {}

    Range(int end) : Range(0, end, 1) {}

    RangeIt begin(){
        return RangeIt(start_, step_, end_);
    }

    RangeIt end(){
        return RangeIt(end_, step_, end_);
    }
};
