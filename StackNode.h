//
// Created by brett on 06/02/18.
//

#ifndef MAZESOLVER_V2_STACKNODE_H
#define MAZESOLVER_V2_STACKNODE_H

/*
 * Custom Structure to hold data about positions inside of the Grid and Stack.
 */
struct Point {
public:
    int col {0};
    int row {1};

    Point() = default;
    Point(int row, int col) : col(row), row(col) {}
    bool equals(Point p);
};

class StackNode {
private:
    Point _data;
    StackNode *_next;

public:
    StackNode(Point data, StackNode* next);
    virtual ~StackNode();

    Point getPoint();
    void setPoint(Point point);

    StackNode* getNext();
    void setNext(StackNode *next);
};

#endif //MAZESOLVER_V2_STACKNODE_H
