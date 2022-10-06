#pragma once

#include "Macro.h"

// vertext status
enum class VStatus
{
    Undiscovered,
    Discoverd,
    Visited
};
// 边在遍历树中所属的类型
enum class EType
{
    Undetermined,
    Tree,
    Cross,
    Forward,
    Backward
};

template <typename Tv>
struct Vertex
{
    Tv data;
    Rank inDegree, outDegree; // 度
    VStatus status;           // 顶点状态
    Rank dTime;               // 被发现的时间
    Rank fTime;               // 被访问的时间
    Rank parent;              // 在遍历树中的父节点
    Rank priority;            // 在遍历树中优先级数
    Vertex(const Tv &d = (Tv)0) : data(d), inDegree(0), outDegree(0),
                                  status(VStatus::Undiscovered), dTime(0), fTime(0),
                                  parent(0), priority(0){};
};

template <typename Te>
struct Edge
{
    Te data;
    Rank weight; // 权重
    EType type;  // 类型
    Edge(const Te &d = (Te)0, Rank weight = 0) : data(d), weight(weight), type(EType::Undetermined){};
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
    virtual Rank &dTime(Rank) = 0;
    // 顶点的时间标签fTime
    virtual Rank &fTime(Rank) = 0;
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
    virtual Rank &weight(Rank, Rank) = 0;
    // 边类型
    virtual EType &type(Rank, Rank) = 0;
    // 边(v, u)是否存在
    virtual bool exists(Rank, Rank) = 0;
    // 在两个顶点之间插入指定权重的边
    virtual void insert(const Te &, Rank, Rank, Rank) = 0;
    // 删除一对顶点的边，返回该边的信息
    virtual Te remove(Rank, Rank) = 0;
    // 多连通域BFS
    template <typename VST>
    void BFS(Rank s, VST &visit)
    {
        reset();
        Rank clock = 0, v = s;
        do
            if (status(v) == VStatus::Undiscovered)
                BFS_ConnectedDomain(v, clock);
        while (s != (v = (v + 1) % n));
    }
    // 多连通域DFS
    template <typename VST>
    void DFS(Rank s, VST &visit)
    {
        reset();
        Rank clock = 0, v = s;
        do
            if (status(v) == VStatus::Undiscovered)
                DFS_ConnectedDomain(v, clock);
        while (s != (v = (v + 1) % n));
    }
    Stack<Tv> *TopoSort(Rank s)
    {
        Stack<Tv> *S = new Stack<Tv>;
        struct PushEdge
        {
            PushEdge(Stack<Tv> *edges) : edges(edges){};
            void operator()(Tv edge) { edges->push(edge); }
            Stack<Tv> *edges;
        } pushEdge(S);
        DFS(s, pushEdge);
        return S;
    }

protected:
    Rank n; // 顶点个数
    Rank e; // 边总数

private:
    // 所有辅助信息复位
    void reset()
    {
        for (Rank v = 0; v < n; v++)
        {
            status(v) = VStatus::Undiscovered;
            dTime(v) = fTime(v) = -1;
            parent(v) = -1;
            priority(v) = INT_MAX;
            for (Rank u = 0; u < n; u++)
                if (exists(v, u))
                    type(v, u) = EType::Undetermined;
        }
    }
    // 连通域BFS；dTime为代际标签，fTime为兄弟标签
    template <typename VST>
    void BFS_ConnectedDomain(Rank v, Rank &clock, VST &visit)
    {
        Queue<Rank> Q;
        status(v) = VStatus::Discoverd;
        dTime(v) = clock;
        Q.push(v);
        clock = 0;
        while (!Q.empty())
        {
            Rank v = Q.dequeue();
            for (Rank u = firstNbr(v); - 1 < u; u = nextNbr(v, u))
                switch (status(v))
                {
                case VStatus::Undiscovered:
                    status(u) = VStatus::Discoverd;
                    dTime(u) = dTime(v) + 1;
                    type(v, u) = EType::Tree;
                    parent(u) = v;
                    Q.enqueue(u);
                    break;

                default:
                    type(v, u) = EType::Cross;
                    break;
                }
            visit(v);
            status(v) = VStatus::Visited;
            fTime(v) = clock++;
            if (dTime(v) < dTime(Q.front()))
                clock = 0;
        }
        clock = dTime(v) + 1;
    }
    // 连通域DFS
    template <typename VST>
    void DFS_ConnectedDomain(Rank v, Rank &clock, VST &visit)
    {
        status(v) = VStatus::Discoverd;
        dTime(v) = clock++;
        for (Rank u = firstNbr(v); - 1 < u; u = nextNbr(v, u))
            switch (status(u))
            {
            case VStatus::Undiscovered:
                type(v, u) = EType::Tree;
                parent(u) = v;
                DFS_ConnectedDomain(u, clock);
                break;

            case VStatus::Discoverd:
                type(v, u) = EType::Backward;

            default:
                type(v, u) = (dTime(v) < dTime(u)) ? EType::Forward : EType::Cross;
                break;
            }
        visit(v);
        status(v) = VStatus::Visited;
        fTime(v) = clock++;
    }
};
