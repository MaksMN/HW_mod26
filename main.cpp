#include "mtsumm.h"
#include <fstream>
#include <cstring>
#if defined(_WIN64) || defined(_WIN32)
#include <io.h>
#include <fcntl.h>
#endif

typedef unsigned int uint;
std::string getConfigValue(const std::string &path, const std::string &section, const std::string &value)
{
    std::ifstream stream(path);

    std::string line;
    const int section_len = section.length();
    const int value_len = value.length();

    char section_buff[section_len];
    char value_buff[value_len];

    while (1)
    {
        if (std::getline(stream, line, '['))
        {
            stream.read(section_buff, section_len);
            if (memcmp(section_buff, section.data(), section_len) == 0)
            {
                if (!std::getline(stream, line, ']'))
                    break;

                std::getline(stream, line, '[');

                // очистка пробелов и табуляторов перед параметрами
                while (1)
                {
                    auto value_bpos = line.find("\n ");
                    auto value_tpos = line.find("\n\t");
                    if (value_bpos == std::string::npos && value_tpos == std::string::npos)
                        break;

                    if (value_bpos != std::string::npos)
                        line.erase(value_bpos + 1, 1);
                    if (value_tpos != std::string::npos)
                        line.erase(value_tpos + 1, 1);
                }
                auto value_pos = line.find("\n" + value) + 1;
                if (value_pos == std::string::npos)
                    break;
                line.erase(0, value_len + value_pos);

                for (int i{0}; i < line.length(); i++)
                {
                    if (line[i] == '=' || line[i] == ' ')
                    {
                        line.erase(0, 1);
                    }
                    else
                    {
                        break;
                    }
                }

                char deleted[5]{';', '#', '\n', '\r', '\t'};
                for (int i{0}; i < 5; i++)
                {
                    value_pos = line.find(deleted[i]);
                    if (value_pos != std::string::npos)
                        line.erase(value_pos, line.length() - value_pos);
                }
                stream.close();
                return line;
            }
            else
            {
                continue;
            }
        }
        else
        {
            break;
        }
    }
    stream.close();
    return std::string();
}
int main()
{
#if defined(_WIN64) || defined(_WIN32)
    _setmode(_fileno(stdout), _O_U16TEXT);
    _setmode(_fileno(stdin), _O_U16TEXT);
#endif
    uint array_count = (uint)atol(getConfigValue("conf.ini", "GENERAL", "array_count").data());
    uint threads_count = atoi(getConfigValue("conf.ini", "GENERAL", "threads_count").data());
    mtsumm mts(array_count, threads_count);
    mts.initialise();
    mts.run();
#if defined(_WIN64) || defined(_WIN32)
    system("pause");
#endif
    return 0;
}