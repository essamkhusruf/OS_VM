#include <iostream>
#include <sys/stat.h>
#include <dirent.h>
#include <cstring>
#include <map>

using namespace std;

// Histogram: key = bin index, value = count
map<int, int> histogram;

// Function to process directory
void traverseDirectory(const string& path, int binWidth) {
    DIR* dir = opendir(path.c_str());
    if (!dir) return;

    struct dirent* entry;

    while ((entry = readdir(dir)) != NULL) {
        string name = entry->d_name;

        // Skip . and ..
        if (name == "." || name == "..")
            continue;

        string fullPath = path + "/" + name;

        struct stat fileStat;
        if (stat(fullPath.c_str(), &fileStat) == -1)
            continue;

        // If directory → recurse
        if (S_ISDIR(fileStat.st_mode)) {
            traverseDirectory(fullPath, binWidth);
        }
        // If regular file → process
        else if (S_ISREG(fileStat.st_mode)) {
            long size = fileStat.st_size;

            int binIndex = size / binWidth;
            histogram[binIndex]++;
        }
    }

    closedir(dir);
}

// Function to print histogram
void printHistogram(int binWidth) {
    cout << "\nFile Size Histogram:\n";

    for (auto it = histogram.begin(); it != histogram.end(); it++) {
        int bin = it->first;
        int count = it->second;

        long lower = bin * binWidth;
        long upper = lower + binWidth - 1;

        cout << "[" << lower << " - " << upper << "] : " << count << " files\n";
    }
}

int main() {
    string startDir;
    int binWidth;

    cout << "Enter starting directory: ";
    cin >> startDir;

    cout << "Enter bin width (e.g., 1024): ";
    cin >> binWidth;

    traverseDirectory(startDir, binWidth);
    printHistogram(binWidth);

    return 0;
}