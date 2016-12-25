#ifndef maze_h
#define maze_h

#include <vector>
#include <cstdlib>

#include <unistd.h>

#include <iostream>

typedef unsigned char byte;
typedef std::pair<int,int> ii;
typedef unsigned int uint32_t;

typedef struct
{
    int node, edge, oedge; //vertex, first edge, opposite...
} PossibleNode;

typedef struct
{
    bool visited;
    bool active;
    bool lEdge,rEdge,tEdge,bEdge; //left,right,top,bottom
    bool invalid;
    bool isStartNode = false;
    bool isEndNode = false;
    bool isPath = false;
    void print()
    {
        if (invalid) { std::cout << "Invalid!" << "\n"; return; }
        std::cout << "Visited: " << visited << "\n";
        std::cout << "Left   : " << lEdge << "\n";
        std::cout << "Right  : " << rEdge << "\n";
        std::cout << "Top    : " << tEdge << "\n";
        std::cout << "Bottom : " << bEdge << "\n";
    }
    
    int weigth;
} Node;

class Maze
{
public:
    Maze(const uint32_t sixex, const uint32_t sizey);
    ~Maze(); //destroy maze
    
    bool Generate(); //iterative generation of maze
    
    //metainfo about maze
    int Width();
    int Height();
    
    Node getNode(int x, int y); //get node at specific coordinate of maze.
    
    std::vector<std::vector<ii> > createNghbrList();
    
    int startNode, endNode;
    std::vector<bool> isInPath;
    
private:
    int randomNode(); //get index of random node from maze
    
    //return -1 if neighbor does not exist.
    int getLeftNghbr(int node);
    int getRightNghbr(int node);
    int getTopNghbr(int node);
    int getBottomNghbr(int node);
    int getEndNode();
    
    uint32_t size_x; // Maze width
    uint32_t size_y; // Maze height
    uint32_t len;         //  (size_x * size_y), length of maze
    std::vector<byte> maze;      // all nodes in the maze
    
    std::vector<int> activeset; // the active nodes in the maze
    int index; // used for storing state in "Generate()"
    int node;  // used for storing state in "Generate()"
    
    // node flags
    static const byte VISITED	  = 0x80;
    static const byte ACTIVE	  = 0x40;
    static const byte LEFT_EDGE   = 0x08;
    static const byte RIGHT_EDGE  = 0x04;
    static const byte TOP_EDGE    = 0x02;
    static const byte BOTTOM_EDGE = 0x01;
    
};

#endif /* maze_h */
