#include <iostream>
#include <cstdlib>
#include <random>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <string>

typedef unsigned short BASE;
#define BASE_SIZE (sizeof(BASE)*8)
#define MASK ((1ULL<<BASE_SIZE)-1)
class BigNumber{
    std::vector<BASE> arr;
    size_t maxLen;
    size_t len;
    void Normalize();
public:
    BigNumber(size_t ml=1, bool random=false);
    BigNumber(const BigNumber&);
    BigNumber& operator=(const BigNumber&);
    friend std::ostream& operator<<(std::ostream&, const BigNumber&);
    friend std::istream& operator>>(std::istream&, BigNumber&);
    bool operator==(const BigNumber&)const;
    bool operator!=(const BigNumber&)const;
    bool operator>=(const BigNumber&)const;
    bool operator<=(const BigNumber&)const;
    bool operator<(const BigNumber&)const;
    bool operator>(const BigNumber&)const;
    BigNumber operator+(const BigNumber&)const;
    BigNumber operator+(const BASE&)const;
    BigNumber& operator+=(const BigNumber&);
    BigNumber& operator+=(const BASE&);
    BigNumber operator-(const BigNumber&)const;
    BigNumber operator-(const BASE&)const;
    BigNumber& operator-=(const BigNumber&);
    BigNumber& operator-=(const BASE&);
    BigNumber operator*(const BigNumber&)const;
    BigNumber operator*(const BASE&)const;
    BigNumber& operator*=(const BigNumber&);
    BigNumber& operator*=(const BASE&);
    BigNumber operator/(const BASE&)const;
    BASE operator%(const BASE&)const;
    BigNumber& operator/=(const BASE&);
    std::string toString10() const;
    void fromString10(const std::string&);
    BigNumber operator/(const BigNumber&)const;
    BigNumber operator%(const BigNumber&)const;
    BigNumber& operator/=(const BigNumber&);
};


BigNumber::BigNumber(size_t ml, bool random): arr(ml,0), maxLen(ml), len(1)
{
    if(random){
        static std::mt19937 rng(std::random_device{}());
        std::uniform_int_distribution<BASE> dist(0, MASK);
        for(size_t i = 0; i < ml; i++){
            arr[i] = dist(rng);
        }
        len = ml;
    }
    Normalize();
}

BigNumber::BigNumber(const BigNumber& cp): arr(cp.arr){
    len = cp.len;
    maxLen = cp.maxLen;
}

BigNumber& BigNumber::operator=(const BigNumber& cp){
    if (this != &cp){
        arr = cp.arr;
        len = cp.len;
        maxLen = cp.maxLen;
    }
    return *this;
}

void BigNumber::Normalize(){
    while(len > 1 && arr[len-1] == 0){
        len--;
    }
}
std::ostream& operator<<(std::ostream& os, const BigNumber& x){
    os << std::hex << std::uppercase << (unsigned long long)x.arr[x.len-1];
    size_t l = BASE_SIZE / 4;
    for(size_t k = 1; k < x.len; k++){
        size_t i = x.len - 1 - k;
        os << std::hex << std::uppercase << std::setw(l) << std::setfill('0') << (unsigned long long)x.arr[i];
    }
    os << std::dec;
    return os;    
}

std::istream& operator>>(std::istream& is, BigNumber& x){
    std::string s; 
    is >> s;
    size_t l = BASE_SIZE / 4;
    size_t pad = (l-s.length()%l)%l;
    s = std::string(pad, '0') + s;
    size_t newLen = s.length()/l;
    x.arr.resize(newLen);
    x.maxLen = newLen;
    x.len = newLen;
    for(size_t i = 0; i < newLen; i++){
        std::string bl = s.substr(s.length() - (i+1)*l,l);
        x.arr[i] = std::stoul(bl, nullptr, 16);
    }
    x.Normalize();
    return is;
}

bool BigNumber::operator==(const BigNumber& x) const{
    if(x.len != len){ return false;}
    for(size_t i = len; i > 0; i--){
        if(x.arr[i-1] != arr[i-1]){
            return false;
        }
    }
    return true;
}
bool BigNumber::operator!=(const BigNumber& x) const{
    return !(*this == x);
}

