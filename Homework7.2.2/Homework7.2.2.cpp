// Homework7.2.2.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#include <windows.h>
#include <conio.h>
#include <random>
#include <vector>

class Process
{
protected:
    std::mutex m;    

    int flows = 1;
    int max_flows = 0;
    int cycles = 0;

    int value = 0;
    std::random_device rd;
    std::mt19937 gen{ rd() };
public:
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD position = { 0, 0 };
    std::vector<int> x_pos;

    Process(int max_flows_, int cycles_) 
    {
        max_flows_ = max_flows;
        cycles_ = cycles;
    }

    void print(int pos_num, int cycles)
    {
        m.lock();
        std::uniform_int_distribution <> dis(100, 1000);
        value = dis(gen);

        std::this_thread::sleep_for(std::chrono::milliseconds(value));

        position.X = 0;
        position.Y = pos_num;
        SetConsoleCursorPosition(hConsole, position);

        std::thread::id this_id = std::this_thread::get_id();
        std::cout << "Поток " << pos_num + 1 << " (" << this_id << ")" << ": ";
        m.unlock();

        auto start = std::chrono::steady_clock::now();

        for (int i = 0; i < cycles; ++i)
        {
            std::uniform_int_distribution <> dis(300, 1000);
            value = dis(gen);
            std::this_thread::sleep_for(std::chrono::milliseconds(value));
            m.lock();
            position.X = x_pos[pos_num];
            position.Y = pos_num;
            SetConsoleCursorPosition(hConsole, position);
            std::cout << "|";
            ++x_pos[pos_num];
            m.unlock();
        }

        auto end = std::chrono::steady_clock::now();
        std::chrono::duration<double> elapsed_seconds = end - start;
        std::cout << " - поток закончил свою работу за " << elapsed_seconds.count() << " секунд." << std::endl;
    }
};

int main(int argc, char** argv)
{
    setlocale(LC_ALL, "Russian");

    int flows_num = 5;
    int cycles_num = 10;

    Process process(flows_num, cycles_num);

    std::vector<std::thread> threads(flows_num);

    for (unsigned i = 0; i < flows_num; ++i)
    {
        threads[i] = std::thread(&Process::print, &process, i, cycles_num);
        process.x_pos.push_back(20);
    }

    for (unsigned i = 0; i < flows_num; ++i)
    {
        threads[i].join();
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    process.position.X = 0;
    process.position.Y = flows_num + 2;
    SetConsoleCursorPosition(process.hConsole, process.position);
    return 0;
}