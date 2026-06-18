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
    int num;

    while (file >> num)
        pages.push_back(num);

    file.close();

    for (int frames = 1; frames <= maxFrames; frames++) {

        vector<int> memory(frames, -1);
        vector<int> age(frames, 0);

        int faults = 0;

        for (int i = 0; i < pages.size(); i++) {

            int current = pages[i];
            bool found = false;

            // shift right (aging step)
            for (int j = 0; j < frames; j++)
                age[j] = age[j] >> 1;

            // check if page exists
            for (int j = 0; j < frames; j++) {
                if (memory[j] == current) {
                    age[j] = age[j] | 128;
                    found = true;
                    break;
                }
            }

            // if not found → page fault
            if (!found) {

                faults++;

                int index = -1;

                // check empty frame
                for (int j = 0; j < frames; j++) {
                    if (memory[j] == -1) {
                        index = j;
                        break;
                    }
                }

                // if no empty frame, find smallest age
                if (index == -1) {
                    index = 0;
                    for (int j = 1; j < frames; j++) {
                        if (age[j] < age[index])
                            index = j;
                    }
                }

                memory[index] = current;
                age[index] = 128;
            }
        }

        double rate = (double)faults / pages.size() * 1000;

        cout << frames << " " << rate << endl;
    }

    return 0;
}
