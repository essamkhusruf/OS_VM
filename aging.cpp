#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

int main(int argc, char* argv[]) {

    if (argc < 2) {
        cout << "Provide input file\n";
        return 1;
    }

    int maxFrames = 10;

    if (argc >= 3)
        maxFrames = atoi(argv[2]);

    ifstream file(argv[1]);

    if (!file) {
        cout << "File error\n";
        return 1;
    }

    vector<int> pages;
    int page;

    while (file >> page)
        pages.push_back(page);

    for (int frames = 1; frames <= maxFrames; frames++) {

        vector<int> memory(frames, -1);
        vector<int> age(frames, 0);

        int faults = 0;

        for (int p : pages) {

            bool hit = false;

            for (int i = 0; i < frames; i++)
                age[i] >>= 1;

            for (int i = 0; i < frames; i++) {
                if (memory[i] == p) {
                    age[i] |= 128;
                    hit = true;
                    break;
                }
            }

            if (!hit) {

                faults++;

                int victim = -1;

                for (int i = 0; i < frames; i++) {
                    if (memory[i] == -1) {
                        victim = i;
                        break;
                    }
                }

                if (victim == -1) {
                    victim = 0;
                    for (int i = 1; i < frames; i++) {
                        if (age[i] < age[victim])
                            victim = i;
                    }
                }

                memory[victim] = p;
                age[victim] = 128;
            }
        }

        double faultsPer1000 =
            (double)faults / pages.size() * 1000;

        cout << frames << " "
             << faultsPer1000 << endl;
    }

    return 0;
}
