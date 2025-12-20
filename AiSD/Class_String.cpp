#include <iostream>
#include <cstring>

using namespace std;

const int alphabet = 256;

class String {
	char* str;
	int len;
public:
	String(int l = 80) {
		str = new char[l+1];
		str[0] = '\0';
		len = l;
	}
	String(const char* x) {
		len = strlen(x);
		str = new char[len+1];
		memcpy(str, x, len);
		str[len] = '\0';
	}
	String(const String& x) {
		len = x.len;
		str = new char[len+1];
		memcpy(str, x.str, len);
		str[len] = '\0';
	}
	~String() {
		if (str) { 
			delete[] str;
			str = nullptr;
		} 
	}

	int strLenght();
	String& operator= (const String&);
	char& operator[](int);
	bool operator==(String&);
	bool operator!=(String&);
	String operator+(const String&);
	String& operator+=(const String&);
	friend istream& operator >> (istream&, String&);
	friend ostream& operator << (ostream&, String&);
	int BMSearch(String&);
	String operator()(int i, int j);
};
int String::strLenght() {
	return len;
}
 
String& String::operator= (const String& x) {
	if (this == &x) {
		return *this;
	}

	delete[] str;
	len = x.len;
	str = new char[len + 1];
	memcpy(str, x.str, len);
	str[len] = '\0';
	return *this;
}

char& String::operator[](int i) {
	if(i >= 0 && i < len){ 
		return str[i];
	}
	throw out_of_range("Некорректно");
}

bool String::operator==(String& x) {
	if (len != x.len) {
		return false;
	}

	for (int i = 0; i < len; i++)
	{
		if (str[i] != x.str[i])
		{
			return false;
		}
	}
	return true;
}

bool String::operator!=(String& x) {
	if (len != x.len) {
		return true;
	}

	for (int i = 0; i < len; i++)
	{
		if (str[i] != x.str[i])
		{
			return true;
		}
	}
	return false;
}

String String::operator+(const String& x) {
	String res(len +x.len);
	snprintf(res.str, res.len+1, "%s%s", str, x.str);
	return res;
}

String& String::operator+=(const String& x) {
	len += x.len;
	char* tmp = new char[len + 1];
	snprintf(tmp, len + 1, "%s%s", str, x.str);
	delete[] str;
	str = tmp;
	return *this;
}

istream& operator >> (istream& cin, String& x) {
	cin >> x.str;
	return cin;
}
ostream& operator << (ostream& cout, String&x) {
	cout << x.str;
	return cout;
}

int String::BMSearch(String& text) {
	int shifts[alphabet];
	for (int i = 0; i < alphabet; i++){shifts[i] = len;}
	for (int i = 0; i < len-1; i++) { shifts[str[i]] = len - 1 - i; }
	int i = len - 1, j = 0;
	while (i < text.len) {
		while (j < len && text.str[i - j] == str[len-j-1])
			j++;
		if (j == len)
			return i - j + 1;
		else {
			i += shifts[text.str[i]];
			j = 0;
		}
	}
	return -1;
}

String String::operator()(int i, int j) {
	String res(j - i + 1);
	memcpy(res.str, str + i, res.len);
	res.str[res.len] = '\0';
	return res;
}


int main() {
	/*String s1("Helloword"), s2 = "word";
	int i = s2.BMSearch(s1);
	s1 = s1(0, i - 1) + s1(i + s2.strLenght(), s1.strLenght() - 1);
	cout << s1[0] << endl;*/
	String s1 = "ABABCBABABCB", s2 = "ABCB";
	cout << s2.BMSearch(s1)<< endl;
}