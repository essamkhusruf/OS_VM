#include <iostream>
#include <sys/stat.h>
#include <dirent.h>
#include <cstring>
#include <map>

using namespace std;

map<int, int> histogram;

// Traverse directory recursively
void traverseDirectory(const string& path, int binWidth) {
    DIR* dir = opendir(path.c_str());
    if (!dir) return;

    struct dirent* entry;
        string name = entry->d_name;

        if (name == "." || name == "..")
            continue;

        string fullPath = path + "/" + name;

        struct stat fileStat;
        if (stat(fullPath.c_str(), &fileStat) == -1)
            continue;

        if (S_ISDIR(fileStat.st_mode)) {
            traverseDirectory(fullPath, binWidth);
        }
        else if (S_ISREG(fileStat.st_mode)) {
            long size = fileStat.st_size;
            int binIndex = size / binWidth;
            histogram[binIndex]++;
        }
    }

    closedir(dir);
}

// Print histogram
void printHistogram(int binWidth) {
    cout << "\nFile Size Histogram:\n";

    for (auto it = histogram.begin(); it != histogram.end(); it++) {
        int bin = it->first;
        int count = it->second;

        long lower = bin * binWidth;
        long upper = lower + binWidth - 1;

        cout << "[" << lower << " - " << upper << "] : "
             << count << " files\n";
    }
}

int main() {
    string startDir;
    int binWidth;

    cout << "Enter starting directory: ";
    cin >> startDir;

    cout << "Enter bin width: ";
    cin >> binWidth;

    traverseDirectory(startDir, binWidth);
    printHistogram(binWidth);

    return 0;
}