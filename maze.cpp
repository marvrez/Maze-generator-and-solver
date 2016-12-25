#include <ctime>
#include "maze.h"
using namespace std;

Maze::Maze(const uint32_t size_x_, const uint32_t size_y_):
        size_x(size_x_), size_y(size_y_)
{
    len = (size_x * size_y);
    maze = vector<byte>(len,0);
    isInPath = vector<bool>(len,false);
    
    srand(time(0));
    
    int node = randomNode();
    startNode = node;
    endNode = getEndNode();
    activeset.push_back(node);
    
}

Maze::~Maze()
{
    cout << "lata pleb" << "\n";
}

int Maze::Width()
{
    return size_x;
}

int Maze::Height()
{
    return size_y;
}
/* example on 7x7 maze
[  0  1  2  3  4  5  6 ]
[  7  8  9 10 11 12 13 ]
[ 14 15 16 17 18 19 20 ]
[ 21 22 23 24 25 26 27 ]
[ 28 29 30 31 32 33 34 ]
[ 35 36 37 38 39 40 41 ]
[ 42 43 44 45 46 47 48 ]
*/
Node Maze::getNode(int x, int y)
{
    Node node;
    int index = (y * size_y) + x;
    if (index >= len || index < 0)
    {
        node.invalid = true;
        return node;
    }
    node.visited = (maze[index] & VISITED) > 0;
    node.active  = (maze[index] & ACTIVE ) > 0;
    node.lEdge   = (maze[index] & LEFT_EDGE) > 0;
    node.rEdge   = (maze[index] & RIGHT_EDGE) > 0;
    node.tEdge   = (maze[index] & TOP_EDGE) > 0;
    node.bEdge   = (maze[index] & BOTTOM_EDGE) > 0;
    if (index == startNode)
        node.isStartNode = true;
    if (index == endNode)
        node.isEndNode = true;
    node.invalid = false;
    
    return node;
}

int Maze::getEndNode()
{
    int i = rand() % 4;
    int x,y;
    
    switch(i)
    {
        case(0): //top
        {
            x = rand() % size_x, y = 0;
            break;
        }
        case(1)://right
        {
            x = size_x-1, y = rand()% size_y;
            break;
        }
        case(2): //bottom
        {
            x = rand() % size_x, y = size_y-1;
            break;
        }
        case(3)://left
        {
            x = 0, y = rand()% size_y;
            break;
        }
    }
    return (y * size_y) + x;
}


int Maze::randomNode()
{
    srand(time(0));
    int i = rand() % len;
    while((maze[i] & VISITED) > 0)
        i = rand() % len;
    
    maze[i]  |= VISITED;
    return i;
}

int Maze::getLeftNghbr(int node)
{
    int py = (node-1) / size_x;
    int y  = node / size_x;
    if (y!=py)
        return -1;
    return node-1;
}

int Maze::getRightNghbr(int node)
{
    int ny = (node+1)/size_x;
    int y  = node/ size_x;
    if (y!=ny)
        return -1;
    return node+1;
}

int Maze::getTopNghbr(int node)
{
    int tx = node - size_x;
    if (tx < 0)
        return -1;
    return tx;
}

int Maze::getBottomNghbr(int node)
{
    int bx = node + size_x;
    if (bx >= len)
        return -1;
    return bx;
}

vector<vector<ii> > Maze::createNghbrList()
{
    std::vector<vector<ii> > edges(len);

    for(int i = 0; i < maze.size(); ++i)
    {
        if (maze[i] & TOP_EDGE)
            edges[i].push_back(make_pair(getTopNghbr(i),1));
        if (maze[i] & RIGHT_EDGE)
            edges[i].push_back(make_pair(getRightNghbr(i),1));
        if (maze[i] & BOTTOM_EDGE)
            edges[i].push_back(make_pair(getBottomNghbr(i),1));
        if (maze[i] & LEFT_EDGE)
            edges[i].push_back(make_pair(getLeftNghbr(i),1));
        }

    for(int i = 0; i < edges.size(); ++i)
    {
        cout << "Node: " << i <<  " has neighbors ";
        for(int j = 0; j < edges[i].size(); ++j)
        {
            cout << edges[i][j].first << " ";
        }
        cout << endl;
    }
    
    return edges;
}

bool Maze::Generate() //tldr - choose node at random and add it to vector
{
    if (activeset.empty())
        return true;
    
    //choose random node from set
    if (rand() % 2 == 0)
        this->index = rand() % activeset.size();
    else
        this->index = uint32_t(activeset.size()-1);
    
    this->node = activeset[this->index];
    
    int left = getLeftNghbr(this->node);
    int right = getRightNghbr(this->node);
    int top = getTopNghbr(this->node);
    int bottom = getBottomNghbr(this->node);
    
    vector<PossibleNode> choices; //vector of possible nodes
    
    if (left != -1 && (maze[left] & VISITED) == 0)
    {
        PossibleNode pn = {left, RIGHT_EDGE, LEFT_EDGE};
        choices.push_back(pn);
    }
    if (right != -1 && (maze[right] & VISITED) == 0)
    {
        PossibleNode pn = {right, LEFT_EDGE, RIGHT_EDGE};
        choices.push_back(pn);
    }
    if (top != -1 && (maze[top] & VISITED) == 0)
    {
        PossibleNode pn = {top, BOTTOM_EDGE, TOP_EDGE};
        choices.push_back(pn);
    }
    if (bottom != -1 && (maze[bottom] & VISITED) == 0)
    {
        PossibleNode pn = {bottom, TOP_EDGE, BOTTOM_EDGE};
        choices.push_back(pn);
    }
    
    if (choices.empty())
    {
        maze[node] &= ~ACTIVE;
        activeset.erase(activeset.begin()+index);
    }
    else
    {
        int pick = rand() % choices.size();
        PossibleNode other = choices[pick];
        maze[other.node] |= ACTIVE | VISITED | other.edge;
        maze[node] |= other.oedge;
        activeset.push_back(other.node);
    }
    return false;
}
