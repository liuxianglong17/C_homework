#ifndef AGRAPH_H_INCLUDED
#define AGRAPH_H_INCLUDED
#include <stdio.h>
#include <malloc.h>

#define OK 1
#define ERROR 0
typedef int Status;

#define MAX_VERTEX_NUM 30
typedef char ElemType;
typedef struct node {
    int vindex; //邻接点在头结点数组中的位置(下标)
    struct node *next;//指向下一个表结点
} NodeLink; // 表结点类型定义
typedef struct {
    // 图的顶点数、边数、种类标志
    int vexnum,edgenum,kind;
    struct {
        ElemType vertex;
        NodeLink *first;// 指向第一个表结点
    }v[MAX_VERTEX_NUM];
}AGraph;

void CreateGraph(AGraph *g,int n,int m);  //n顶点数，m图的种类
void ListGraph(AGraph *g);
int LocateVex(AGraph *g,ElemType u);
ElemType GetVex(AGraph *g,int v);

#endif // AGRAPH_H_INCLUDED
