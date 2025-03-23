#ifndef PHILOSOPHER_H
#define PHILOSOPHER_H

#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <chrono>
#include <iomanip>
#include <ctime>
#include <random>

using namespace std;

class Philosopher {
private:
    int num_philosophers;
    int eating_time;
    int min_thinking_time;
    int max_thinking_time;
    vector<mutex> forks;
    vector<thread> philosophers;
    vector<chrono::time_point<chrono::system_clock>> last_meal_time;
    mutex print_mutex;

    void philosopher(int id) {
        int left_fork = id;
        int right_fork = (id + 1) % num_philosophers;
        while (true) {
            think(id);
            eat(id, left_fork, right_fork);
        }
    }

    void think(int id) {
        static std::mt19937 rnd(std::time(nullptr));
        int thinking_time = std::uniform_int_distribution<>(min_thinking_time, max_thinking_time)(rnd);

        {
            lock_guard<mutex> lock(print_mutex);
            cout << timestamp() << " Philosopher " << id << " is thinking...";
            cout << "  Philosopher " << id << " last ate " << last_meal_seconds_ago(id) << " seconds ago" << endl;
        }
        this_thread::sleep_for(chrono::milliseconds(thinking_time));
    }

    void eat(int id, int left_fork, int right_fork) {
        bool left_locked = forks[left_fork].try_lock();
        bool right_locked = forks[right_fork].try_lock();

        if (left_locked && right_locked) {
            {
                lock_guard<mutex> lock(print_mutex);
                cout << timestamp() << " Philosopher " << id << " is eating..." << endl;
            }
            this_thread::sleep_for(chrono::milliseconds(eating_time));
            forks[left_fork].unlock();
            forks[right_fork].unlock();
            last_meal_time[id] = chrono::system_clock::now();
        } else {
            if (left_locked) {
                forks[left_fork].unlock();
            }
            if (right_locked) {
                forks[right_fork].unlock();
            }
            {
                lock_guard<mutex> lock(print_mutex);
                cout << timestamp() << " Philosopher " << id << " wants to eat, but forks are busy..." << endl;
            }
            this_thread::sleep_for(chrono::milliseconds(100));
        }
    }

    string timestamp() {
        auto now = chrono::system_clock::now();
        auto in_time_t = chrono::system_clock::to_time_t(now);
        stringstream ss;
        ss << put_time(localtime(&in_time_t), "%X");
        return ss.str();
    }

    int last_meal_seconds_ago(int id) {
        auto now = chrono::system_clock::now();
        auto duration = chrono::duration_cast<chrono::seconds>(now - last_meal_time[id]);
        return duration.count();
    }

public:
    Philosopher(int n, int eat_time, int min_think_time, int max_think_time)
        : num_philosophers(n), eating_time(eat_time), min_thinking_time(min_think_time), max_thinking_time(max_think_time),
          forks(n), last_meal_time(n, chrono::system_clock::now()) {}

    void start() {
        for (int i = 0; i < num_philosophers; i++) {
            philosophers.emplace_back(&Philosopher::philosopher, this, i);
        }

        for (auto &philosopher : philosophers) {
            philosopher.join();
        }
    }
};


#endif //PHILOSOPHER_H