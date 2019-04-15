//
// Created by YangGuang on 2019-04-12.
//

#ifndef CAMPUS_CHAT_LIMIT_H
#define CAMPUS_CHAT_LIMIT_H

namespace footbook {

template<typename T>
class Limit final {
 public:
    using size_type = T;

    Limit(size_type start, size_type end)
            : min_(start), max_(end) {}

    Limit(const Limit &other) = default;

    Limit &operator=(const Limit &other) = default;

    Limit(Limit &&other) noexcept = default;

    Limit &operator=(Limit &&other) = default;

    void set_min(T start) { min_ = start; }
    void set_max(T end) { max_ = end; }

    size_type min() const { return min_; }

    size_type max() const { return max_; }

    template<typename Value>
    bool IsAmongLimit(Value value) const {
        return (value >= min_ && value <= max_) ? true : false;
    }

 private:
    size_type min_;
    size_type max_;
};

}   // namesapce footbook

#endif //CAMPUS_CHAT_LIMIT_H
