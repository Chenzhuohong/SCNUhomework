#include "Graph.h"

Graph::Graph()
{
    firstVertex = NULL;
    numberOfVertex = 0;
}

Vertex* Graph::getFirstVertext() {
    return firstVertex;
}

int Graph::getNumberOfVertex() {
    return numberOfVertex;
}

Vertex* Graph::getVertexByNum(int num) {
    Vertex* p = firstVertex;
    while(p != NULL) {
        if(p->num == num) {
            return p;
        }
        p = p->link;
    }
    return NULL;
}

void Graph::clear() {
    firstVertex = NULL;
    numberOfVertex = 0;
}

void Graph::insertVertex(int num) {
    if(firstVertex == NULL) {
        firstVertex = new Vertex(num);
        numberOfVertex++;
    }
    else {
        Vertex* p = firstVertex;
        Vertex* pre = firstVertex;
        while(p != NULL) {
            if(p->num == num) {
                return;
            }
            pre = p;
            p = p->link;
        }
        pre->link = new Vertex(num);
        numberOfVertex++;
    }
}

void Graph::insertEdge(int start, int end, QString* val) {
    Vertex* startVertex = getVertexByNum(start);
    Vertex* endVertex = getVertexByNum(end);
    if(startVertex != NULL && endVertex != NULL) {
        Edge* newEdge = new Edge(end, val, startVertex->first);
        startVertex->first = newEdge;
    }
}

int Graph::maxEdgesOfVertex() {
    int max = 0;
    Vertex* p1 = firstVertex;
    while(p1 != NULL) {
        int cnt = 0;
        Edge* p2 = p1->first;
        while(p2 != NULL) {
            cnt++;
            p2 = p2->next;
        }
        if(cnt > max) {
            max = cnt;
        }
        p1 = p1->link;
    }
    return max;
}

