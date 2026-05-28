#include <iostream>
#include <cstdlib>
#include <random>
#include <string>
#include <queue>

class Node{
    int key;
    Node *left;
    Node *right;
    Node(int k = 0): key(k), left(nullptr), right(nullptr){}
    friend class SearchTree;
};

class SearchTree{
    Node *root;
    Node* copy(Node*);
    Node* remove(Node*,int);
    void print(Node*, int);
    void inorder(Node*);
    void clear(Node*);
public:
    SearchTree(): root(nullptr) {}
    SearchTree(int n):root(nullptr){
        for (int i = 0; i < n; i++)
        {
            insert(rand()%1000);           
        }
    }
    SearchTree(int* m, int n):root(nullptr){
        for (int i = 0; i < n; i++)
        {
            insert(m[i]);           
        }
    }
    SearchTree(const SearchTree &cp):root(nullptr){
        root = copy(cp.root);
    }
    ~SearchTree(){
        clear(root);
    }
    SearchTree& operator=(const SearchTree &);
    void insert(int);
    bool search(int);
    int findMin();
    int findMax();
    void remove(int);
    void inorder();
    void levelorder();
    void print();
};

SearchTree& SearchTree::operator=(const SearchTree &src){
    if (this == &src){return *this;}
    clear(root);
    root = copy(src.root);
    return *this;
}

void SearchTree::print(){
    print(root, 0);
}

void SearchTree::print(Node* x, int level){
    if(x == nullptr) return;
    print(x->right, level + 1);
    for(int i = 0; i < level; i++) std::cout << "    ";
    std::cout << x->key << "\n";
    print(x->left, level + 1);
}

void SearchTree::levelorder(){
    if(root == nullptr){return;}
   
    std::queue<Node*> q;
    q.push(root);
    while (!q.empty()){
        Node* cur = q.front();
        q.pop();
        std::cout << cur->key << " ";
        if(cur->left) {q.push(cur->left);}
        if(cur->right) {q.push(cur->right);}
    }
    std::cout<<std::endl;
}

void SearchTree::inorder(){
    inorder(root);
    std::cout<<std::endl;
}

void SearchTree::inorder(Node* x){
    if(x == nullptr){return;}
    std::cout << x->key << " ";
    inorder(x->left);
    inorder(x->right);
}

Node* SearchTree::remove(Node* x, int k){
    if(x == nullptr){return nullptr;}
    if(k < x->key){
        x->left = remove(x->left, k);
    }
    else{
        if(k > x->key){
            x->right = remove(x->right, k);
        }
        else{
            if (x->left == nullptr){
                Node* rightCh = x->right;
                delete x;
                return rightCh;
            }
            if (x->right == nullptr){
                Node* leftCh = x->left;
                delete x;
                return leftCh;
            }
            Node* minNode = x;
            Node* pred = x;
            minNode = minNode->right;
            while(minNode->left){
                pred = minNode;
                minNode = minNode->left;
            }
            if(pred == x){
                pred->right = minNode->right;
            }
            else{
                pred->left = minNode->right;
            }
            x->key = minNode->key;
            delete minNode;
        }
    }
    return x;
}

void SearchTree::remove(int k){
    root = remove(root, k);
}

int SearchTree::findMax(){
    if(root == nullptr){throw std::exception();}
    Node* cur = root;
    while(cur->right != nullptr){
        cur = cur->right;
    }
    return cur->key;
}

int SearchTree::findMin(){
    if(root == nullptr){throw std::exception();}
    Node* cur = root;
    while(cur->left != nullptr){
        cur = cur->left;
    }
    return cur->key;
}

bool SearchTree::search(int k){
    Node* cur = root;
    while(cur){
        if(k == cur->key){return true;}
        if(k > cur->key){cur = cur->right;}
        else{cur = cur->left;}
    }
    return false;
}

void SearchTree::clear(Node* x){
    if(x == nullptr){return;}
    clear(x->left);
    clear(x->right);
    delete x;
}

