#ifndef ASTAR_H
#define ASTAR_H

#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <stdlib.h>
#include <math.h>
#include "base.h"
#include "pacman.h"

typedef struct node Node;
typedef struct list List;

struct node {
    int x;
    int y;
    int h; //distanza dalla fine
    int g; //distanza dall'inizio
    int f; //g + h
    Coord parent;
};
struct list {
    Node node;
    List* next;
};

List* newNode(Node node) {
    List* newElement;
    newElement =(List*) malloc(sizeof(List));
    newElement->next = NULL;
    newElement->node = node;
    return newElement;
}

//end of list
List* eol(List* list) {
    List* end = list;
    if (end != 0) {
        while (end->next != NULL) {
            end = end->next;
        }
    }
    return end;
}

List* add(Node node, List* list) {
    List* end = eol(list);
    List* newElement = newNode(node);
    if (end != 0)
        end->next = newElement;
    return list;
}

List* removeNode(Node node, List* list) {
    List* ptr = list;
    List* prev = NULL;
    List* tmp;
    while (ptr != NULL) {
        if (ptr->node.y == node.y && ptr->node.x == node.x) {
            if (prev == NULL) {
                list = list->next;
                free(ptr);
                return list;
            } else {
                tmp = ptr;
                ptr = ptr->next;
                prev->next = ptr;
                free(tmp);
                return list;
            }
        } else {
            prev = ptr;
            ptr = ptr->next;
        }
    }
    return list;
}

void freeList(List* start) {
    List* tmp;
    while (start != NULL) {
        tmp = start->next;
        free(start);
        start = tmp;
    }
}

int len(List* list) {
    List* ptr = list;
    int n = 0;
    while (ptr != NULL) {
        ptr = ptr->next;
        n++;
    }
    return n;
}

int contains(List* list, Node node) {
    while (list != NULL) {
        if (list->node.x == node.x && list->node.y == node.y)
            return 1;
        list = list->next;
    }
    return 0;
}

void printList(List* start) {
  List* ptr = start;
  while (ptr != NULL) {
    printf("x: %d y: %d -> ", ptr->node.x, ptr->node.y);
    ptr = ptr->next;
  }
  printf("/\n\n");
}

int getDistance(Node a, Node b) {
    return abs(b.x - a.x) + abs(b.y - a.y);
}

List* getNeighbours(Node node) {
    List* neighbours = NULL;
    Node newN;
//the node at the left
    newN.x = node.x - 1;
    newN.y = node.y;
    neighbours = newNode(newN);
//the node at the right
    newN.x = node.x + 1;
    newN.y = node.y;
    neighbours = add(newN, neighbours);
//the note at the top
    newN.x = node.x;
    newN.y = node.y - 1;
    neighbours = add(newN, neighbours);
//the node at the bottom
    newN.x = node.x;
    newN.y = node.y + 1;
    neighbours = add(newN, neighbours);

    return neighbours;
}

Node getParent(List* nodes, int x, int y) {
    while (nodes != NULL) {
        if (nodes->node.x == x && nodes->node.y == y)
            return nodes->node;
        nodes = nodes->next;
    }
    return nodes->node;
}

List* getPath(List* nodes, Node a, Node b) {
    List* path = NULL;
    List* end = eol(nodes);
    //start from end till the start to build the path
    Node currentNode = end->node;
    //while the parent is not the end point
    while (currentNode.parent.x != 0 && currentNode.parent.y != 0) {
        //add the current node to the path and get the parent of the current node
        //the current node will be its parent
        if (path == NULL) //first cicle
            path = newNode(currentNode);
        else
            path = add(currentNode, path);
        currentNode = getParent(nodes, currentNode.parent.x, currentNode.parent.y);
    }
    //add the start to the path
    path = add(a, path);
    //visual path
    /*
    end = path;
    while (end != NULL) {
        if (nmove == 0)
            al_draw_circle(end->node.x*CELL+10, end->node.y*CELL+10, 4.0, al_map_rgb(255, 0, 0), 3);
        if (nmove == 1)
            al_draw_circle(end->node.x*CELL+10, end->node.y*CELL+10, 4.0, al_map_rgb(0, 255, 255), 3);
        if (nmove == 2)
            al_draw_circle(end->node.x*CELL+10, end->node.y*CELL+10, 4.0, al_map_rgb(255, 204, 255), 3);
        if (nmove == 3)
            al_draw_circle(end->node.x*CELL+10, end->node.y*CELL+10, 4.0, al_map_rgb(255, 153, 102), 3);

        end = end->next;
    }*/
    freeList(end);
    return path;
}

List* astar(Coord start, Coord target) {
    List* openlist = NULL;
    List* closelist = NULL;
    List* tmp = NULL;
    List* neighbours = NULL;
    Node currentNode;
    Node pac;
    Node en;
//creation of start as a node
    pac.x = start.x/CELL; pac.y = start.y/CELL; pac.parent.x = 0; pac.parent.y = 0;
    en.x = target.x/CELL;   en.y = target.y/CELL;
    pac.h = getDistance(pac, en); pac.g = 0; pac.f = 0;
    en.h = 0; en.g = pac.h; en.f = 0;
//init openlist adding the start node
    openlist = newNode(pac);
//while the openlist is not empty
    while (len(openlist) > 0) {
        //take the current node, it is the node that has the < f in the openlist
        currentNode = openlist->node;
        tmp = openlist;
        while (tmp->next != NULL) {
            if (tmp->node.f < currentNode.f || (tmp->node.f == currentNode.f && tmp->node.h < currentNode.h))
                currentNode = tmp->node;
            tmp = tmp->next;
        }
        //remove the currenNode from the openlist
        openlist = removeNode(currentNode, openlist);
        //put the current node in the closelist
        if (closelist == NULL)
            closelist = newNode(currentNode);
        else
            add(currentNode, closelist);
        //if it is at the end returns the path
        if (currentNode.h == 0) {
            closelist = getPath(closelist, pac, en);
            return closelist;
        }
        //else
        //taking the neighbours
        neighbours = getNeighbours(currentNode);
        //add neighbours to the lists
        while (neighbours != NULL) {
            if (matrix[neighbours->node.y][neighbours->node.x] != 2 && !contains(openlist, neighbours->node) && !contains(closelist, neighbours->node)) {
                neighbours->node.h = getDistance(neighbours->node, en);
                neighbours->node.g = getDistance(neighbours->node, pac);
                neighbours->node.f = neighbours->node.g + neighbours->node.h;
                neighbours->node.parent.x = currentNode.x;
                neighbours->node.parent.y = currentNode.y;
                if (openlist == NULL)
                    openlist = newNode(neighbours->node);
                else
                    openlist = add(neighbours->node, openlist);
            }
            neighbours = neighbours->next;
        }
    }
    printf("no path found.\n");
    return NULL;
}

#endif // ASTAR_H
