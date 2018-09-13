#pragma once



#include <set>
#include <string>
#include <iostream>

using namespace std;

class ObjectCounter
{
public:
    ObjectCounter(string obj_name)
    {
        name = obj_name;
        objects.insert(this);
    }
    ~ObjectCounter()
    {
        objects.erase(this);
    }
    static void printObjects()
    {
        cout << "***********************" << endl;
        cout << "OBJECTS:" << endl;
        set<ObjectCounter *>::iterator pos;
        for(pos = objects.begin(); pos != objects.end(); ++pos)
        {
            cout << "Object Name = " << (*pos)->name << endl;
        }
        cout << "***********************" << endl;
    }
    string name;
    static set<ObjectCounter * > objects;
};

