#pragma once
#include <iostream>

class Array {
    static bool init;
    static void Initialized();
    void recursive_Hoar_sort(int l, int r);
    void Sift_Down_recursive(int, int);
    void Sift_Down(int, int);
protected:
    int* arr;
    int length;
    static inline void Swap(int& x, int& y){
        int c = x;
        x = y;
        y = c;
    }
public:
    Array(int len = 1, int mode = 1, int range = 10);
    Array(int*, int len);
    Array(const Array&);
    ~Array();

    Array& operator=(const Array&);
    int& operator[](int);
    bool isSorted();
    bool operator==(const Array&);

    void Shell_sort();
    void Heapsort_recursive();
    void Heapsort();
    void Hoar_sort();
    // void Bit_sort();

    friend std::istream& operator>>(std::istream&, Array&);
    friend std::ostream& operator<<(std::ostream&, const Array&);
};
