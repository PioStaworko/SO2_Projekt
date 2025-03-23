#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <chrono>
#include "Philosopher.h"

using namespace std;



int main() {

    int num_philosophers;
    cout << "Enter the number of philosophers: ";
    cin >> num_philosophers;

    if (num_philosophers <= 0) {
        cerr << "Invalid number of philosophers. Exiting." << endl;
        return 1;
    }

    Philosopher dp(num_philosophers);
    dp.start();
    

    return 0;
}
