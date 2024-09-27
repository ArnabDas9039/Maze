// #include<bits/stdc++.h>
#include<iostream>
#include<fstream>
#include<vector>
#include<map>
#include<list>
#include<stack>
#include<queue>
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
private:
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

    void createVertex(Node n){
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

    void display(){
        for(auto i : adj){
            cout << "(" << i.first->x << ", " << i.first->y << ") -> ";
            for(auto p : i.second){
                cout << "(" << p->x << ", " << p->y << ") => ";
            }
            cout << endl;
        }
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

    void gbfs(Node* start){
        priority_queue<Node*, vector<Node*>, CompareH> HFrontier;
        map<Node*, bool> visit;
        int size = 0;

        HFrontier.push(start);
        while(!visit[this->end]){
            Node* value = HFrontier.top();
            HFrontier.pop();
            if(!visit[value]){
                cout << "(" << value->x << ", " << value->y << ", " << value->m << ") ->";
                size++;
                visit[value] = true;
                for(auto i : adj[value]){
                    HFrontier.push(i);
                }
            }
        }
        cout << "States explored: " << size << endl;
    }

    void ass(Node* start){
        priority_queue<Node*, vector<Node*>, CompareG> GFrontier;
        map<Node*, bool> visit;
        int size = 0;

        GFrontier.push(start);
        while(!visit[this->end]){
            Node* value = GFrontier.top();
            GFrontier.pop();
            if(!visit[value]){
                cout << "(" << value->x << ", " << value->y << ", " << value->m << ") ->";
                size++;
                visit[value] = true;
                for(auto i : adj[value]){
                    GFrontier.push(i);
                }
            }
        }
        cout << "States explored: " << size << endl;
    }
};

int main(int argc, char* argv[]){

    // CLI to filename
    if(argc != 2){
        cout << "Usage: a.exe maze.txt" << endl;
        return 1;
    }

    ifstream file(argv[1]);
    // Initialize Graph
    Graph maze;

    if(!file.is_open()){
        cout << "Couldn't open file" << endl;
        return 1;
    }

    // file to 2D vector
    vector<vector<char> > data;
    string line;
    while(getline(file, line)){
        vector<char> row;
        for(int i = 0; line[i] != '\0'; i++){
            row.push_back(line[i]);
        }
        data.push_back(row);
    }
    file.close();

    // printf("\nMaze is: %d %d\n", r, c);
    for(int i = 0; i < data.size(); i++){
        for(int j = 0; j < data[i].size(); j++){
            cout << data[i][j];
        }
        cout << endl;
    }

    // Vertex
    for(int i = 0; i < data.size(); i++){
        for(int j = 0; j < data[i].size(); j++){
            // cout << "At()" << i << ", " << j << endl;
            if(data[i][j] == 'A' || data[i][j] == ' ' || data[i][j] == 'B'){
                Node n = { i, j, 0 };
                maze.createVertex(n);
                if(data[i][j] == 'A')
                    maze.start = maze.getVertex(n);
                if(data[i][j] == 'B')
                    maze.end = maze.getVertex(n);
            }
        }
    }
    
    // Calls to check
    // cout << maze.graphsize() << endl;
    // maze.display();

    // Edges
    for(auto i : maze.adj){
        Node src = { i.first->x, i.first->y, 0 }, dest;
        if(data[src.x + 1][src.y] == ' ' || data[src.x + 1][src.y] == 'B'){
            dest = { src.x + 1, src.y, 0 };
            maze.adddirection(&src, &dest);
        }
        if(data[src.x][src.y + 1] == ' ' || data[src.x][src.y + 1] == 'B'){
            dest = { src.x, src.y + 1, 0 };
            maze.adddirection(&src, &dest);
        }
        if(data[src.x - 1][src.y] == ' ' || data[src.x - 1][src.y] == 'B'){
            dest = { src.x - 1, src.y, 0 };
            maze.adddirection(&src, &dest);
        }
        if(data[src.x][src.y - 1] == ' ' || data[src.x][src.y - 1] == 'B'){
            dest = { src.x, src.y - 1, 0 };
            maze.adddirection(&src, &dest);
        }
    }
    
    // Calls to check
    cout << maze.graphsize() << endl;
    maze.display();

    // Heuristic
    for(auto i : maze.adj){
        i.first->m = abs(maze.end->x - i.first->x) + abs(maze.end->y - i.first->y);
    }

    // solve
    maze.dfs(maze.start);
    cout << endl;
    maze.bfs(maze.start);
    cout << endl;
    maze.gbfs(maze.start);
    cout << endl;
    maze.ass(maze.start);

    return 0;
}