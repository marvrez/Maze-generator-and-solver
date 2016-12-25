// Library for finding path from  green to red node
#ifndef pathfinder_h
#define pathfinder_h

#include "maze.h"

#include <iostream>
#include <vector>
#include <queue>

//BFS
void findPath(std::vector<std::vector<ii> > &graph,int startNode, int endNode, std::vector<int> &path)
{
    std::vector<int> prev(graph.size(),-1);
    std::vector<bool> visited(graph.size(), false);
    
    std::queue<ii> q;
    visited[endNode] = true;
    q.push(std::make_pair(endNode,0));
    
    while(!q.empty())
    {
        ii par = q.front();
        int node = par.first, dist = par.second;
        q.pop();
        
        if (node == startNode)
        {
            path = prev;
            std::cout << "Steps taken: " << dist << "\n";
            return;
        }
        for(int i = 0; i < graph[node].size(); ++i)
        {
            if(!visited[graph[node][i].first])
            {
                q.push(std::make_pair(graph[node][i].first,dist+1));
                visited[graph[node][i].first] = true;
                prev[graph[node][i].first] = node;
            }
        }
        
    }
}

void printPath(std::vector<int> prev,int endNode)
{
    while(prev[endNode] != -1)
    {
        std::cout << prev[endNode] << " ";
        endNode = prev[endNode];
    }
    std::cout << "\n";
}

#endif /* pathfinder_h */
