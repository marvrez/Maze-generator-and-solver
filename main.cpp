#include <GLFW/glfw3.h>

#include "maze.h"
#include "pathfinder.h"
#include <iostream>
#include <vector>
#include <ctime>

Maze maze(7,7);
vector<int> path;
int cur;

float px = 100.f;
float nx = -px;
float py = 100.f;
float ny = -px;

/////////////////////PROTOTYPES/////////////////////////////////////

static void error_callback(int error, const char* description);
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void initOpenGL(void);
void reshape(GLFWwindow *window,int ws, int h);
void drawMiniSquare(float x, float y, float dx, float dy, const Node& node, int idx);
void paint(void);
int backtrack(int prev);
/////////////////////MAIN/////////////////////////////////////

int main(int argc,char* argv[])
{
    glfwInit();
    
    GLFWwindow* window = glfwCreateWindow(640, 480, "Maze generator", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        std::cout << "Error: Window could not be created." << "\n";
        exit(EXIT_FAILURE);
    }
    
    //callbacks
    glfwSetErrorCallback(error_callback);
    glfwSetKeyCallback(window, key_callback);
    //glfwSetWindowSizeCallback(window, reshape);
    glfwSetFramebufferSizeCallback(window, reshape); //for OSX
    
    glfwMakeContextCurrent(window);
    
    bool changedTitle = false;
    vector<vector<ii> > edges;
    cur = maze.endNode;
    
    while(!glfwWindowShouldClose(window))
    {
        initOpenGL();
        paint();
        if(!maze.Generate())
            paint();
        else if(!changedTitle)
        {
            glfwSetWindowTitle(window, "Maze Generator - Finished generating maze!");
            changedTitle = true;
            edges = maze.createNghbrList();
            findPath(edges, maze.startNode, maze.endNode, path);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glfwTerminate();
    return 0;
}

/////////////////////FUNCTIONS/////////////////////////////////////

static void error_callback(int error, const char* description)
{
    fputs(description, stderr);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

void initOpenGL()
{
    glClearColor(0, 0, 0, 0);
    glEnable(GL_LINE_SMOOTH);
    glShadeModel(GL_SMOOTH);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void reshape(GLFWwindow *window,int w, int h)
{
    glClearColor(0.5, 0.5, 1.0, 1.0);
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glOrtho(nx,px,ny,py,-1,1);
}

void drawMiniSquare(float x, float y, float dx, float dy, const Node& node,int idx)
{
    if (!node.visited)
    {
        if (node.active)
            glColor3f(1.0f, 1.0f, 0.5f);
        else
            glColor3f(0.4f, 0.4f, 0.4f);
        if (node.isStartNode)
            glColor3f(0, 0.7f, 0);
    }
    else
    {
        if (node.active)
            glColor3f(1.f, 0.8f, 0.8f);
        else
            glColor3f(1.0f, 1.0f, 1.0f);
        if (node.isStartNode)
            glColor3f(0, 0.8f, 0);
        if (node.isEndNode)
            glColor3f(0.8f, 0, 0);
        
    }
    if (maze.isInPath[idx])
        glColor3f(0.5f, 0.5f, 1.f);
    //cout << x << " " << y << " " << node.isStartNode << endl;
    glBegin(GL_QUADS);
    glVertex2f(x,y);
    glVertex2f(x+dx, y);
    glVertex2f(x+dx, y-dy);
    glVertex2f(x, y-dy);
    glEnd();
 
    glColor3f(0, 0, 0);
    glBegin(GL_LINES);
    if (!node.tEdge)
    {
        glVertex2f(x, y);
        glVertex2f(x+dx, y);
    }
    if (!node.rEdge)
    {
        glVertex2f(x+dx, y);
        glVertex2f(x+dx, y-dy);
    }
    if (!node.bEdge)
    {
        glVertex2f(x+dx, y-dy);
        glVertex2f(x,	 y-dy);
        
    }
    if (!node.lEdge)
    {
        glVertex2f(x, y-dy);
        glVertex2f(x, y);
    }
    glEnd();
}

void paint(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glOrtho(nx-0.1f, px+0.1f, ny-0.1f, py+0.1f, -1, 1);
    
    glColor3f(1, 1, 1);
    glBegin(GL_QUADS);
    glVertex2f(nx, ny);
    glVertex2f(nx, py);
    glVertex2f(px, py);
    glVertex2f(px, ny);
    glEnd();
    
    float _px = nx;
    float _py = py;
    
    glLineWidth(1.0);
    glPointSize(2.0);
    float step_x = (px-nx)/maze.Width();
    float step_y = (py-ny)/maze.Height();
    for(int i = 0; i < maze.Height(); ++i)
    {
        for(int j = 0; j < maze.Width(); ++j)
        {
            Node n = maze.getNode(j,i);
            int node = (i * maze.Height()) + j;
            if(!path.empty())
            {
                int prev = backtrack(cur);
                if (prev == node && prev != maze.startNode)
                {
                    cur = prev;
                
                    //printPath(path,cur);
                    maze.isInPath[node] = true;
                }
            }
            /*
            if (n.invalid)
            {
                n.print();
                glBegin(GL_POINTS);
                glColor3f(1, 0, 0);
                glVertex2f(_px + (step_x/2), _py - (step_y/2));
                glEnd();
            }*/
            drawMiniSquare(_px, _py, step_x, step_y, n, node);
            _px +=step_x;
        }
        _px = nx;
        _py -= step_y;
    }
}

int backtrack(int cur)
{
    //cout << path[cur] << endl;
    if(path[cur] != -1)
        cur = path[cur];
    return cur;
}

