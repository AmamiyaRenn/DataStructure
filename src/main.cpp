#include <ctime>
#include <sstream>
#include "../inc/Structure.h"
#include "assert.h"
#include "../app/CommuDictionary.h"

using namespace std;

#define EntryAdd(sceneName, varName) (int)(sceneName), CommuID::varName, (size_t)(&varName), sizeof(varName)

int main(void)
{
    CommuDictionary dictionary(3);
    float WorldTime = 114.0;
    int textNum = 514;
    dictionary.addEntry((int)(SceneID::Chassis), CommuID::WorldTime, (size_t)(&WorldTime), sizeof(WorldTime));
    dictionary.addEntry(EntryAdd(SceneID::Bluetooth, textNum));
    dictionary.traverse(print<CommuEntry>());

    return 0;
}