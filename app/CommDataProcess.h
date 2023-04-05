#pragma once

#include "CommuDictionary.h"
#include <string.h>

// typedef char u8;
//数据头 0xff,0xfe; id u8; 一字节检验和； 数据尾 '\n','\r'
//原始数据增加了6个字节
const static char head_char[2] = {static_cast<char>(0xff), static_cast<char>(0xfe)};
const static char end_char[2]  = {'\n', '\r'};

class DataProcess
{
public:
    DataProcess()
    {
        get_head_id = get_head = get_end = false;
        rec_len                          = 0;
        rec_head_arr[0] = rec_head_arr[1] = ' ';
        rec_end_arr[0] = rec_end_arr[1] = ' ';
    };
    bool getHeadMsg(char _ch)
    {
        if (!get_head)
        {
            rec_head_arr[0] = rec_head_arr[1];
            rec_head_arr[1] = _ch;
            if (rec_head_arr[0] == head_char[0] && rec_head_arr[1] == head_char[1])
            {
                get_head = true;
                rec_len  = 0;
            }
        }
        else if (!get_head_id)
        {
            head_id     = _ch;
            get_head_id = true;
        }
        else
            return get_head_id;

        return false;
    };

    void clearFlag()
    {
        rec_head_arr[0] = rec_head_arr[1] = ' ';
        rec_end_arr[0] = rec_end_arr[1] = ' ';
        get_head_id = get_head = get_end = false;
        rec_len                          = 0;
    };

    char headId() { return head_id; };

    template<typename T>
    bool dataDecode(char _ch, T* stru);

    template<typename T>
    char* dataEncode(T* stru, char _head_id, int* _len);

private:
    const static int ARRSIZE = 128;
    bool             get_head, get_end, get_head_id;
    char             rec_head_arr[2];
    char             rec_end_arr[2];
    char             rec_arr[ARRSIZE];
    char             send_arr[ARRSIZE];
    int              rec_len;
    char             head_id;
    char             checkSum(char* ch, int n)
    {
        char _sum = 0;
        for (int i = 0; i < n; i++)
            _sum += ch[i];
        return _sum;
    };
};

template<typename T>
bool DataProcess::dataDecode(char _ch, T* stru)
{
    if (rec_len < sizeof(T) + sizeof(end_char) + 1) // 数据, 检验和 endchar
    {
        rec_arr[rec_len] = _ch;
        rec_len++;
        rec_end_arr[0] = rec_end_arr[1];
        rec_end_arr[1] = _ch;
        if (rec_end_arr[0] == end_char[0] && rec_end_arr[1] == end_char[1])
            get_end = true;
    }
    else // 数据满了, 重来
        clearFlag();

    if (get_end)
    {
        clearFlag();
        memcpy((char*)stru, &rec_arr[0], sizeof(T));
        return (rec_arr[sizeof(T)] == checkSum(&rec_arr[0], sizeof(T)));
    }
    return false;
}

template<typename T>
char* DataProcess::dataEncode(T* stru, char _head_id, int* _len)
{
    for (int i = 0; i < sizeof(head_char); i++)
    {
        send_arr[i] = head_char[i];
    }
    send_arr[sizeof(head_char)] = _head_id;
    memcpy(&send_arr[sizeof(head_char) + 1], (char*)stru, sizeof(T));
    send_arr[sizeof(T) + sizeof(head_char) + 1] = checkSum(&send_arr[sizeof(head_char) + 1], sizeof(T));
    for (int i = 0; i <= sizeof(end_char); i++)
    {
        send_arr[sizeof(T) + sizeof(head_char) + 2 + i] = end_char[i];
    }
    *_len = sizeof(T) + sizeof(head_char) + sizeof(end_char) + 2;
    return send_arr;
}
