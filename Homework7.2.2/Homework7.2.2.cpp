// Homework7.2.2.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#include <windows.h>
#include <conio.h>
#include <random>

std::mutex m;

void math(int cycle)
{
    m.lock();

    auto start = std::chrono::steady_clock::now();

    int value = 0;
    std::random_device rd;
    std::mt19937 gen{ rd() };

    std::thread::id this_id = std::this_thread::get_id();
    std::cout << this_id << ": ";

    for (int i = 0; i <= cycle; ++i)
    {
        //m.lock();
        std::uniform_int_distribution <> dis(0, 1000);
        value = dis(gen);
        std::cout << "|";
        std::this_thread::sleep_for(std::chrono::milliseconds(value));
        //m.unlock();
    }
    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    std::cout << " - поток закончил свою работу за " << elapsed_seconds.count() << " секунд." << std::endl;
    m.unlock();
}

int main(int argc, char** argv)
{
    setlocale(LC_ALL, "Russian");

    int cycles = 10;

    std::thread t1(math, cycles);
    std::thread t2(math, cycles);
    std::thread t3(math, cycles);
    std::thread t4(math, cycles);

    t1.join();
    t2.join();
    t3.join();
    t4.join();

    return 0;
}