#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <fstream>
#include <map>

using namespace std;

int iterations;
int maxIteration = 1000000;
int maxQ;
vector<string> goalState;
string hNum;

class Node {
public:
    vector<string> state;
    int pathCost;
    vector<Node> successorsList;
    vector<string> printList;

    Node() {}

    Node(vector<string> s) {
        state = s;
        pathCost = 0;
    }

    Node(vector<string> s, int currPathCost) {
        state = s;
        pathCost = currPathCost + 1;
    }

    int heuristic() {
        int h = 0;
        if (hNum == "-H1") {
            for (int i = 0; i < state.size(); i++) {
                for (int j = 0; j < state.at(i).size(); j++) {
                    if (j < goalState.at(i).size()) {
                        if (state.at(i).at(j) != goalState.at(i).at(j)) {
                            h++;
                        }
                    }
                }
            }
        } 
        else if (hNum == "-H1") {
            for (int i = 0; i < state.size(); i++) {
                if (state.at(i).size() < goalState.at(i).size()) {
                    h += (goalState.at(i).size() - state.at(i).size());
                }
                for (int j = 0; j < state.at(i).size(); j++) {
                    if (j < goalState.at(i).size()) {
                        if (state.at(i).at(j) != goalState.at(i).at(j)) {
                            h++;
                        }
                    }
                }
            }
        } 
        else if (hNum == "-H3") { //best one DEFAULT
            for (int i = 0; i < state.size(); i++) {
                if (state.at(i).size() < goalState.at(i).size()) {
                    h += (goalState.at(i).size() - state.at(i).size());
                }
                for (int j = 0; j < state.at(i).size(); j++) {
                    if (j < goalState.at(i).size()) {
                        if (state.at(i).at(j) != goalState.at(i).at(j)) {
                            h += (state.at(i).size() - state.at(i).find(state.at(i).at(j)));
                        }                     
                    }
                    if (j != 0) {
                        if (j < goalState.at(i).size()) {
                            if (state.at(i).at(j-1) != goalState.at(i).at(j-1)) {
                                h++;
                            }
                        }    
                    }                    
                }
            }
        }       
        else if (hNum == "-H4") {
            for (int i = 0; i < state.size(); i++) {
                if (state.at(i).size() < goalState.at(i).size()) {
                    h += (goalState.at(i).size() - state.at(i).size());
                }
                for (int j = 0; j < state.at(i).size(); j++) {
                    if (j < goalState.at(i).size()) {
                        if (state.at(i).at(j) != goalState.at(i).at(j)) {
                            h += (state.at(i).size() - state.at(i).find(state.at(i).at(j)));
                        }
                        if (state.at(i).at(j) != goalState.at(i).at(j)) {
                            h++;
                        }                        
                    }
                }
            }
        }             
        else if (hNum == "-H5") {
            for (int i = 0; i < state.size(); i++) {
                for (int j = 0; j < state.at(i).size(); j++) {
                    if (j < goalState.at(i).size()) {
                        if (state.at(i).at(j) != goalState.at(i).at(j)) {
                            h++;
                        }
                    }
                    if (j != 0) {
                        if (j < goalState.at(i).size()) {
                            if (state.at(i).at(j-1) != goalState.at(i).at(j-1)) {
                                h++;
                            }
                        }    
                    }
                
                }
            }
        }                     
        return h;
    }

    void successors() {
        vector<string> success = state;
        for (int i = 0; i < success.size(); i++) {
            if (success.at(i).size() != 0) {
                char topStack = success.at(i).at(success.at(i).size() - 1);
                success.at(i).pop_back();

                for (int j = 0; j < success.size(); j++) {
                    if (j != i) {
                        success.at(j) += topStack;
                        Node node(success, pathCost);
                        for (int i = 0; i < printList.size(); i++) {
                            node.printList.push_back(printList.at(i));
                        }
                        node.printList.push_back(">>>>>>>>>>");
                        node.printList.push_back("Move " + to_string(pathCost) + ", pathcost=" + to_string(pathCost) + ", heuristic=" + to_string(heuristic()) + ", f(n)=g(n)+h(n)=" + to_string(pathCost + heuristic()));   

                        for (int i = 0; i < state.size(); i++) {
                            node.printList.push_back(state.at(i));
                        }

                        successorsList.push_back(node);
                        success.at(j).pop_back();
                    }
                }
            success = state;
            }
        }
    }
};

