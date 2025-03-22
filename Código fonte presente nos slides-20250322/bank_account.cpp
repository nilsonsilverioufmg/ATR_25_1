#include <iostream>
#include <thread>
#include <vector>
#include <random>
#include <algorithm>
#include <atomic>

static const int NUM_OPERATIONS = 10; // Number of operations to perform
static const int NUM_THREADS = 1000; // Number of threads

std::atomic<int> account_balance(0); // Shared bank account balance

// Create a random device and seed the generator
std::random_device rd;
std::mt19937 gen(rd());

int sample_random_integer(int N) {
    std::uniform_int_distribution<> dist(0, N - 1);
    return dist(gen);
}

void perform_operations(const int thread_id) {
    for (int i = 0; i < NUM_OPERATIONS; ++i) {
        int operation = sample_random_integer(10); // 0 for deposit, 1 for withdrawal
        int amount = sample_random_integer(100); // Random amount between 0 and 99

        if (operation <= 1) {
            // Deposit operation
            account_balance += amount;
            std::cout << "Thread " << thread_id << " deposited " << amount << ". Balance: " << account_balance.load() << std::endl;
        } else {
            // Withdrawal operation
            if (account_balance >= amount) {
                std::this_thread::sleep_for(std::chrono::microseconds(1));
                account_balance -= amount;
                std::cout << "Thread " << thread_id << " withdrew " << amount << ". Balance: " << account_balance.load() << std::endl;
            } else {
                std::cout << "Thread " << thread_id << " tried to withdraw " << amount << " but insufficient funds. Balance: " << account_balance.load() << std::endl;
            }
        }
    }
}

int main() {
    std::vector<std::thread> threads;

    for (int i = 0; i < NUM_THREADS; ++i) {
        threads.emplace_back(perform_operations, i);
    }

    for (auto& t : threads) {
        t.join();
    }

    std::cout << "Final balance: " << account_balance.load() << std::endl;

    return 0;
}