#pragma once

#include "Macro.h"
#include "Queue.h"
#include "Stack.h"

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
    Tree,    // 树边
    Cross,   // 跨边
    Forward, // 前向边（指向子孙）
    Backward // 后向边（指向祖先）
};

template<typename Tv>
struct Vertex
{
    Tv      data;
    Rank    in_degree, out_degree; // 度
    VStatus status;                // 顶点状态
    Rank    d_time;                // 被发现的时间
    Rank    f_time;                // 被访问的时间
    Rank    parent;                // 在遍历树中的父节点
    Rank    priority;              // 在遍历树中优先级数
    explicit Vertex(const Tv& d = Tv()) :
        data(d), in_degree(0), out_degree(0), status(VStatus::Undiscovered), d_time(0), f_time(0), parent(0),
        priority(0) {};
};

template<typename Te>
struct Edge
{
    Te    data;
    Rank  weight; // 权重
    EType type;   // 类型
    explicit Edge(const Te& d = Te(), Rank weight = 0) : data(d), weight(weight), type(EType::Undetermined) {};
};

/**
 * @brief 图模板
 * @details CD is Connected Domain
 * @tparam Tv 顶点类型
 * @tparam Te 边类型
 */
template<typename Tv, typename Te>
class Graph
{
public:
    Graph() : n(0), e(0) {};
    // 顶点数据
    virtual Tv& vertex(Rank) = 0;
    // 顶点入度
    virtual Rank inDegree(Rank) = 0;
    // 顶点出度
    virtual Rank outDegree(Rank) = 0;
    // 顶点状态
    virtual VStatus& status(Rank) = 0;
    // 顶点的时间标签dTime
    virtual Rank& dTime(Rank) = 0;
    // 顶点的时间标签fTime
    virtual Rank& fTime(Rank) = 0;
    // 顶点在遍历树中的父亲
    virtual Rank& parent(Rank) = 0;
    // 顶点在遍历树中的优先级
    virtual Rank& priority(Rank) = 0;
    // 顶点的首个邻接顶点
    virtual Rank firstNbr(Rank) = 0;
    // 顶点的下一个邻接顶点
    virtual Rank nextNbr(Rank, Rank) = 0;
    // 插入顶点，返回编号
    virtual Rank insert(const Tv&) = 0;
    // 删除顶点及其关联边，返回该顶点信息
    virtual Tv remove(Rank) = 0;
    // 边数据
    virtual Te& edge(Rank, Rank) = 0;
    // 边权重
    virtual Rank& weight(Rank, Rank) = 0;
    // 边类型
    virtual EType& type(Rank, Rank) = 0;
    // 边(v, u)是否存在
    virtual bool exists(Rank, Rank) = 0;
    // 在两个顶点之间插入指定权重的边
    virtual void insert(const Te&, Rank, Rank, Rank) = 0;
    // 删除一对顶点的边，返回该边的信息
    virtual Te remove(Rank, Rank) = 0;
    // 多连通域BFS
    template<typename VST>
    void bfs(Rank s, VST& visit)
    {
        reset();
        Rank clock = 0, v = s;
        do
            if (status(v) == VStatus::Undiscovered)
                bfsCD(v, clock, visit);
        while (s != (v = (++v % n)));
    }
    // 多连通域DFS
    template<typename VST>
    void dfs(Rank s, VST& visit)
    {
        reset();
        Rank clock = 0, v = s;
        do
            if (status(v) == VStatus::Undiscovered)
                dfsCD(v, clock, visit);
        while (s != (v = (++v % n)));
    }
    /**
     * @brief 多连通域基于DFS的拓扑排序算法
     * @return Stack<Tv>* 自顶向下排序排序的栈，如果为空则不存在拓扑排序
     */
    Stack<Tv>* topoSort(Rank s)
    {
        reset();
        Rank       clock = 0, v = s;
        Stack<Tv>* stack = new Stack<Tv>;
        do
        {
            if (status(v) == VStatus::Undiscovered)
                if (!topoSortCD(v, clock, stack))
                    while (!stack->empty())
                        stack->pop();
            break;
        } while (s != (v = (++v % n)));
        return stack;
    }
    // 多连通域基于DFS的双连通分量分解算法
    void bcc(Rank s)
    {
        reset();
        Rank        clock = 0, v = s;
        Stack<Rank> stack; // 用栈记录已经访问过的顶点
        do
        {
            if (status(v) == VStatus::Undiscovered)
                bccCD(v, clock, stack);
            stack.pop();
        } while (s != (v = (++v % n)));
    }
    // 最短路径Dijkstra算法
    template<typename VST>
    void dijkstra(Rank s, VST& visit)
    {
        struct DijkstraPrioUpdater
        {
            void operator()(Graph<Tv, Te>* G, Rank v, Rank u)
            {
                if (G->status(u) == VStatus::Undiscovered)
                {
                    Rank new_priority = G->priority(v) + G->weight(v, u);
                    if (new_priority < G->priority(u))
                    {
                        G->priority(u) = new_priority;
                        G->parent(u)   = v;
                    }
                }
            }
        } prio_updater;
        pfs(s, prio_updater, visit);
    }
    // 最小支撑树Prim算法
    template<typename VST>
    void prim(Rank s, VST& visit)
    {
        struct DijkstraPrioUpdater
        {
            void operator()(Graph<Tv, Te>* G, Rank v, Rank u)
            {
                if (G->status(u) == VStatus::Undiscovered)
                {
                    Rank new_priority = G->weight(v, u);
                    if (new_priority < G->priority(u))
                    {
                        G->priority(u) = new_priority;
                        G->parent(u)   = v;
                    }
                }
            }
        } prio_updater;
        pfs(s, prio_updater, visit);
    }
    template<typename PrioUpdater, typename VST>
    /**
     * @param s 优先级搜索框架
     * @param prioUpdater 优先级更新策略
     * @param visit 访问方法
     */
    void pfs(Rank s, PrioUpdater prioUpdater, VST& visit)
    {
        reset();
        Rank v = s;
        do
        {
            if (status(v) == VStatus::Undiscovered)
                pfsCD(v, prioUpdater, visit);
        } while (s != (v = (++v % n)));
    }

protected:
    Rank n; // 顶点个数
    Rank e; // 边总数（这里约定，无向边均统一转化为方向互逆的一对有向边，从而将无向图视作有向图的特例）

private:
    // 所有辅助信息复位
    void reset()
    {
        for (Rank v = 0; v < n; v++)
        {
            status(v) = VStatus::Undiscovered;
            dTime(v) = fTime(v) = -1;
            parent(v)           = -1;
            priority(v)         = INT_MAX;
            for (Rank u = 0; u < n; u++)
                if (exists(v, u))
                    type(v, u) = EType::Undetermined;
        }
    }
    // （连通域）BFS；dTime为代际标签，fTime为兄弟标签
    template<typename VST>
    void bfsCD(Rank v, Rank& clock, VST& visit)
    {
        Queue<Rank> queue;
        status(v) = VStatus::Discoverd;
        dTime(v)  = clock;
        queue.enqueue(v);
        clock = 0;
        while (!queue.empty())
        {
            Rank v = queue.dequeue();
            for (Rank u = firstNbr(v); - 1 < u; u = nextNbr(v, u))
                switch (status(u))
                {
                    case VStatus::Undiscovered:
                        status(u)  = VStatus::Discoverd;
                        dTime(u)   = dTime(v) + 1;
                        type(v, u) = EType::Tree;
                        parent(u)  = v;
                        queue.enqueue(u);
                        break;

                    default:
                        type(v, u) = EType::Cross;
                        break;
                }
            visit(v);
            status(v) = VStatus::Visited;
            fTime(v)  = clock++;
            if (dTime(v) < dTime(queue.front()))
                clock = 0;
        }
        clock = dTime(v) + 1;
    }
    // （连通域）DFS
    template<typename VST>
    void dfsCD(Rank v, Rank& clock, VST& visit)
    {
        status(v) = VStatus::Discoverd;                        // v被发现
        dTime(v)  = clock++;                                   // 记录v被发现的时间
        for (Rank u = firstNbr(v); - 1 < u; u = nextNbr(v, u)) // 检查v的邻居
            switch (status(u))
            {
                case VStatus::Undiscovered:
                    type(v, u) = EType::Tree;
                    parent(u)  = v;
                    dfsCD(u, clock, visit);
                    break;

                case VStatus::Discoverd:
                    type(v, u) = EType::Backward;

                case VStatus::Visited:
                    type(v, u) = (dTime(v) < dTime(u)) ? EType::Forward : EType::Cross;
                    break;
            }
        visit(v);
        status(v) = VStatus::Visited;
        fTime(v)  = clock++;
    }
    /**
     * @brief （连通域）基于DFS的拓扑排序算法
     * @return true v及其后代可以拓扑排序
     * @return false v及其后代不可以拓扑排序
     */
    bool topoSortCD(Rank v, Rank& clock, Stack<Tv>* S)
    {
        status(v) = VStatus::Discoverd;
        dTime(v)  = clock++;
        for (Rank u = firstNbr(v); - 1 < u; u = nextNbr(v, u))
            switch (status(u))
            {
                case VStatus::Undiscovered:
                    type(v, u) = EType::Tree;
                    parent(u)  = v;
                    if (!topoSortCD(u, clock, S))
                        return false; // u及其后代存在后向边则不可拓扑排序，递归返回false
                    break;

                case VStatus::Discoverd:
                    type(v, u) = EType::Backward;
                    return false; // v及其后代存在后向边则不可拓扑排序

                case VStatus::Visited:
                    type(v, u) = (dTime(v) < dTime(u)) ? EType::Forward : EType::Cross;
                    break;
            }
        S->push(vertex(v));
        status(v) = VStatus::Visited;
        fTime(v)  = clock++;
        return true; // v及其后代可以拓扑排序
    }
#define hca(x) (fTime(x))
    // （连通域）基于DFS的双连通分量分解算法
    void bccCD(Rank v, Rank& clock, Stack<Rank>& S)
    {
        hca(v) = dTime(v) = clock++;
        status(v)         = VStatus::Discoverd;
        S.push(v);
        for (Rank u = firstNbr(v); - 1 < u; u = nextNbr(v, u))
            switch (status(u))
            {
                case VStatus::Undiscovered:
                    type(v, u) = EType::Tree;
                    parent(u)  = v;
                    bccCD(u, clock, S);
                    if (hca(u) < dTime(v)) // 递归回来后开始审问子节点
                        hca(v) = std::min(hca(v), hca(u));
                    else // 定制算法，此处为简单的打印
                    {
                        Rank temp;
                        do
                        {
                            temp = S.pop();
                            printf("%d ", temp);
                        } while (u != temp);
                        printf("%d\n", parent(u));
                    }
                    break;

                case VStatus::Discoverd:
                    type(v, u) = EType::Backward;
                    // if (u != parent(v))// 不影响
                    hca(v) = std::min(hca(v), dTime(u));

                case VStatus::Visited:
                    type(v, u) = (dTime(v) < dTime(u)) ? EType::Forward : EType::Cross;
                    break;
            }
        status(v) = VStatus::Visited;
    }
#undef hca
    // （连通域）优先级搜索框架
    template<typename PrioUpdater, typename VST>
    void pfsCD(Rank v, PrioUpdater prioUpdater, VST& visit)
    {
        priority(v) = 0;
        visit(v);
        parent(v) = -1;
        status(v) = VStatus::Visited;
        while (true)
        {
            for (Rank u = firstNbr(v); - 1 < u; u = nextNbr(v, u))
                prioUpdater(this, v, u);
            for (Rank shortest = INT_MAX, u = 0; u < n; u++)
                if (status(u) == VStatus::Undiscovered)
                    if (shortest > priority(u))
                    {
                        shortest = priority(u);
                        v        = u;
                    }
            if (status(v) == VStatus::Visited)
                break;
            visit(v);
            status(v)          = VStatus::Visited;
            type(parent(v), v) = EType::Tree;
        }
    }
};
