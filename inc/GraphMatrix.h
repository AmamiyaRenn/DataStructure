#pragma once

#include "inc/Graph.h"
#include "inc/Macro.h"

// 邻接矩阵
template<typename Tv, typename Te>
class GraphMatrix : public Graph<Tv, Te>
{
public:
    using Graph<Tv, Te>::Graph;
    ~GraphMatrix()
    {
        for (Rank v = 0; v < this->n; v++)
            for (Rank u = 0; u < this->n; u++)
                delete e_set[v][u];
    }
    // 顶点数据
    virtual Tv& vertex(Rank v) { return v_set[v].data; }
    // 顶点入度
    virtual Rank inDegree(Rank v) { return v_set[v].in_degree; }
    // 顶点出度
    virtual Rank outDegree(Rank v) { return v_set[v].out_degree; }
    // 顶点状态
    virtual VStatus& status(Rank v) { return v_set[v].status; }
    // 顶点的时间标签dTime
    virtual Rank& dTime(Rank v) { return v_set[v].d_time; }
    // 顶点的时间标签fTime
    virtual Rank& fTime(Rank v) { return v_set[v].f_time; }
    // 顶点在遍历树中的父亲
    virtual Rank& parent(Rank v) { return v_set[v].parent; }
    // 顶点在遍历树中的优先级
    virtual Rank& priority(Rank v) { return v_set[v].priority; }
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
    virtual Rank insert(const Tv& vertex);
    // 删除顶点及其关联边，返回该顶点信息
    virtual Tv remove(Rank v);
    // 边数据
    virtual Te& edge(Rank v, Rank u) { return e_set[v][u]->data; }
    // 边权重
    virtual Rank& weight(Rank v, Rank u) { return e_set[v][u]->weight; }
    // 边类型
    virtual EType& type(Rank v, Rank u) { return e_set[v][u]->type; }
    // 边(v, u)是否存在
    virtual bool exists(Rank v, Rank u) { return (v < this->n) && (u < this->n) && e_set[v][u] != nullptr; }
    // 在两个顶点之间插入指定权重的边
    virtual void insert(const Te& edge, Rank w, Rank v, Rank u)
    {
        if (exists(v, u))
            return;
        e_set[v][u] = new Edge<Te>(edge, w);
        this->e++, v_set[v].out_degree++, v_set[v].in_degree++;
    }
    // 删除一对顶点的边，返回该边的信息
    virtual Te remove(Rank v, Rank u);

private:
    Vector<Vertex<Tv>>        v_set; // 顶点集（向量）
    Vector<Vector<Edge<Te>*>> e_set; // 边集（邻接矩阵）
};

template<typename Tv, typename Te>
Tv GraphMatrix<Tv, Te>::remove(Rank v)
{
    for (Rank u = 0; u < this->n; u++) // 删除行
        if (exists(v, u))
        {
            delete e_set[v][u];
            v_set[u].in_degree--;
            this->e--;
        }
    e_set.remove(v);
    this->n--;
    Tv v_bak = vertex(v);
    v_set.remove(v);
    for (Rank u = 0; u < this->n; u++) // 删除列
        if (Edge<Te>* x = e_set[u].remove(v))
        {
            delete x;
            v_set[u].out_degree--;
            this->e--;
        }
    return v_bak;
}
template<typename Tv, typename Te>
Rank GraphMatrix<Tv, Te>::insert(const Tv& vertex)
{
    for (Rank u = 0; u < this->n; u++) // 插入列
        e_set[u].insert(nullptr);
    this->n++;
    e_set.insert(Vector<Edge<Te>*>(this->n, this->n, static_cast<Edge<Te>*>(nullptr))); // 插入行
    return v_set.insert(Vertex<Tv>(vertex));
}

template<typename Tv, typename Te>
Te GraphMatrix<Tv, Te>::remove(Rank v, Rank u)
{
    if (!exists(v, u))
        return Te();
    Te e_bak = edge(v, u);
    delete e_set[v][u];
    e_set[v][u] = nullptr;
    this->e--, v_set[v].out_degree--, v_set[v].in_degree--;
    return e_bak;
}