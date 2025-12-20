#include <iostream>
#include <cstring>
using namespace std;
template <typename T> class Array{
    T *arr;
    int length;
    void Sift_Down(int, int);
    static void Swap(T& x, T& y);
public:
    Array(int len = 1);
    Array(T*, int);
    Array(const Array<T>&);
    ~Array();
    Array<T>& operator=(const Array<T>&);
    T& operator[](int);
    bool isSorted();
    bool operator==(const Array<T>&);

    void Sort();
    template <typename T1> friend istream& operator>> (istream&, Array<T1>&);
    template <typename T1> friend ostream& operator<< (ostream&, const Array<T1>&);
};

template <typename T> Array<T>::Array(int len){
    length = len;
    arr = new T[length];
}

template <typename T> Array<T>::Array(T *src_arr, int len){
    length = len;
    arr = new T[length];
    memcpy(arr,src_arr,sizeof(T)*length);
}

template <typename T> Array<T>::Array(const Array<T>& src){
    length = src.length;
    arr = new T[length];
    memcpy(arr,src.arr,sizeof(T)*length);
}

template <typename T> Array<T>::~Array(){
    if(arr){
        delete[] arr;
    }
    arr = nullptr;
}

template <typename T> Array<T>& Array<T>::operator=(const Array<T>& src){
    if(this == &src){ return *this;}
    delete[] arr;
    length = src.length;
    arr = new T[length];
    memcpy(arr, src.arr, sizeof(T)*length);
    return *this;
}

template <typename T> T& Array<T>::operator[](int i){
    if(i < 0 || i >= length){ throw out_of_range("Incorrect index");}
    return arr[i];
}

template <typename T> bool Array<T>::isSorted(){
    for(int i = 0; i < length-1; i++){
        if(arr[i] > arr[i+1]){return false;}
    }
    return true;
}

template <typename T> bool Array<T>::operator==(const Array<T>& src){
    if(length != src.length){return false;}
    Array<T> temp1{*this}, temp2{src};
    temp1.Sort();
    temp2.Sort();
    for(int i = 0; i <length; i++){
        if(temp1.arr[i] != temp2.arr[i]){ return false;}
    }
    return true;
}

template <typename T> inline void Array<T>::Swap(T& x, T& y) {
    T c = x;
    x = y;
    y = c;
}

template <typename T> void Array<T>::Sift_Down(int i, int n){
    int m, left, right;
    T temp = arr[i];
    while(true){
        left = 2*i+1, right = left+1, m =left;;
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


template <typename T> void Array<T>::Sort(){
    for (int i = length/2-1; i >= 0; i--){
        Sift_Down(i, length);
    }
    int n = length;
    while (n > 1){
        Swap(arr[0], arr[--n]);
        Sift_Down(0,n);
    }
}

template <typename T1> istream& operator>> (istream& cin, Array<T1>& dest){
    if(dest.arr){
        delete[] dest.arr;
    }
    cout << "Введите длину: ";
    int n;
    cin >> n;
    if(n <= 0){
        throw out_of_range("Invalid length");
    }
    dest.length = n;
    dest.arr = new T1[dest.length];
    for(int i = 0; i <dest.length; i++){ cin >> dest.arr[i];}
    return cin;
}

template <typename T1> ostream& operator<< (ostream& cout, const Array<T1>& dest){
    cout << "[ ";
    for(int i = 0; i <dest.length; i++){ cout << dest.arr[i] << " ";}
    cout << "]";
    return cout;
}


int main(){
    int *x;
    x = new int[5];
    cout << "Введите значения типа int" << endl;
    for(int i = 0; i < 5; i++){cin >> x[i];}
    Array m_i{x,5};
    Array<double> m_d{2};
    Array<char> m_ch{5};
    Array<string> m_st;
    cout << "Введите значения double" << endl;
    cin >> m_d;
    cout << "Введите значения string" << endl;
    cin >> m_st;
    for (int i = 0; i < 5; i++){
        m_ch[i] = m_st[0][i];
    }
    delete[] x;
    cout << "Вывод массива int" << m_i <<endl;
    cout << "Вывод массива double" << m_d <<endl;
    cout << "Вывод массива char" << m_ch <<endl;
    cout << "Вывод массива string" << m_st <<endl;

    Array<double> cp{2};
    cp = m_d;

    m_i.Sort();
    m_d.Sort();
    m_ch.Sort();
    m_st.Sort();

    cout << "Вывод отсортированного массива int с кодом " << m_i.isSorted() << endl;
    cout << m_i << endl;
    cout << "Вывод отсортированного массива double с кодом " << m_d.isSorted() << endl;
    cout << m_d << endl;
    cout << "Вывод отсортированного массива String с кодом " << m_st.isSorted() << endl;
    cout << m_st << endl;
    cout << "Вывод отсортированного массива char с кодом " << m_ch.isSorted() << endl;
    cout << m_ch << endl;

    if(cp == m_d){cout << "Равенство" << endl;}
    cout << cp << endl;
    cout << m_d << endl;
    return 0;
}