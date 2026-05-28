#pragma once
#include "../lib_arr/array.h"
#include <iostream>
using namespace std;
struct Edge{
    int n = -1;
    int m = -1;
    int wei = -1;
};

class Route: public Array{
    void Clean(int, int, int**);
    bool Check(int, int, int count, Edge*);
public:
    Route(int num = 0);
    Route(const Route&);
    Route& operator= (const Route&);
    int routePrice(int**);
    int greedySearch(int**);
    bool nextRoute();
    friend ostream& operator<<(ostream&, const Route&);
};