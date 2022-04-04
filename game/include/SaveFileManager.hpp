#pragma once

#include <string>
#include <memory>

#include "Score.hpp"

#define SAVE_DATA_MAP_COUNT 9
struct SaveData
{
  public:
    SaveData();
    //settings for later
    float generalVolume;
    Score scores[SAVE_DATA_MAP_COUNT];
};

class SaveFileManager
{
  public:
    SaveFileManager(const std::string &fileName);

    std::shared_ptr<SaveData> load();
    void save();

  private:
    std::shared_ptr<SaveData> data;
    std::string fileName;
};