Node* SearchTree::copy(Node* src){
    if (src == nullptr)
    {
        return nullptr;
    }
    Node* dest = new Node(src->key);
    dest->left = copy(src->left);
    dest->right = copy(src->right);
    return dest;
}

void SearchTree::insert(int k){
    Node* cur = root;
    Node* pred = nullptr;
    while(cur){
        pred = cur;
        if(k > cur->key){
            cur = cur->right;
        } 
        else{
            cur = cur->left;
        }
    }
    cur = new Node(k);
    if(pred == nullptr){ root = cur; return; }
    if(k > pred->key){
        pred->right = cur;
    } 
    else{
        pred->left = cur;
    }
}

int main() {
    srand(time(0));
    
    int n, key;
    int arr[100];

    std::cout << "Пустое дерево\n";
    SearchTree t1;
    t1.print();
    
    std::cout << "\nДерево из случайных ключей\n";
    std::cout << "Введите количество узлов: ";
    std::cin >> n;
    SearchTree t2(n);
    t2.print();
    std::cout << "Обход в глубину (КЛП): ";
    t2.inorder();
    std::cout << "Обход в ширину:        ";
    t2.levelorder();
    std::cout << "Минимум: " << t2.findMin() << "\n";
    std::cout << "Максимум: " << t2.findMax() << "\n";
    
    std::cout << "\nУдаление\n";
    std::cout << "Введите ключ для удаления (лист): ";
    std::cin >> key;
    t2.remove(key);
    t2.print();
    t2.inorder();
    t2.levelorder();    

    std::cout << "\nДерево из массива\n";
    std::cout << "Введите количество элементов: ";
    std::cin >> n;
    std::cout << "Введите " << n << " элементов: ";
    for (int i = 0; i < n; i++) {
        std::cin >> arr[i];
    }
    SearchTree t3(arr, n);
    t3.print();
    std::cout << "Обход в глубину (КЛП): ";
    t3.inorder();
    std::cout << "Обход в ширину:        ";
    t3.levelorder();
    std::cout << "Минимум: " << t3.findMin() << "\n";
    std::cout << "Максимум: " << t3.findMax() << "\n";
    
    
    std::cout << "\nПоиск\n";
    std::cout << "Введите ключ для поиска: ";
    std::cin >> key;
    std::cout << "search(" << key << "): " << (t3.search(key) ? "найден" : "не найден") << "\n";
    
    
    std::cout << "\nДобавление\n";
    std::cout << "Введите ключ для добавления: ";
    std::cin >> key;
    t3.insert(key);
    t3.print();
    t3.inorder();
    t3.levelorder();
    std::cout << "search(" << key << "): " << (t3.search(key) ? "найден" : "не найден") << "\n";
    
 
    std::cout << "\nУдаление\n";
    std::cout << "Введите ключ для удаления (лист): ";
    std::cin >> key;
    t3.remove(key);
    t3.print();
    t3.inorder();
    t3.levelorder();
    
    std::cout << "Введите ключ для удаления (узел с двумя потомками): ";
    std::cin >> key;
    std::cout << "Удаляем " << key << ":\n";
    t3.remove(key);
    t3.print();
    t3.inorder();
    t3.levelorder();
    
    std::cout << "Введите ключ для удаления (несуществующий): ";
    std::cin >> key;
    std::cout << "Удаляем " << key << ":\n";
    t3.remove(key);
    t3.print();
    t3.inorder();
    t3.levelorder();
    
    
    std::cout << "\nКонструктор копирования\n";
    SearchTree t4(t3);
    std::cout << "Копия: ";
    t4.inorder();
    std::cout << "Введите ключ для вставки в копию: ";
    std::cin >> key;
    t4.insert(key);
    std::cout << "После вставки " << key << " в копию:\n";
    std::cout << "Копия:    ";
    t4.inorder();
    std::cout << "Оригинал: ";
    t3.inorder();
    
    std::cout << "\nОператор присваивания\n";
    SearchTree t5;
    t5 = t3;
    std::cout << "Присвоенное дерево: ";
    t5.inorder();
    
    return 0;
}