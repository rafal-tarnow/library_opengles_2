#pragma once



#include <vector>
#include <string>
#include <iostream>
#include <algorithm>

using namespace std;

class ObjectCounter
{
public:
    ObjectCounter(string obj_name)
    {
        name = obj_name;
        objects.push_back(this);
    }
    ~ObjectCounter()
    {
        objects.erase(find(objects.begin(),objects.end(),this));
    }
    static void printObjects()
    {
        cout << "***********************" << endl;
        cout << "OBJECTS:" << endl;
        vector<ObjectCounter *>::iterator pos;
        for(pos = objects.begin(); pos != objects.end(); ++pos)
        {
            cout << "Object Name = " << (*pos)->name << endl;
        }
        cout << "***********************" << endl;
    }
    string name;
    static vector<ObjectCounter *> objects;
};