bool BigNumber::operator>=(const BigNumber& x) const{
    return !(*this < x);
}

bool BigNumber::operator<=(const BigNumber& x) const{
    return !(*this > x);
}

bool BigNumber::operator>(const BigNumber& x) const{
    if(x.len < len){ return true;}
    if(x.len > len){return false;}
    for(int i = len-1; i >= 0; i--){
        if(x.arr[i] > arr[i]){
            return false;
        }
        if (x.arr[i] < arr[i]){
            return true;
        }

    }
    return false;
}

bool BigNumber::operator<(const BigNumber& x) const{
    if(x.len > len){ return true;}
    if(x.len < len){return false;}

    for(int i = len-1; i >= 0; i--){
        if(x.arr[i] < arr[i]){
            return false;
        }
        if (x.arr[i] > arr[i]){
            return true;
        }

    }
    return false;
}

BigNumber BigNumber::operator+(const BASE& x)const{ 
    BigNumber res(len+1);
    unsigned long long s = x;
    size_t i = 0;
    for(; i < len; i++){
        s += arr[i];
        res.arr[i] = s;
        s >>= BASE_SIZE;
    }
    if (s > 0){
        res.arr[len] = s;
        res.len = i+1;
    }
    else{
        res.len = i;
    }
    return res;   
}

BigNumber BigNumber::operator+(const BigNumber& x)const{
    size_t l = std::max(len,x.len);
    size_t m = std::min(len,x.len);
    BigNumber res(l+1);
    unsigned long long s = 0;
    size_t i = 0;
    for(; i < m; i++){ 
        s += (unsigned long long)x.arr[i] + arr[i];
        res.arr[i] = s;
        s >>= BASE_SIZE;
    }
    for(; i < len; i++){ 
        s += arr[i]; 
        res.arr[i] = s;
        s >>= BASE_SIZE;
    }
    for(; i < x.len; i++){ 
        s += x.arr[i];
        res.arr[i] = s;
        s >>= BASE_SIZE;
    }
    if (s > 0){
        res.arr[l] = s;
        res.len = i+1;
    }
    else{
        res.len = i;
    }
    return res;   
}

BigNumber& BigNumber::operator+=(const BigNumber& x){
    *this = *this + x;
    return *this;
}

BigNumber& BigNumber::operator+=(const BASE& x){
    *this = *this + x;  
    return *this;
}

BigNumber BigNumber::operator-(const BASE& x)const{
    if(*this < x){ return BigNumber(1,false);}
    BigNumber res(*this);
    if(res.arr[0] >= x){
        res.arr[0] -= x;
    }
    else{
        size_t i = 1;
        res.arr[0] -= x;
        while(res.arr[i] == 0){
            res.arr[i] = MASK;
            i++;
        }
        res.arr[i]--;
    }
    res.Normalize();
    return res;   
}

BigNumber BigNumber::operator-(const BigNumber& x)const{
    if(*this < x){ return BigNumber(1,false);}

    BigNumber res(*this);
    size_t i = 0;
    unsigned long long b = 0;
    for(; i < x.len; i++){
        b += x.arr[i];
        if(res.arr[i] >= b){
            res.arr[i] -= b;
            b = 0;
        }
        else{
            res.arr[i] -= b;
            b = 1;
        }
    }
    for(; i < len && b > 0; i++){
        if(res.arr[i] == 0){
            res.arr[i] = MASK;
        }
        else{
            res.arr[i] -= b;
            b = 0;
        }
    }

    res.Normalize();
    return res;   
}

BigNumber& BigNumber::operator-=(const BigNumber& x){
    *this = *this - x;
    return *this;
}

BigNumber& BigNumber::operator-=(const BASE& x){
    *this = *this - x;
    return *this;
}

BigNumber BigNumber::operator*(const BASE& x)const{
    BigNumber res(len+1);
    BASE b = 0;
    unsigned long long tmp; 
    for (size_t i = 0; i < len; i++)
    {
        tmp = (unsigned long long)arr[i] * x + b;
        res.arr[i] = (BASE) tmp;
        b = tmp >> BASE_SIZE;
    }
    if(b != 0){
        res.arr[len] = b;
        res.len = len+1;
    }
    else{
        res.len = len;
    }
    return res;
}

