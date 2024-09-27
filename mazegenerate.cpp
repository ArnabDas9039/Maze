#include<iostream>
#include<fstream>
#include<list>
#include<vector>
#include<map>
#include<queue>
#include<stack>
using namespace std;

class Node{
public:
    int x, y, m;
    // (x, y) is the coordinate and m is the manhattan distance from the goal
};

struct CompareH {
    bool operator()(Node* a, Node* b){
        return a->m > b->m;
    }
};

struct CompareG {
    bool operator()(Node* a, Node* b){
        return (a->m + a->x + a->y) > (b->m + b->x + b->y);
    }
};

class Graph{
public:
    Node* start, * end;
    map<Node*, list<Node*> > adj;
    // constructor
    Graph(){
    }

    int graphsize(){
        return adj.size();
    }

    Node* getVertex(Node n){
        for(auto i : adj){
            if(i.first->x == n.x && i.first->y == n.y){
                return i.first;
            }
        }
    }

    void createvertex(Node n){
        Node* vertex = new Node(n);
        adj.insert({ vertex, list<Node*>() });
        // cout << "createvertex (" << n.x << ", " << n.y << ")" << endl;
    }

    void addedges(Node* src, Node* dest){
        // cout << "addedges (" << src.x << ", " << src.y << ") ->";
        // cout << " (" << dest.x << ", " << dest.y << ")" << endl;
        for(auto i : adj){
            if(i.first->x == src->x && i.first->y == src->y){
                src = i.first;
            }
            if(i.first->x == dest->x && i.first->y == dest->y){
                dest = i.first;
            }
        }
        adj[src].push_back(dest);
        adj[dest].push_back(src);
    }

    void adddirection(Node* src, Node* dest){
        // cout << "adddirection (" << src.x << ", " << src.y << ") ->";
        // cout << " (" << dest.x << ", " << dest.y << ")" << endl;
        for(auto i : adj){
            if(i.first->x == src->x && i.first->y == src->y){
                src = i.first;
            }
            if(i.first->x == dest->x && i.first->y == dest->y){
                dest = i.first;
            }
        }
        adj[src].push_back(dest);
    }

    void deletedirection(Node* src, Node* dest){

    }

    void display(){
        for(auto i : adj){
            cout << "(" << i.first->x << ", " << i.first->y << ") -> ";
            for(auto p : i.second){
                cout << "(" << p->x << ", " << p->y << ") => ";
            }
            cout << endl;
        }
    }

    void MST_queue(Node* start, Graph* maze){
        queue<Node*> QueueFrontier;
        map<Node*, bool> visit;
        list<Node*> MST;
        map<Node*, bool> pushed;

        QueueFrontier.push(start);
        while(!QueueFrontier.empty()){
            Node* value = QueueFrontier.front();
            QueueFrontier.pop();
            if(!visit[value]){
                MST.push_back(value);
                visit[value] = true;
                for(auto i : adj[value]){
                    QueueFrontier.push(i);
                    if(!pushed[i]){
                        maze->adddirection(maze->getVertex(*value), i);
                        pushed[i] = true;
                    }
                }
            }
        }

        for(auto i : MST){
            cout << "(" << i->x << ", " << i->y << ") ->";
        }
        cout << endl;
    }

    void MST_stack(Node* start, Graph* maze){
        stack<Node*> StackFrontier;
        map<Node*, bool> visit;
        list<Node*> MST;
        map<Node*, bool> pushed;

        StackFrontier.push(start);
        while(!StackFrontier.empty()){
            Node* value = StackFrontier.top();
            StackFrontier.pop();
            if(!visit[value]){
                MST.push_back(value);
                visit[value] = true;
                for(auto i : adj[value]){
                    StackFrontier.push(i);
                    if(!pushed[i]){
                        maze->adddirection(maze->getVertex(*value), i);
                        pushed[i] = true;
                    }
                }
            }
        }

        for(auto i : MST){
            cout << "(" << i->x << ", " << i->y << ") ->";
        }
        cout << endl;
    }

    void dfs(Node* start){
        stack<Node*> StackFrontier;
        map<Node*, bool> visit;
        int state = 0;

        StackFrontier.push(start);
        while(!visit[this->end]){
            if(visit.empty()){
                cout << "No solution" << endl;
                return;
            }
            Node* value = StackFrontier.top();
            StackFrontier.pop();
            if(!visit[value]){
                cout << "(" << value->x << ", " << value->y << ") ->";
                state++;
                visit[value] = true;
                for(auto i : adj[value]){
                    StackFrontier.push(i);
                }
            }
        }
        cout << "States explored: " << state << endl;
    }

