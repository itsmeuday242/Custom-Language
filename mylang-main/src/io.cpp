#include <fstream>
#include <vector>
#include <string>
#include <iostream>
using namespace std;

string get_file_contents(const string filepath)
{
    ifstream file(filepath, ios::binary | ios::ate);
    if (!file)
    {
        cerr << "Error reading file " << filepath << '\n';
        exit(2);
    }
    file.seekg(0, ios::end);
    streampos length = file.tellg();
    file.seekg(0, ios::beg);
    vector<char> buffer(length);
    if (file.read(buffer.data(), length))
    {
        return string(buffer.data(), length);
    }
    cerr << "Error reading file " << filepath << '\n';
    exit(2);
}