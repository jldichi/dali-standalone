#ifndef IXLIST_H
#define IXLIST_H


#include <iostream>
#include <ideafix.h>

class node {
   public:
    node(int v, String s,  node *sig = NULL)
    {
       intValue = v;
       stringValue = s;
       next = sig;
    }

   private:
    int intValue;
    String stringValue;
    node *next;
        
   friend class IxList;
};
 
typedef node *pnode;
 
class IxList {
   public:
    IxList() { firstNode = actualNode = NULL; }
    ~IxList();
    
    void insert(int v, String s);
    void del(int v);
    bool isEmpty() { return firstNode == NULL; } 
    void next() { if(actualNode) actualNode = actualNode->next; }
    void first() { actualNode = firstNode; }
    void last() { first(); if(!isEmpty()) while(actualNode->next) next(); }
    bool actual() { return actualNode != NULL; }
    int actualIntValue() { return actualNode->intValue; }
    int actualStringValue() { return actualNode->stringValue; }
    void newList(void);
    
   private:
    pnode firstNode;
    pnode actualNode;
};
 
#endif

