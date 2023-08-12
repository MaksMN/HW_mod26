#pragma once
#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <chrono>
#include <ctime>
#include <codecvt>
#include <locale>
typedef unsigned int uint;
class mtsumm
{
private:
    const uint array_count;
    const uint threads_count;
    std::vector<int> arr;

public:
    mtsumm(const uint &_array_count, const uint &_threads);
    mtsumm(const uint &&_array_count, const uint &&_threads);
    void initialise();
    void run();
    ~mtsumm() = default;
    void printMessage(const std::string &msg, bool endl = true);

private:
    /// @brief Вычисляет сумму
    /// @param left начало в массиве
    /// @param right конец в массиве
    /// @param t_diff время выполнения
    /// @return
    void summa(uint left, uint right, uint tread_num, std::vector<long> &t_diffs);
};
