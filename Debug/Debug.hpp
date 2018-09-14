#pragma once
#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <algorithm>

using namespace std;

class ObjectCounter;

#define DBG_COUNT(x) ObjectCounter objectCounter = ObjectCounter(x)

class ObjectCounter
{
public:
    ObjectCounter(const ObjectCounter & ref)
    {
        //type = 2;
        name = ref.name;
        objects.push_back(this);
        objects_map[name].push_back(this);
    }
    ObjectCounter(string obj_name)
    {
        //type = 1;
        name = obj_name;
        objects.push_back(this);
        objects_map[name].push_back(this);
    }

    ~ObjectCounter()
    {
        auto it = find(objects.begin(), objects.end(),this);
        if(it != objects.end())
        {
            objects.erase(it);
        }

        auto itm = find(objects_map[this->name].begin(), objects_map[this->name].end(), this);
        if(itm != objects_map[this->name].end())
        {
            objects_map[this->name].erase(itm);
        }
    }

    static void print()
    {
        cout << "***********************" << endl;
        //printVectorObjects();
        printMapObjects();
        cout << "***********************" << endl;
    }

private:
    static void printVectorObjects()
    {

        cout << "VECTOR: size = " << objects.size() << endl;
        vector<ObjectCounter *>::iterator pos;
        for(pos = objects.begin(); pos != objects.end(); ++pos)
        {

            cout << "   Object Name = " << (*pos)->name << endl;
            //cout << "           Type = " << (*pos)->type << endl;

        }
    }
    static void printMapObjects()
    {
        cout << "MAP:" << endl;
        map<string, vector<ObjectCounter *>>::iterator positon;
        for( positon = objects_map.begin(); positon != objects_map.end(); ++positon)
        {
            if(positon->second.size() > 0)
                cout << "   " << positon->first << " " << positon->second.size() << endl;
        }
    }

private:
    string name;
    // int type = 0;
    static vector<ObjectCounter *> objects;
    static map<string, vector<ObjectCounter *>> objects_map;
};

