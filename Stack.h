//
// Created by brett on 06/02/18.
//

#ifndef MAZESOLVER_V2_STACK_H
#define MAZESOLVER_V2_STACK_H


#include <ostream>
#include "StackNode.h"

class Stack {
private:
    StackNode *_top;

public:
    Stack();
    virtual ~Stack();

    void Push(Point data);
    Point Peek();
    void Pop();
    bool empty();

    Point getTop();
};

#endif //MAZESOLVER_V2_STACK_H
