#pragma once

#include "Macro.h"

// vertext status
enum class VStatus
{
    UNDISCOVERD,
    DISCOVERD,
    VISTIED
};
// 边在遍历树中所属的类型
enum class EType
{
    UNDETERMINED,
    TREE,
    CROSS,
    FORWORD,
    BACKWARD
};

template <typename Tv>
struct Vertex
{
    Tv data;
    Rank inDegree, outDegree; // 度
    VStatus status;           // 顶点状态
    DSize dTime, fTime;       // 时间标签
    Rank parent;              // 在遍历树中的父节点
    Rank priority;            // 在遍历树中优先级数
    Vertex(const Tv &d = (Tv)0) : data(d), inDegree(0), outDegree(0),
                                  status(VStatus::UNDISCOVERD), dTime(0), fTime(0),
                                  parent(0), priority(0){};
};

template <typename Te>
struct Edge
{
    Te data;
    DSize weight; // 权重
    EType type;   // 类型
    Edge(const Te &d = (Te)0, DSize weight = 0) : data(d), weight(weight), type(EType::UNDETERMINED){};
};

template <typename Tv, typename Te> // 顶点类型，边类型
class Graph
{
public:
    Graph() : n(0), e(0){};
    // 顶点数据
    virtual Tv &vertex(Rank) = 0;
    // 顶点入度
    virtual Rank inDegree(Rank) = 0;
    // 顶点出度
    virtual Rank outDegree(Rank) = 0;
    // 顶点状态
    virtual VStatus &status(Rank) = 0;
    // 顶点的时间标签dTime
    virtual DSize &dTime(Rank) = 0;
    // 顶点的时间标签fTime
    virtual DSize &fTime(Rank) = 0;
    // 顶点在遍历树中的父亲
    virtual Rank &parent(Rank) = 0;
    // 顶点在遍历树中的优先级
    virtual Rank &priority(Rank) = 0;
    // 顶点的首个邻接顶点
    virtual Rank firstNbr(Rank) = 0;
    // 顶点的下一个邻接顶点
    virtual Rank nextNbr(Rank, Rank) = 0;
    // 插入顶点，返回编号
    virtual Rank insert(const Tv &) = 0;
    // 删除顶点及其关联边，返回该顶点信息
    virtual Tv remove(Rank) = 0;
    // 边数据
    virtual Te &edge(Rank, Rank) = 0;
    // 边权重
    virtual DSize &weight(Rank, Rank) = 0;
    // 边类型
    virtual EType &type(Rank, Rank) = 0;
    // 边(v, u)是否存在
    virtual bool exists(Rank, Rank) = 0;
    // 在两个顶点之间插入指定权重的边
    virtual void insert(const Te &, DSize, Rank, Rank) = 0;
    // 删除一对顶点的边，返回该边的信息
    virtual Te remove(Rank, Rank) = 0;

protected:
    Rank n; // 顶点个数
    Rank e; // 边总数
};