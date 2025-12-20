#include "array.h"

#include <cmath>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <iostream>
using namespace std;

bool Array::init = false;

void Array::Initialized() {
    if (!init) {
        srand(time(NULL));
        init = true;
    }
}

Array::Array(int len, int mode, int range) {
    if (len <= 0) throw std::invalid_argument("Length must be positive");
    Initialized();
    arr = new int[len];
    length = len;
    switch (mode) {
        case 1:
            if (range <= 0)
                throw std::invalid_argument("Range must be positive");
            for (int i = 0; i < length; i++) {
                arr[i] = rand() % range;
            }
            break;
        case 2:
            arr[0] = rand() % 1000;
            for (int i = 1; i < length; i++) {
                arr[i] = arr[i - 1] + rand() % 100;
            }
            break;
        case 3:
            arr[0] = 1000 + rand() % 1000;
            for (int i = 1; i < length; i++) {
                arr[i] = arr[i - 1] - rand() % 100;
            }
            break;
        default:
            throw std::invalid_argument("Incorrect mode");
    }
}

Array::Array(int* src, int len) {
    if (len <= 0) throw std::invalid_argument("Length must be positive");
    arr = new int[len];
    length = len;
    memcpy(arr, src, sizeof(int) * length);
}

Array::Array(const Array& src) {
    arr = new int[src.length];
    length = src.length;
    memcpy(arr, src.arr, sizeof(int) * length);
}

Array::~Array() {
    if (arr) {
        delete[] arr;
    }
    arr = nullptr;
}

Array& Array::operator=(const Array& src) {
    if (this == &src) {
        return *this;
    }
    delete[] arr;
    arr = new int[src.length];
    length = src.length;
    memcpy(arr, src.arr, sizeof(int) * length);
    return *this;
}

int& Array::operator[](int i) {
    if (i < 0 || i >= length) {
        throw std::out_of_range("Incorrect index");
    }

    return arr[i];
}

std::istream& operator>>(std::istream& cin, Array& dest) {
    delete[] dest.arr;
    cout << "Input size: ";
    cin >> dest.length;
    dest.arr = new int[dest.length];
    for (int i = 0; i < dest.length; i++) cin >> dest.arr[i];
    return cin;
}

std::ostream& operator<<(std::ostream& cout, const Array& dest) {
    cout << "[";
    for (int i = 0; i < dest.length; i++) cout << dest.arr[i] << " ";
    cout << "]" << endl;
    return cout;
}

bool Array::operator==(const Array& src) {
    if (length != src.length) return false;
    bool* m;
    m = new bool[length];
    for (int i = 0; i < length; i++) {
        m[i] = true;
    }

    bool flag = false;
    bool result = true;

    for (int i = 0; i < length; i++) {
        for (int j = 0; j < length; j++) {
            if (arr[i] == src.arr[j] && m[j]) {
                m[j] = false;
                flag = true;
                break;
            }
        }
        if (!flag) {
            result = false;
            break;
        }
        flag = false;
    }
    delete[] m;
    return result;
}

bool Array::isSorted() {
    for (int i = 0; i < length - 1; i++) {
        if (arr[i] > arr[i + 1]) return false;
    }
    return true;
}


void Array::Swap(int& x, int& y) {
    int c = x;
    x = y;
    y = c;
}

void Array::Sift_Down_recursive(int i, int n){
    int m = i, left = 2*i+1, right = left+1;
    
    if(left < n && arr[m] < arr[left]){
        m = left;
    }

    if(right < n && arr[m] < arr[right]){
        m = right;
    }
    if(m != i){
        Swap(arr[i], arr[m]);
        Sift_Down_recursive(m,n);
    }
    return;
}


void Array::Heapsort_recursive(){
    for (int i = length/2-1; i >= 0; i--){
        Sift_Down_recursive(i, length);
    }
    int n = length;
    while (n > 1){
        Swap(arr[0], arr[--n]);
        Sift_Down_recursive(0,n);
    }
}

void Array::Sift_Down(int i, int n){
    int m, left, right;
    int temp = arr[i];
    while(true){
        left = 2*i+1, right = left+1, m =left;
        if(left >= n){break;}
        if(right < n && arr[right] > arr[left]){m=right;}
        if(temp < arr[m]){
            arr[i] = arr[m];
            i = m;
        }
        else{break;}
    }
    arr[i] = temp;
}


void Array::Heapsort(){
    for (int i = length/2-1; i >= 0; i--){
        Sift_Down(i, length);
    }
    int n = length;
    while (n > 1){
        Swap(arr[0], arr[--n]);
        Sift_Down(0,n);
    }
}

void Array::Shell_sort() {
    for (int n = length / 2; n > 0; n = n / 2) {
        for (int i = n; i < length; i++) {
            int j = i;
            int temp = arr[j];
            while (j >= n && temp < arr[j - n]) {
                arr[j] = arr[j - n];
                j -= n;
            }
            arr[j] = temp;
        }
    }
}

void Array::recursive_Hoar_sort(int l, int r) {
    if(l >= r){return;}
    int i = l, j = r;
    int x = arr[(r+l)/ 2];
    while(i <= j){
        while(arr[i] < x) i++;
        while(arr[j] > x) j--;
        if(i<=j){
            Swap(arr[i], arr[j]);
            i++;
            j--;
        }
    }
    recursive_Hoar_sort(l,j);
    recursive_Hoar_sort(i,r);
}

void Array::Hoar_sort() {
    int l = 0, r = length - 1;
    recursive_Hoar_sort(l, r);
}