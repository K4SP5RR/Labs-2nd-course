#include "route.h"
#include <climits>
#include <iostream>

Route::Route(int num):Array(num){
    for(int i = 0; i < length; i++){
        arr[i] = i;
    }
}
Route::Route(const Route& src):Array(src){}

Route& Route::operator=(const Route& src){
    Array::operator=(src);
    return *this;
}

int Route::routePrice(int** matr){
    int road = 0;
    for(int i = 0; i < length-1; i++){
        road += matr[arr[i]][arr[i+1]];
    }
    road += matr[arr[length-1]][arr[0]];
    return road;
}

bool Route::nextRoute(){
    int i,j;
    for(i = length-2; i >= 0; i--){
        if(arr[i]<arr[i+1]){break;}
    }
    if(i < 0){return false;}
    
    for(j = length-1; j > i; j--){
        if(arr[i] < arr[j]){break;}
    }
    Swap(arr[i], arr[j]);
    int n = length-1;
    i++;
    while(i < n){
        Swap(arr[i], arr[n]);
        i++;
        n--;
    }
    return true;
}

void Route::Clean(int n, int m, int** matr){
    for(int i = 0; i < length; i++){
        matr[i][m] = 0;
    }
    for(int i = 0; i < length; i++){
        matr[n][i] = 0;
    }
}

bool Route::Check(int start, int next, int count, Edge* edges){
    for(int j = 0; j < count; j++){
        for(int i = 0; i < count; i++){
            if(next == edges[i].n){
                next = edges[i].m;
                if(next == start){
                    return 0; //есть цикл
                }
            }
        }
    }
    return 1;
}

int Route::greedySearch(int** data){
    int** matr = new int*[length];
    for(int i = 0; i < length; i++) {
        matr[i] = new int[length];
        for(int j = 0; j < length; j++) {
            matr[i][j] = data[i][j];
        }
    }
    Edge *edges = new Edge[length];
    int min = INT_MAX,count = 1;
    int n,m;
    while(count <= length){
        for(int i = 0; i < length; i++){
            for(int j = 0; j < length; j++){
                if(matr[i][j] > 0 && min > matr[i][j]){
                    if(count == length || Check(i,j,count,edges)){
                        min = matr[i][j];
                        n = i, m = j;
                    }
                }
            }
        }
        Clean(n,m,matr);
        edges[count-1].n = n;
        edges[count-1].m = m;
        edges[count-1].wei = min;
        min = INT_MAX;
        count++;
    }

    for(int i = 0; i < length; i++){
        cout << edges[i].n << " " << edges[i].m << " " << edges[i].wei << endl;
    }

    n=edges[0].n; m = edges[0].m;
    int weigth = 0;
    arr[0] = m;
    for(int j = 1; j < count; j++){
        for(int i = 0; i < count; i++){
            if(m == edges[i].n){
                m = edges[i].m;
                arr[j] = m;
                weigth += edges[i].wei;
                break;
            }
        }
    }

    for(int i = 0; i< length; i++){
        delete[] matr[i];
    }
    delete[] matr;
    delete[] edges;

    return weigth;
}

/*Идея оптимизации:
Как можно оптимизировать сложность n^5. У меня появилась идея. 
Основная проблема, что каждый раз я строю путь с нуля. 
А что если создать также массив ребер, только в структуру добавить ссылку на следующую и предыдущий элемент(сделать список).
И держать массив незадействованных ребер, а также этот список. 
Когда новое ребро добавили, то пытаемся включить в список, иначе оставляем как незадействованное
Если удалось включить ребро в список смотрим и на незадействованные ребра сразу же после включения нового 
и если получается включаем их в список, удаляя из массива.
Ну и проверять, чтобы после всех преобразований раньше времени не зациклился
Потенциально,если не получилось присоединить к освновному списку,
можно пытаться присоединить к незадействованным, формируя 2 список. Формируя множество дорожек. 
Тогда проверять только конец и начала цепочек на возможность их связать новым ребром*/



ostream& operator<<(ostream& cout, const Route& src){
    for(int i = 0; i < src.length; i++){
        cout << src.arr[i]<< " -> ";
    }
    cout << src.arr[0] << endl;
    return cout;
}





