#include <iostream>
#include <cstdio>
#include <math.h>
using namespace std;
class complex {
	double re, im;
public:
	complex(double, double);
	double getRe();
	void setRe(double);
	double getIm();
	void setIm(double);
	bool operator==(const complex&);
	bool operator!=(const complex&);
	double mod();
	operator char*() const;
	complex operator - ();
	complex operator+(const complex&);
	complex operator-(const complex&);
	complex operator*(const complex&);
	complex operator/(const complex&);
	complex operator+=(const complex&);
	complex operator-=(const complex&);
	complex operator*=(const complex&);
	complex operator/=(const complex&);
	friend istream& operator>> (istream&, complex&);
	friend ostream& operator<< (ostream&, const complex &);
};

istream& operator>> (istream& cin, complex& x) {
	cin >> x.re >> x.im;
	return cin;
}

ostream& operator<< (ostream& cout, const complex& x) {
	if (x.im >= 0) { cout << x.re << "+" << x.im << "i"; }
	else{ cout << x.re << x.im << "i"; }
	return cout;
}

complex::complex(double Re = 0, double Im = 0) {
	re = Re;
	im = Im;
}

double complex::getRe() {
	return re;
}

double complex::getIm() {
	return im;
}

void complex::setRe(double Re) {
	re = Re;
}

void complex::setIm(double Im) {
	im = Im;
}
	
bool complex::operator==(const complex& x) {
	if (re == x.re && im == x.im) {
		return true;
	}
	return false;
}

bool complex::operator!=(const complex& x) {
	if (re != x.re || im != x.im) {
		return true;
	}
	return false;
}

complex::operator char* () const{
	char* str = new char[50];
	if (im >= 0) { sprintf_s(str, sizeof(char) * 50, "%.2f+%.2fi", re, im); }
	else{ sprintf_s(str, sizeof(char) * 50, "%.2f%.2fi", re, im); }
	return str;
}

complex complex::operator-() {
	im = -im;
	return *this;
}

double complex::mod() {
	double res;
	res = sqrt(pow(re, 2) + pow(im, 2));
	return res;
}

complex complex::operator+=(const complex& x) {
	this->re += x.re;
	this->im += x.im;
	return *this;
}
complex complex::operator-=(const complex& x) {
	this->re -= x.re;
	this->im -= x.im;
	return *this;
}
complex complex::operator*=(const complex& x) {
	complex res;
	res.re = re * x.re - im * x.im; //(1+2i)(2+3i)
	res.im = im * x.re + re * x.im;
	*this = res;
	return *this;
}
complex complex::operator/=(const complex& x) {
	complex res, sop = x;
	sop = -sop;
	double den = sop.re * x.re - sop.im * x.im;
	if (den == 0)
	{
		throw "Деление на 0!";
	}
	res.re = (re * sop.re - im * sop.im) / den; //(1+2i)(2-3i)/(2+3i)(2-3i)
	res.im = (re * sop.im + im * sop.re) / den;
	*this = res;
	return *this;
}

complex complex::operator+(const complex& x) {
	complex res;
	res.re = re + x.re;
	res.im = im + x.im;
	return res;
}
complex complex::operator-(const complex& x) {
	complex res;
	res.re = re - x.re;
	res.im = im - x.im;
	return res;
}
complex complex::operator*(const complex& x) {
	complex res;
	res.re = re * x.re - im * x.im; //(1+2i)(2+3i)
	res.im = im * x.re + re * x.im;

	return res;
}
complex complex::operator/(const complex& x) {
	complex res, sop=x;
	sop = -sop;
	double den = sop.re * x.re - sop.im * x.im;
	if (den == 0)
	{
		throw "Деление на 0!";
	}
	res.re = (re * sop.re - im * sop.im) / den; //(1+2i)(2-3i)/(2+3i)(2-3i)
	res.im = (re * sop.im + im * sop.re) / den;
	return res;
}

int main() {
	complex c1, c2, c3;
	cin >> c1 >> c2;
	cout << c1 << " " << c2 << endl;
	c3 = c1 / c2;
	cout << c3 << endl;
	c3 = c1 * c2;
	cout << c3 << endl;
	c3 /= c2;
	cout << c3 << endl;
	c3 *= c2;
	cout << c3 << endl;

	return 0;
}