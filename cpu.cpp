#include <iostream>
#include <fstream>
using namespace std;

int main() {
    ifstream file("input.txt");

    int n;
    file >> n;

    int arrival[n], burst[n];

    for (int i = 0; i < n; i++) {
        file >> arrival[i] >> burst[i];
    }

    // ===== FCFS =====
    int time = 0;
    float total_wait_fcfs = 0;

    for (int i = 0; i < n; i++) {
        if (time < arrival[i])
            time = arrival[i];

        total_wait_fcfs += time - arrival[i];
        time += burst[i];
    }

    float avg_fcfs = total_wait_fcfs / n;

    // ===== SJF =====
    int done[n] = {0};
    time = 0;
    float total_wait_sjf = 0;

    for (int count = 0; count < n; count++) {
        int idx = -1;
        int min = 9999;

        for (int i = 0; i < n; i++) {
            if (done[i] == 0 && arrival[i] <= time && burst[i] < min) {
                min = burst[i];
                idx = i;
            }
        }

        if (idx == -1) {
            time++;
            count--;
            continue;
        }

        total_wait_sjf += time - arrival[idx];
        time += burst[idx];
        done[idx] = 1;
    }

    float avg_sjf = total_wait_sjf / n;

    // ===== Round Robin =====
    int quantum = 2;
    int remaining[n];
    int waiting[n] = {0};

    for (int i = 0; i < n; i++)
        remaining[i] = burst[i];

    time = 0;
    int finished = 0;

    while (finished < n) {
        for (int i = 0; i < n; i++) {
            if (remaining[i] > 0 && arrival[i] <= time) {
                if (remaining[i] > quantum) {
                    time += quantum;
                    remaining[i] -= quantum;
                } else {
                    time += remaining[i];
                    waiting[i] = time - burst[i] - arrival[i];
                    remaining[i] = 0;
                    finished++;
                }
            }
        }
    }

    float total_wait_rr = 0;
    for (int i = 0; i < n; i++)
        total_wait_rr += waiting[i];

    float avg_rr = total_wait_rr / n;

    // ===== OUTPUT =====
    cout << "FCFS: " << avg_fcfs << endl;
    cout << "SJF: " << avg_sjf << endl;
    cout << "RR: " << avg_rr << endl;

    file.close();
    return 0;
}