BigNumber BigNumber::operator*(const BigNumber& x)const{
    BigNumber res(len+x.len);
    unsigned long long b = 0;
    for (size_t i = 0; i < len; i++)
    {   
        b = 0;
        for(size_t j = 0; j < x.len; j++){
            unsigned long long tmp = b + (unsigned long long)arr[i]*x.arr[j] + res.arr[i+j];
            res.arr[i+j] = (BASE)(tmp & MASK);
            b = tmp >> BASE_SIZE;
        }
        if(b){
            res.arr[x.len+i] = (BASE)b;
        }   
    }
    res.len = len+x.len;
    res.Normalize();
    return res;
}

BigNumber& BigNumber::operator*=(const BigNumber& x){
    *this = *this * x;
    return *this;
}

BigNumber& BigNumber::operator*=(const BASE& x){
    *this = *this * x;
    return *this;
}

BigNumber BigNumber::operator/(const BASE& x)const{
    if(x == 0){throw std::invalid_argument("Division by zero");}
    int j = len-1;
    BigNumber res(len);
    unsigned long long tmp, r = 0;
    for(;j >= 0; j--){
        tmp = (r<<BASE_SIZE)+arr[j];
        res.arr[j] = tmp / x;
        r = tmp % x;
    }
    res.len = len;
    res.Normalize();
    return res;
}

BigNumber& BigNumber::operator/=(const BASE& x){
    *this = *this / x;
    return *this;
}

BASE BigNumber::operator%(const BASE& x)const{
    if(x == 0) throw std::invalid_argument("Division by zero");
    int j = len-1;
    unsigned long long tmp, r = 0;
    for(;j >= 0; j--){
        tmp = (r<<BASE_SIZE)+arr[j];
        r = tmp % x;
    }
    return r;
}

void BigNumber::fromString10(const std::string& s){ //
    BigNumber res;
    *this = res;
    for(char c : s){
        *this *= (BASE)10;
        *this += (BASE)(c-'0');
    }
}

std::string BigNumber::toString10() const{
    BigNumber tmp(*this);
    BigNumber zero;
    std::string res;
    while(tmp !=  zero){
        res += ('0' + tmp % (BASE)10);
        tmp /= (BASE)10;
    }
    if(res.empty()) return "0";
    std::reverse(res.begin(), res.end());
    return res;
}

BigNumber BigNumber::operator/(const BigNumber& x)const {
    BigNumber zero;
    if(x == zero) throw std::invalid_argument("Division by zero");
    if(x > *this) return zero;
    if(x.len == 1){
        return *this / x.arr[0];
    }
    BigNumber u(*this);
    BigNumber v(x);
    unsigned long long b = MASK+1;
    unsigned long long d = b / (v.arr[v.len-1]+1);
    size_t t = u.len, n = v.len, m = t-n;
    BigNumber res(m+1);         
    res.len = m+1;
    u *= d;
    v *= d;
    if(u.len <= t){ u.arr.resize(t+1, 0); u.len = t+1; u.maxLen = t+1; }
    unsigned long long q,r;
    int j;
    for(j = m; j >= 0; j--){
        //D3
        q = ((unsigned long long)u.arr[j+n] * b + u.arr[j+n-1]) / v.arr[n-1];
        r = ((unsigned long long)u.arr[j+n] * b + u.arr[j+n-1]) % v.arr[n-1];
        while((q == b) || ((q*v.arr[n-2]) > (b*r + u.arr[j+n-2]))){
            q--;
            r += v.arr[n-1];
            if (r >= b) break;
        }
        //D4
        unsigned long long bor = 0;
        for(size_t i = 0; i < n; i++){
            unsigned long long sub = q * (unsigned long long)v.arr[i] + bor;
            long long dif = (long long)u.arr[j+i] - (long long) (sub & MASK);
            u.arr[j+i] = (BASE) (dif & MASK);
            bor = (sub >> BASE_SIZE) + (dif < 0 ? 1ULL : 0ULL);
        }
        long long dif = (long long)u.arr[j+n] - bor;
        u.arr[j+n] = (BASE) (dif & MASK);   
        int k;
        if (dif < 0) k=1;
        else k=0;
        //D5
        res.arr[j] = (BASE)q;
        //D6
        if (k==1){
            res.arr[j]--;
            unsigned long long c = 0;
            for(size_t i = 0; i < n; i++){
                c += (unsigned long long)u.arr[j+i] + v.arr[i];
                u.arr[j+i] = c & MASK;
                c >>= BASE_SIZE;
            }
            u.arr[j+n] += c; 
        }
    }
    res.Normalize();
    return res;
}

