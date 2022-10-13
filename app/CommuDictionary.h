#pragma once

#include "../inc/Vector.h"
#include "../inc/List.h"

enum class SceneID
{
    Bluetooth,
    Chassis,
    etc,
};

enum class CommuID
{
    Chassis_Angle1,
    textNum,
    WorldTime,
    etc,
};

#include <iostream>
struct CommuEntry
{
    CommuEntry(){};
    CommuEntry(CommuID ID, size_t address, int size)
        : ID(ID), address(address), size(size){};
    CommuID ID;     // 唯一ID
    size_t address; // 词条数据地址
    int size;       // 词条数据大小(sizeof(this))
    long readData()
    {
        long data;
        memcpy(&data, (void *)address, size);
        return data;
    }
    bool operator==(const CommuEntry &other) { return ID == other.ID; }
    bool operator!=(const CommuEntry &other) { return ID != other.ID; }
    bool operator<(const CommuEntry &other) { return ID < other.ID; }
    bool operator<=(const CommuEntry &other) { return ID <= other.ID; }
    bool operator>(const CommuEntry &other) { return ID > other.ID; }
    bool operator>=(const CommuEntry &other) { return ID >= other.ID; }
    friend std::ostream &operator<<(std::ostream &os, CommuEntry &entry)
    {
        os << "ID:" << (int)entry.ID << " 10进制地址:" << entry.address << " 字节大小:" << entry.size << " 粗暴的直接把读来的数据输出为long:" << entry.readData() << "\n";
        return os;
    }
};

class CommuDictionary
{
public:
    CommuDictionary(int sceneNum) : Dictionary(Vector<List<CommuEntry>>(sceneNum)), sceneNum(sceneNum){};
    int SceneNum() const { return sceneNum; }
    // 添加词条
    void addEntry(int sceneIndex, CommuID ID, size_t address, int size) { Dictionary[sceneIndex].insertAsLast(CommuEntry(ID, address, size)); }
    // 对整个词典进行遍历
    template <typename VST>
    void traverse(VST &visit)
    {
        for (int i = 0; i < sceneNum; i++)
            Dictionary[i].travForward(visit);
    }
    // 对整个词典中给定的scene进行遍历
    template <typename VST>
    void traverse(VST &visit, int sceneIndex) { Dictionary[sceneIndex].travForward(visit); }

private:
    Vector<List<CommuEntry>> Dictionary; // 因为场景个数是确定的且需要快速访问，而场景内词条需要完全遍历且不确定个数，所以用Vector<List>
    int sceneNum;
};