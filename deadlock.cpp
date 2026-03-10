#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

int main() {

    ifstream file("input.txt");

    int n, m;
    file >> n >> m;   // n = processes, m = resource types

    vector<int> E(m);
    for(int i = 0; i < m; i++)
        file >> E[i];

    vector<vector<int>> C(n, vector<int>(m));
    for(int i = 0; i < n; i++)
        for(int j = 0; j < m; j++)
            file >> C[i][j];

    vector<vector<int>> R(n, vector<int>(m));
    for(int i = 0; i < n; i++)
        for(int j = 0; j < m; j++)
            file >> R[i][j];

    vector<int> available(m);

    for(int j = 0; j < m; j++){
        int sum = 0;
        for(int i = 0; i < n; i++)
            sum += C[i][j];

        available[j] = E[j] - sum;
    }

    vector<bool> finished(n, false);
    bool progress = true;

    while(progress){ //loop to check if processes can be finished
        progress = false;

        for(int i = 0; i < n; i++){

            if(!finished[i]){

                bool canFinish = true;

                for(int j = 0; j < m; j++){//check if enough available resources
                    if(R[i][j] > available[j]){
                        canFinish = false;
                        break;
                    }
                }

                if(canFinish){//update available and finish process

                    for(int j = 0; j < m; j++)
                        available[j] += C[i][j];

                    finished[i] = true;
                    progress = true;
                }
            }
        }
    }

    bool deadlock = false;

    for(int i = 0; i < n; i++){
        if(!finished[i]){
            deadlock = true;
            break;
        }
    }

    if(!deadlock){
        cout << "No deadlock detected\n";
    }
    else{
        cout << "Deadlock detected. Processes: ";

        for(int i = 0; i < n; i++){
            if(!finished[i])
                cout << "P" << i << " ";
        }

        cout << endl;
    }

    return 0;
}