    void bfs(Node* start){
        queue<Node*> QueueFrontier;
        map<Node*, bool> visit;
        int size = 0;

        QueueFrontier.push(start);
        while(!visit[this->end]){
            Node* value = QueueFrontier.front();
            QueueFrontier.pop();
            if(!visit[value]){
                cout << "(" << value->x << ", " << value->y << ") ->";
                size++;
                visit[value] = true;
                for(auto i : adj[value]){
                    QueueFrontier.push(i);
                }
            }
        }
        cout << "States explored: " << size << endl;
    }

};
int main(int argc, char* argv[]){
    Graph grid, maze;

    // CLI to filename
    if(argc != 2){
        cout << "Usage: a.exe maze.txt" << endl;
        return 1;
    }

    // Vertex
    for(int i = 1; i < 6; i += 2){
        for(int j = 1; j < 6; j += 2){
            Node n = { i, j, 0 };
            grid.createvertex(n);
            maze.createvertex(n);
            if(i == 1 && j == 1){
                grid.start = grid.getVertex(n);
                maze.start = maze.getVertex(n);
            }
            if(i == 5 && j == 5){
                grid.end = grid.getVertex(n);
                maze.end = maze.getVertex(n);
            }
        }
    }

    // Calls to check
    cout << grid.graphsize() << endl;
    grid.display();

    // Edge
    for(auto i : grid.adj){
        Node src = { i.first->x, i.first->y }, dest;
        if(src.x + 2 < 6){
            dest = { src.x + 2, src.y, 0 };
            grid.adddirection(&src, &dest);
        }
        if(src.y + 2 < 6){
            dest = { src.x, src.y + 2, 0 };
            grid.adddirection(&src, &dest);
        }
        if(src.x - 2 > -1){
            dest = { src.x - 2, src.y, 0 };
            grid.adddirection(&src, &dest);
        }
        if(src.y - 2 > -1){
            dest = { src.x, src.y - 2, 0 };
            grid.adddirection(&src, &dest);
        }
    }

    // Calls to check
    cout << grid.graphsize() << endl;
    grid.display();

    // Heuristic
    for(auto i : maze.adj){
        i.first->m = abs(maze.end->x - i.first->x) + abs(maze.end->y - i.first->y);
    }

    // Generate
    grid.MST_stack(grid.start, &maze);
    cout << maze.graphsize() << endl;
    maze.display();

    // Solve
    maze.dfs(maze.start);
    maze.bfs(maze.start);

    cout << "Start is: (" << maze.start->x << ", " << maze.start->y << ")" << endl;
    cout << "End is: (" << maze.end->x << ", " << maze.end->y << ")" << endl;

    char data[6][6];
    for(int i = 0; i <= 6; i++){
        for(int j = 0; j <= 6; j++){
            data[i][j] = '#';
        }
    }

    for(auto vertex : maze.adj){
        data[vertex.first->x][vertex.first->y] = ' ';
        for(auto value : vertex.second){
            if(value->x - vertex.first->x == 2 && value->y - vertex.first->y == 0){
                data[vertex.first->x + 1][vertex.first->y] = ' ';
            }
            if(value->x - vertex.first->x == 0 && value->y - vertex.first->y == 2){
                data[vertex.first->x][vertex.first->y + 1] = ' ';
            }
            if(value->x - vertex.first->x == -2 && value->y - vertex.first->y == 0){
                data[vertex.first->x - 1][vertex.first->y] = ' ';
            }
            if(value->x - vertex.first->x == 0 && value->y - vertex.first->y == -2){
                data[vertex.first->x][vertex.first->y - 1] = ' ';
            }
        }
    }

    data[1][1] = 'A';
    data[5][5] = 'B';

    for(int i = 0; i <= 6; i++){
        for(int j = 0; j <= 6; j++){
            cout << data[i][j];
        }
        cout << endl;
    }

    ofstream file(argv[1]);

    if(!file.is_open()){
        cout << "Couldn't open file" << endl;
        return 1;
    }


    // 2D vector to file
    for(int i = 0; i <= 6; i++){
        for(int j = 0; j <= 6; j++){
            file.put(data[i][j]);
        }
        file.put('\n');
    }

    file.close();

    return 0;
}