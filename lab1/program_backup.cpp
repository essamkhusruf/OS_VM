#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <pthread.h>

using namespace std;

struct Data
{
    vector<string> words;
    map<string,int> result;
};

void* count(void* arg)
{
    Data* d = (Data*)arg;

    for(int i = 0; i < d->words.size(); i++)
    {
        string w = d->words[i];
        d->result[w]++;
    }

    return NULL;
}

int main()
{
    ifstream file("input.txt");

    vector<string> all;
    string word;

    while(file >> word)
    {
        all.push_back(word);
    }

    int N = 2;

    pthread_t threads[N];
    Data data[N];

    int part = all.size() / N;

    for(int i=0;i<N;i++)
    {
        int start = i * part;
        int end = (i==N-1) ? all.size() : start + part;

        for(int j=start;j<end;j++)
            data[i].words.push_back(all[j]);

        pthread_create(&threads[i], NULL, count, &data[i]);
    }

    for(int i=0;i<N;i++)
        pthread_join(threads[i], NULL);

    map<string,int> final;

    for(int i=0;i<N;i++)
    {
        for(auto p : data[i].result)
            final[p.first] += p.second;
    }

    for(auto p : final)
        cout << p.first << " : " << p.second << endl;

    return 0;
}
