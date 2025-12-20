#include <iostream>
#include <chrono> 
#include <thread>
#include "libs/lib_arr/array.h"
using namespace std;
int main() {
    Array a{100000,1,100000}, a1{a}, a2{a}, a3{a}, a4{a};
    
    auto start1 = chrono::high_resolution_clock::now();
    a1.Hoar_sort();
    auto end1 = chrono::high_resolution_clock::now();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    auto start2 = chrono::high_resolution_clock::now();
    a2.Shell_sort();
    auto end2 = chrono::high_resolution_clock::now();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    auto start3 = chrono::high_resolution_clock::now();
    a3.Heapsort();
    auto end3 = chrono::high_resolution_clock::now();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    auto start4 = chrono::high_resolution_clock::now();
    a4.Heapsort_recursive();
    auto end4 = chrono::high_resolution_clock::now();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    long long time1 = chrono::duration_cast<chrono::microseconds>(end1-start1).count();
    long long time2 = chrono::duration_cast<chrono::microseconds>(end2-start2).count();
    long long time3 = chrono::duration_cast<chrono::microseconds>(end3-start3).count();
    long long time4 = chrono::duration_cast<chrono::microseconds>(end4-start4).count();
   
    auto start5 = chrono::high_resolution_clock::now();
    bool f1 = a1.isSorted();
    auto end5 = chrono::high_resolution_clock::now();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    long long time5 = chrono::duration_cast<chrono::microseconds>(end5-start5).count();
    bool f2 = a2.isSorted();
    bool f3 = a3.isSorted();
    bool f4 = a4.isSorted();
    
    auto start6= chrono::high_resolution_clock::now();
    bool f5 = (a1 == a2);
    auto end6= chrono::high_resolution_clock::now();
    long long time6 = chrono::duration_cast<chrono::microseconds>(end6-start6).count();

    cout << "Сортировка Хоара выполнена с кодом " << f1 << " за " << time1 << " микросекунд" << endl;
    cout << "Сортировка Шелла выполнена с кодом " << f2 << " за " << time2 << " микросекунд" << endl;
    cout << "Пирамидальная сортировка с итерацией выполнена с кодом " << f3 << " за " << time3 << " микросекунд" << endl;
    cout << "Пирамидальная сортировка с рекурсией выполнена с кодом " << f4 << " за " << time4 << " микросекунд" << endl;
    cout << "isSorted выполнена за " << time5 << " микросекунд" << endl;
    cout << "operator== выполнен за " << time6 <<" микросекунд" << " " << f5 << endl;
    return 0;   
}