struct comparator{
    bool operator()(Node node1, Node node2) const {
        return node1.pathCost + node1.heuristic() >= node2.pathCost + node2.heuristic();
    }
};

Node aStar(vector<string> problem, vector<string> goal) {
    Node node(problem);
    priority_queue<Node, vector<Node>, comparator> frontier;
    frontier.push(node);
    map<vector<string>, Node> reached;
    reached[problem] = node;
    iterations = 0;
    maxQ = 0;

    while(!frontier.empty() && iterations < maxIteration) {
        if (frontier.size() > maxQ) {
            maxQ = frontier.size();
        }
        node = frontier.top();
        frontier.pop();
        iterations++;
        
        if (node.state == goal) {
            cout << "success! " << "iter=" << iterations << ", depth=pathcost=" << node.pathCost << ", max_Qsize=" << maxQ << endl;
            return node;
        }
        node.successors();
        cout << "iter=" << iterations << ", depth=pathcost=" << node.pathCost << ", heuristic=" << node.heuristic() << ", score=" << (node.pathCost+node.heuristic()) << " children=" << node.successorsList.size() << ", Qsize=" << maxQ << endl;
        for (int i = 0; i < node.successorsList.size(); i++) {
            Node s = node.successorsList.at(i);
            if (reached.find(s.state) == reached.end() || s.pathCost < reached.at(s.state).pathCost) {
                reached[s.state] = s;
                frontier.push(s);
            }

        }

    }
    Node failed(problem);
    return failed;
}

void print(Node n) {
    int count = 0;
    for (int i = 0; i < n.printList.size(); i++) {
        cout << n.printList.at(i) << endl;
    }
    cout << ">>>>>>>>>>" << endl;
    cout << "Move " << n.pathCost << ", pathcost=" << n.pathCost << ", heuristic=" << n.heuristic() << ", f(n)=g(n)+h(n)=" << (n.pathCost + n.heuristic()) << endl;

    for (int i = 0; i < n.state.size(); i++) {
        cout << n.state.at(i) << endl;
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cout << "Missing Filename" << endl;
        return 0;
    }
    else if (argc > 2) {
        hNum = argv[2];
    }
    else {
        hNum = "-H3";
    }

    string filename = argv[1];
    vector<string> solution;
    vector<string> problem; 
    int stackCount;
    int blockCount;
    int moveCount = 0; 

    ifstream f;
    string read;
    f.open(filename);
    f >> stackCount >> blockCount >> moveCount;
    getline(f, read); 
    getline(f, read); 

    for (int i = 0; i < stackCount; i++) {
        getline(f, read);
        problem.push_back(read);
    }
    getline(f, read);

    for (int i = 0; i < stackCount; i++) {
        getline(f, read);
        solution.push_back(read);
    }   

    goalState = solution;

    Node goal = aStar(problem, solution);

    print(goal);

    cout << ">>>>>>>>>>" << endl;
    cout << "statistics: " << filename << " method ";
    if (hNum == "-H0") {
        cout << "BFS";
    }
    else {
        cout << "Astar";
    }
    cout << " planlen ";
    
    if (goal.pathCost != 0 && iterations <= maxIteration) {
        cout << goal.pathCost << " iter " << iterations << " maxq " << maxQ << endl;
    } 
    else {
        cout << "FAILED" << " iter " << iterations << " maxq " << maxQ << endl;
    }

}