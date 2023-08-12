#include "mtsumm.h"

mtsumm::mtsumm(const uint &_array_count, const uint &_threads) : array_count(_array_count), threads_count(_threads) {}
mtsumm::mtsumm(const uint &&_array_count, const uint &&_threads) : array_count(_array_count), threads_count(_threads) {}

void mtsumm::initialise()
{
    std::srand(time(NULL));
    for (uint i{0}; i < array_count; i++)
    {
        arr.push_back(std::abs(std::rand()) % 11);
    }
    return;
}

void mtsumm::run()
{
    uint step = array_count / threads_count;
    printMessage("Запущена обработка массива " + std::to_string(array_count) + " элементов");
    printMessage("в " + std::to_string(threads_count) + " потоков");
    std::vector<std::thread> threads;

    std::vector<long> t_diffs;
    uint k = 0;
    for (int i{0}; i < arr.size(); i += step)
    {

        t_diffs.push_back(0);
        std::thread t(&mtsumm::summa, this, i, i + step, k++, std::ref(t_diffs));
        threads.push_back(std::move(t));
    }
    for (auto &t : threads)
    {
        if (t.joinable())
            t.join();
    }
    printMessage("Обработка массива " + std::to_string(array_count) + " элементов в " + std::to_string(threads_count) + " потоков");
    long t_diff = 0;
    for (long &td : t_diffs)
    {
        t_diff += td;
    }
    printMessage("завершена за " + std::to_string(t_diff) + " миллисекунд");
}

void mtsumm::printMessage(const std::string &msg, bool endl)
{
#if defined(_WIN64) || defined(_WIN32)
    std::wstring wmesg = std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t>{}.from_bytes(msg.data());
    std::wcout << wmesg;
    if (endl)
        std::wcout << std::endl;
#else
    std::cout << msg;
    if (endl)
        std::cout << std::endl;
#endif
}

void mtsumm::summa(uint left, uint right, uint tread_num, std::vector<long> &t_diffs)
{
    if (right > arr.size())
        right = arr.size();
    if (left > right)
    {
        printMessage("Поток №" + std::to_string(tread_num) + " НЕВЕРНЫЕ ЗНАЧЕНИЯ.");
        return;
    }

    printMessage("\nПоток №" + std::to_string(tread_num) + " запущен.");
    printMessage("Будут обработаны элементы с " + std::to_string(left) + " по " + std::to_string(right));
    auto t_start = std::chrono::system_clock::now();
    uint res = 0;
    for (uint i{left}; i < right; i++)
    {
        res += arr[i];
    }
    auto t_end = std::chrono::system_clock::now();
    long t_diff = std::chrono::duration_cast<std::chrono::milliseconds>(t_end - t_start).count();
    t_diffs[tread_num] = t_diff;
    printMessage("\n>>>>Поток №" + std::to_string(tread_num) + " завершил работу за " + std::to_string(t_diff) + " миллисекунд");
    printMessage("Обработано: " + std::to_string(right - left) + " элементов");
    printMessage("Значение суммы: " + std::to_string(res) + "\n");
}
