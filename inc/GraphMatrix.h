#pragma once

#include "Vector.h"
#include "Graph.h"

// 邻接矩阵
template <typename Tv, typename Te>
class GraphMatrix : public Graph<Tv, Te>
{
public:
    using Graph<Tv, Te>::Graph;
    ~GraphMatrix()
    {
        for (Rank v = 0; v < this->n; v++)
            for (Rank u = 0; u < this->n; u++)
                delete E[v][u];
    }
    // 顶点数据
    virtual Tv &vertex(Rank v) { return V[v].data; }
    // 顶点入度
    virtual Rank inDegree(Rank v) { return V[v].inDegree; }
    // 顶点出度
    virtual Rank outDegree(Rank v) { return V[v].outDegree; }
    // 顶点状态
    virtual VStatus &status(Rank v) { return V[v].status; }
    // 顶点的时间标签dTime
    virtual Rank &dTime(Rank v) { return V[v].dTime; }
    // 顶点的时间标签fTime
    virtual Rank &fTime(Rank v) { return V[v].fTime; }
    // 顶点在遍历树中的父亲
    virtual Rank &parent(Rank v) { return V[v].parent; }
    // 顶点在遍历树中的优先级
    virtual Rank &priority(Rank v) { return V[v].priority; }
    // 顶点的首个邻接顶点
    virtual Rank firstNbr(Rank v) { return nextNbr(v, this->n); }
    // 顶点的下一个邻接顶点
    virtual Rank nextNbr(Rank v, Rank u)
    {
        while ((-1 < u) && !(exists(v, --u)))
            ;
        return u;
    }
    // 插入顶点，返回编号
    virtual Rank insert(const Tv &vertex)
    {
        for (Rank u = 0; u < this->n; u++) // 插入列
            E[u].insert(nullptr);
        E.insert(Vector<Edge<Te> *>(nullptr, ++this->n)); // 插入行
        return V.insert(Vertex<Tv>(vertex));
    }
    // 删除顶点及其关联边，返回该顶点信息
    virtual Tv remove(Rank v)
    {
        for (Rank u = 0; u < this->n; u++) // 删除行
            if (exists(v, u))
            {
                delete E[v][u];
                V[u].inDegree--;
                this->e--;
            }
        E.remove(v);
        this->n--;
        Tv vBak = vertex(v);
        V.remove(v);
        for (Rank u = 0; u < this->n; u++) // 删除列
            if (Edge<Te> *x = E[u].remove(v))
            {
                delete x;
                V[u].outDegree--;
                this->e--;
            }
        return vBak;
    }
    // 边数据
    virtual Te &edge(Rank v, Rank u) { return E[v][u]->data; }
    // 边权重
    virtual Rank &weight(Rank v, Rank u) { return E[v][u]->weight; }
    // 边类型
    virtual EType &type(Rank v, Rank u) { return E[v][u]->type; }
    // 边(v, u)是否存在
    virtual bool exists(Rank v, Rank u) { return (v < this->n) && (u < this->n) && E[v][u] != nullptr; }
    // 在两个顶点之间插入指定权重的边
    virtual void insert(const Te &edge, Rank w, Rank v, Rank u)
    {
        if (exists(v, u))
            return;
        E[v][u] = new Edge<Te>(edge, w);
        this->e++, V[v].outDegree++, V[v].inDegree++;
    }
    // 删除一对顶点的边，返回该边的信息
    virtual Te remove(Rank v, Rank u)
    {
        if (!exists(v, u))
            return (Te)0;
        Te eBak = edge(v, u);
        delete E[v][u];
        E[v][u] = nullptr;
        this->e--, V[v].outDegree--, V[v].inDegree--;
        return eBak;
    }

private:
    Vector<Vertex<Tv>> V;         // 顶点集（向量）
    Vector<Vector<Edge<Te> *>> E; // 边集（邻接矩阵）
};