BigNumber BigNumber::operator%(const BigNumber& x)const {
    BigNumber zero;
    if(x == zero) throw std::invalid_argument("Division by zero");
    if(x > *this) return *this;
    if(x.len == 1){
        BigNumber ost;
        ost.arr[0] = *this % x.arr[0];
        return ost;
    }
    BigNumber u(*this);
    BigNumber v(x);
    unsigned long long b = MASK+1;
    unsigned long long d = b / (v.arr[v.len-1]+1);
    size_t t = u.len, n = v.len, m = t-n;

    u *= d;
    v *= d;
    if(u.len <= t){ u.arr.resize(t+1, 0); u.len = t+1; u.maxLen = t+1; }       
    unsigned long long q,r;
    int j;
    for(j = m; j >= 0; j--){
        //D3
        q = ((unsigned long long)u.arr[j+n] * b + u.arr[j+n-1]) / v.arr[n-1];
        r = ((unsigned long long)u.arr[j+n] * b + u.arr[j+n-1]) % v.arr[n-1];
        while((q == b) || ((q*v.arr[n-2]) > (b*r + u.arr[j+n-2]))){
            q--;
            r += v.arr[n-1];
            if (r >= b) break;
        }
        //D4
        unsigned long long bor = 0;
        for(size_t i = 0; i < n; i++){
            unsigned long long sub = q * (unsigned long long)v.arr[i] + bor;
            long long dif = (long long)u.arr[j+i] - (long long) (sub & MASK);
            u.arr[j+i] = (BASE) (dif & MASK);
            bor = (sub >> BASE_SIZE) + (dif < 0 ? 1ULL : 0ULL);
        }
        long long dif = (long long)u.arr[j+n] - bor;
        u.arr[j+n] = (BASE) (dif & MASK);   
        int k;
        if (dif < 0) k=1;
        else k=0;
        //D5
        //D6
        if (k==1){
            unsigned long long c = 0;
            for(size_t i = 0; i < n; i++){
                c += (unsigned long long)u.arr[j+i] + v.arr[i];
                u.arr[j+i] = c & MASK;
                c >>= BASE_SIZE;
            }
            u.arr[j+n] += c; 
        }
    }
    //D8
    BigNumber ost(n);
    ost.len = n;
    unsigned long long ost_r = 0;
    for(int i = n-1; i >= 0; i--){
        unsigned long long tmp = (ost_r<<BASE_SIZE)+u.arr[i];
        ost.arr[i] = tmp / d;
        ost_r = tmp % d;
    }
    ost.Normalize();
    return ost;
}

BigNumber& BigNumber::operator/=(const BigNumber& x) {
    *this = *this / x;
    return *this;
}

int main() {
    int max_length = 1000;
    int N = 1000;
    int len_A = rand() % max_length + 1;
    int len_D = rand() % max_length + 1;
    BigNumber A(len_A, true), D(len_D, true);

    BigNumber Q,R;
    do
    {
        int len_A = rand() % max_length + 1;
        int len_D = rand() % max_length + 1;
        BigNumber A1(len_A, true), D1(len_D, true);
        A = A1; D = D1;
        Q = A / D; 
        R = A % D;
        std::cout << N << ":  " << len_A << "  " << len_D << std::endl;
        if (!(A == Q * D + R && A - R == Q * D && R < D)){break;}
        
    } while(A == Q * D + R && A - R == Q * D && R < D && --N);

    if(N == 0) std::cout << "PASS\n";
    else std::cout << "FAIL\n";
    return 0;
}