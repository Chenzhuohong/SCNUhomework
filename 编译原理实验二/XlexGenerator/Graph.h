#ifndef GRAPH_H
#define GRAPH_H

#include <QChar>

struct Edge {
    int dest;
    QString* value;
    Edge* next;
    Edge(int dst = 0, QString* val = NULL, Edge* nxt = NULL) {
        dest = dst;
        value = val;
        next = nxt;
    }
};

struct Vertex {
    int num;
    Edge* first;
    Vertex* link;
    Vertex(int n = 0, Edge* fir = NULL, Vertex* lnk = NULL) {
        num = n;
        first = fir;
        link = lnk;
    }
};

class Graph {
public:
    Graph();
    Vertex* getFirstVertext();
    int getNumberOfVertex();
    void clear();
    Vertex* getVertexByNum(int num);
    void insertVertex(int num);
    void insertEdge(int start, int end, QString* val);
    int maxEdgesOfVertex();


private:
    Vertex* firstVertex;
    int numberOfVertex;

};


#endif // GRAPH_H
