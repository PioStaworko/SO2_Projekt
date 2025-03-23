#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <chrono>
#include "Philosopher.h"

using namespace std;

int main() {
    int num_philosophers;
    int eating_time;
    int min_thinking_time;
    int max_thinking_time;

    cout << "Enter the number of philosophers: ";
    cin >> num_philosophers;

    if (num_philosophers <= 0) {
        cerr << "Invalid number of philosophers. Exiting." << endl;
        return 1;
    }

    cout << "Enter the eating time (in milliseconds): ";
    cin >> eating_time;

    cout << "Enter the minimum thinking time (in milliseconds): ";
    cin >> min_thinking_time;

    cout << "Enter the maximum thinking time (in milliseconds): ";
    cin >> max_thinking_time;

    Philosopher dp(num_philosophers, eating_time, min_thinking_time, max_thinking_time);
    dp.start();

    return 0;
}