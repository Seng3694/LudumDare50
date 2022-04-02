#include "SaveFileManager.hpp"

#include <iostream>
#include <fstream>

SaveData::SaveData()
{ 
    for (uint32_t i = 0; i < SAVE_DATA_MAP_COUNT; ++i)
    {
        scores[i].map = (Maps)i;
        scores[i].value = 0;
        scores[i].time = 0.0f;
    }
}

SaveFileManager::SaveFileManager(const std::string &fileName)
    : fileName(fileName), data(nullptr)
{
}

std::shared_ptr<SaveData> SaveFileManager::load()
{
    if (data)
        return data;

    data = std::make_shared<SaveData>();
    std::ifstream file;
    file.open(fileName, std::ios::in | std::ios::binary);
    if (file.is_open())
    {
        file.seekg(0, file.end);
        size_t size = file.tellg();
        if (size != sizeof(SaveData))
        {
            printf("Invalid save file\n");
            return data;
        }
        file.seekg(0, file.beg);
        
        file.read((char*)data.get(), sizeof(SaveData));
        file.close();
    }
    return data;
}

void SaveFileManager::save()
{
    if (!data)
        return;
    std::ofstream file;
    file.open(fileName, std::ios::in | std::ios::binary | std::ios::trunc);
    if (file.is_open())
    {
        file.write((char *)data.get(), sizeof(SaveData));
        file.close();
    }
}
