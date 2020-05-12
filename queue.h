#ifndef WORD_C2_QUEUE_H
#define WORD_C2_QUEUE_H
#include <deque>
#include <mutex>
#include <condition_variable>
#include <iostream>
#include <atomic>

template <typename T>
class Queue {
private:
    T poison_pill;
    std::deque<T> que_m;
    mutable std::mutex m_m;
    const size_t MAX_LENGTH = 80;


public:
    std::condition_variable cv_is_empty_m;
    std::condition_variable cv_is_max_length_m;
    std::atomic<bool> will_be_push = true;

    Queue(const T& p_p) : poison_pill{p_p} {};

    T pop_back() {

        std::unique_lock<std::mutex> lg{m_m};
        while ((que_m.empty() && will_be_push)) {
            cv_is_empty_m.wait(lg);
        }
        if (!will_be_push && que_m.size() == 0) {
            return poison_pill;
        }
        T last_el = std::move(que_m.back());
        que_m.pop_back();

        cv_is_max_length_m.notify_one();
        return last_el;

    };


    T pop_front(){
        std::lock_guard<std::mutex> lg{m_m};
        if (!will_be_push && que_m.size() == 0) {
            return poison_pill;
        }
        cv_is_empty_m.wait(lg, [this](){return que_m.size() != 0;});
        T first_el = std::move(que_m.front());
        que_m.pop_front();
        cv_is_max_length_m.notify_one();
        return first_el;
    };

    void push_front(const T& elem, bool limit = true){
        {
            std::unique_lock<std::mutex> lg{m_m };
            if(limit)
                cv_is_max_length_m.wait(lg, [this](){return que_m.size() < MAX_LENGTH;});
            que_m.push_front(elem);
            cv_is_empty_m.notify_one();
        }
    };

    void push_front(T&& elem, bool limit = true){
        {
            std::unique_lock<std::mutex> lg{m_m };
            if(limit)
                cv_is_max_length_m.wait(lg, [this](){return que_m.size() < MAX_LENGTH;});
            que_m.push_front(std::move(elem));
        }
        cv_is_empty_m.notify_one();
    };

    void push_back(const T& elem, bool limit = true){
        {
            std::unique_lock<std::mutex> lg{m_m };
            if(limit)
                cv_is_max_length_m.wait(lg, [this](){return que_m.size() < MAX_LENGTH;});
            que_m.push_back(elem);
            cv_is_empty_m.notify_one();
        }
    };

    void push_back(T&& elem, bool limit = true){
        {
            std::unique_lock<std::mutex> lg{m_m };
            if(limit)
                cv_is_max_length_m.wait(lg, [this](){return que_m.size() < MAX_LENGTH;});
            que_m.push_back(std::move(elem));
        }
        cv_is_empty_m.notify_one();
    };

    size_t size() const{
        std::lock_guard<std::mutex> lg{m_m};
        return que_m.size();
    };

    bool is_empty() const{
        std::unique_lock<std::mutex> lg{m_m};
        return que_m.empty();
    };

    T get_poison_pill(){
        return poison_pill;
    }

};
#endif //WORD_C2_QUEUE_H
