/* 
 * File:   graph.cpp
 * Author: njin
 * 
 * Created on October 10, 2009, 6:58 PM
 */

#include "graph.h"

//destructor
graph::~graph()
{
    for (int i = 0; i < adjList.size(); i++)
        delete adjList[i];
}

