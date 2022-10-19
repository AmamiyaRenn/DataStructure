#pragma once

#include "../inc/Entry.h"
#include "../inc/List.h"
#include "../inc/Vector.h"
#include <string.h>

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
struct CommuEntryValue
{
    explicit CommuEntryValue(void* address = nullptr, uint8_t size = 0) : address(address), size(size) {};
    void*   address; // 词条数据地址
    uint8_t size;    // 词条数据字节数
};

// 通讯词典
class CommuDictionary
{
public:
    explicit CommuDictionary(int sceneNum) :
        dictionary(Vector<List<Entry<CommuID, CommuEntryValue>>>(sceneNum, sceneNum)), scene_num(sceneNum) {};
    int sceneNum() const { return scene_num; }
    // 添加词条
    template<typename E>
    void addEntry(SceneID sceneID, CommuID ID, E& entry)
    {
        dictionary[(int)sceneID].insertAsLast(Entry<CommuID, CommuEntryValue>(ID, CommuEntryValue(&entry, sizeof(E))));
    }
    // 词典有序化（添加完词条后需要有序化，才能让解码成功）
    void ordering()
    {
        for (int i = 0; i < scene_num; i++)
            dictionary[i].selectionSort();
    }
    /**
     * @brief 从buffer+offset处开始对指定场景编码
     * @return size_t 返回offset+场景编码数据量
     */
    size_t encode(SceneID sceneID, char* buffer, size_t offset = 0)
    {
        struct EncodeBuffer
        {
            char*   buffer;
            size_t& offset;
            EncodeBuffer(char* buffer, size_t& offset) : buffer(buffer), offset(offset) {};
            /**
             * @brief 把此词条编码入字节数组，编码格式为小端法:(4B->1B)ID->(1B)size->((size)B)数据内容
             * @param entry 词条
             */
            void operator()(Entry<CommuID, CommuEntryValue> entry)
            {
                memcpy(buffer + offset++, &entry.key, 1);
                memcpy(buffer + offset++, &entry.value.size, 1);
                memcpy(buffer + offset, entry.value.address, entry.value.size);
                offset += entry.value.size;
            }
        };
        EncodeBuffer encode_buffer(buffer, offset);
        dictionary[(int)sceneID].travForward(encode_buffer);
        memset(buffer + offset, 255, 2); // 包尾标识符
        return offset + 2;
    }
    /**
     * @brief 从buffer+offset处开始对指定场景解码（假设已经对ID进行了排序）
     * @return size_t 返回offset+场景解码数据量
     */
    size_t decode(SceneID sceneID, char* buffer, size_t offset = 0)
    {
        Entry<CommuID, CommuEntryValue>              entry;
        ListNodePos<Entry<CommuID, CommuEntryValue>> entry_pos = dictionary[(int)sceneID].first();
        while (true)
        {
            memcpy(&entry.key, buffer + offset++, 1);
            memcpy(&entry.value.size, buffer + offset++, 1);
            if ((int)entry.key == 255 && entry.value.size == 255) // 到最后了
                return offset;
            while (true)
            {
                if (entry_pos->element == entry) // ID相同
                {
                    memcpy((entry_pos->element.value.address), buffer + offset, entry.value.size);
                    entry_pos = entry_pos->next;
                    break;
                }
                else if (entry_pos->element < entry)
                {
                    if (entry_pos == dictionary[(int)sceneID].last())
                        break;
                    entry_pos = entry_pos->next;
                }
                else if (entry_pos->element > entry)
                    break;
            }
            offset += entry.value.size;
        }
        return offset;
    }

private:
    Vector<List<Entry<CommuID, CommuEntryValue>>>
        dictionary; // 因为场景个数是确定的且需要快速访问，而场景内词条需要完全遍历且不确定个数，所以用Vector<List>
    int scene_num;
};
