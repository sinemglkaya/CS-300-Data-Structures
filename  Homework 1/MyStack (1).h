

#ifndef UNTITLED20_MYSTACK_H
#define UNTITLED20_MYSTACK_H

#include <iostream>

using namespace std;


template <class Object>

struct Node
{
    Object & value;
    //Object value;
    Node <Object> * next;
    Node(Object &new_value, Node<Object> *newNext = nullptr) : value(new_value), next(newNext) {}
};

template <class Object>
class MyStack{

public:

    MyStack() : topStack(nullptr) {} // Default constructor
    //~MyStack(); // Destructor to handle memory deallocation
    void push(Object&);//pushes onto stack
    //void push(Object value); // Pushes onto stack
    void pop(); // Pops the element on top
    Node <Object> *top(); // Returns a reference to the top element of the stack
    bool isEmpty(); // Checks if the stack is empty
private:
    Node <Object>* topStack;
};


#endif //UNTITLED20_MYSTACK_H

