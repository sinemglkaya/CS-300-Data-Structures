//
// Created by Sinem Kaya on 4.11.2023.
//

#include "MyStack.h"



template <class Object>
void MyStack<Object>::push(Object & value)
{
    if (this->isEmpty()) {//if it is empty, add to the head
        topStack = new Node<Object>(value,nullptr);

    }
    else {//if it is not empty, add to the beginning, update head
        Node<Object>* temp = new Node<Object>(value, topStack);
        topStack = temp;
    }
}


template <class Object>
void MyStack<Object>::pop()
{
    if (!this->isEmpty()) {
        Node <Object> * temp = topStack;
        topStack = topStack->next;
        delete temp;
    }

}

template <class Object>
Node<Object> * MyStack <Object> :: top()
{
    return this ->topStack;
}


template <class Object>
bool MyStack<Object>::isEmpty()
{
    if (this->topStack == nullptr) {
        return true;
    }
    return false;
    //return topStack == nullptr;
}





