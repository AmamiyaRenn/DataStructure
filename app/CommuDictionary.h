#pragma once

#include <string.h>
#include "../inc/Vector.h"
#include "../inc/List.h"

enum class SceneID
{
    Bluetooth,
    Chassis,
    Master,
    Fire,
    etc,
};

// 通讯ID，只允许有<255的数量
enum class CommuID
{
    Chassis_Angle1,
    textNum,
    WorldTime,
    etc,
};

// 通讯词典词条
struct CommuEntry
{
    CommuEntry(){};
    CommuEntry(CommuID ID, void *address = NULL, int size = 0)
        : ID(ID), address(address), size(size){};
    CommuID ID;    // 唯一ID
    void *address; // 词条数据地址
    uint8_t size;  // 词条数据字节数
    bool operator==(const CommuEntry &other) { return ID == other.ID; }
    bool operator!=(const CommuEntry &other) { return ID != other.ID; }
    bool operator<(const CommuEntry &other) { return ID < other.ID; }
    bool operator<=(const CommuEntry &other) { return ID <= other.ID; }
    bool operator>(const CommuEntry &other) { return ID > other.ID; }
    bool operator>=(const CommuEntry &other) { return ID >= other.ID; }
    long readData()
    {
        long data = 0;
        memcpy(&data, address, size);
        return data;
    }
    // for display
    friend std::ostream &operator<<(std::ostream &os, CommuEntry &entry)
    {
        os << "ID: " << (int)entry.ID << " 10进制地址" << (size_t)entry.address << " 字节大小" << (int)entry.size << " 读来的数据输出为long: " << entry.readData() << "\n";
        return os;
    }
};

// 通讯词典
class CommuDictionary
{
public:
    CommuDictionary(int sceneNum) : Dictionary(Vector<List<CommuEntry>>(sceneNum)), sceneNum(sceneNum){};
    int SceneNum() const { return sceneNum; }
    // 添加词条
    template <typename Entry>
    void addEntry(SceneID sceneID, CommuID ID, Entry &entry) { Dictionary[(int)sceneID].insertAsLast(CommuEntry(ID, &entry, sizeof(Entry))); }
    // 词典有序化（添加完词条后需要有序化，才能让解码成功）
    void ordering()
    {
        for (int i = 0; i < sceneNum; i++)
            Dictionary[i].selectionSort();
    }
    // 对整个词典进行遍历
    template <typename VST>
    void traverse(VST &visit)
    {
        for (int i = 0; i < sceneNum; i++)
            Dictionary[i].travForward(visit);
    }
    // 对整个词典中给定的scene进行遍历
    template <typename VST>
    void traverse(VST &visit, SceneID sceneID) { Dictionary[(int)sceneID].travForward(visit); }
    /**
     * @brief 从buffer+offset处开始对指定场景编码
     * @return size_t 返回offset+场景编码数据量
     */
    size_t encode(SceneID sceneID, char *buffer, size_t offset = 0)
    {
        struct EncodeBuffer
        {
            char *buffer;
            size_t &offset;
            EncodeBuffer(char *buffer, size_t &offset) : buffer(buffer), offset(offset){};
            /**
             * @brief 把此词条编码入字节数组，编码格式为小端法:(4B->1B)ID->(1B)size->((size)B)数据内容
             * @param entry 词条
             */
            void operator()(CommuEntry entry)
            {
                memcpy(buffer + offset++, &entry.ID, 1);
                memcpy(buffer + offset++, &entry.size, 1);
                memcpy(buffer + offset, entry.address, entry.size);
                offset += entry.size;
            }
        };
        EncodeBuffer encodeBuffer(buffer, offset);
        Dictionary[(int)sceneID].travForward(encodeBuffer);
        memset(buffer + offset, 255, 2); // 包尾标识符
        return offset + 2;
    }
    /**
     * @brief 从buffer+offset处开始对指定场景解码（假设已经对ID进行了排序）
     * @return size_t 返回offset+场景解码数据量
     */
    size_t decode(SceneID sceneID, char *buffer, size_t offset = 0)
    {
        static CommuEntry entry;
        ListNodePos<CommuEntry> entryPos = Dictionary[(int)sceneID].first();
        while (true)
        {
            memcpy(&entry.ID, buffer + offset++, 1);
            memcpy(&entry.size, buffer + offset++, 1);
            if ((int)entry.ID == 255 && entry.size == 255) // 到最后了
                return offset;
            while (true)
            {
                if (entryPos->element.ID == entry.ID) // ID相同
                {
                    memcpy((entryPos->element.address), buffer + offset, entry.size);
                    entryPos = entryPos->next;
                    break;
                }
                else if (entryPos->element.ID < entry.ID)
                {
                    if (entryPos == Dictionary[(int)sceneID].last())
                        break;
                    entryPos = entryPos->next;
                }
                else if (entryPos->element.ID > entry.ID)
                    break;
            }
            offset += entry.size;
        }
        return offset;
    }

private:
    Vector<List<CommuEntry>> Dictionary; // 因为场景个数是确定的且需要快速访问，而场景内词条需要完全遍历且不确定个数，所以用Vector<List>
    int sceneNum;
};
