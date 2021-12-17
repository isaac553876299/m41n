

#ifndef YNT
#define YNT

#include <string>
#include <iostream>
#include <vector>

class ynt
{

	std::string digits;

public:

	ynt(unsigned long long n = 0);
	ynt(std::string&);
	ynt(const char*);
	ynt(ynt&);

	friend bool Null(const ynt&);
	friend bool aredigits(const ynt&);
	friend void divide_by_2(ynt&);

	ynt& operator=(const ynt&);

	ynt& operator++(); ynt operator++(int);
	ynt& operator--(); ynt operator--(int);

	friend ynt& operator+=(ynt&, const ynt&); friend ynt operator+(const ynt&, const ynt&);
	friend ynt& operator-=(ynt&, const ynt&); friend ynt operator-(const ynt&, const ynt&);
	friend ynt& operator*=(ynt&, const ynt&); friend ynt operator*(const ynt&, const ynt&);
	friend ynt& operator/=(ynt&, const ynt&); friend ynt operator/(const ynt&, const ynt&);

	friend ynt& operator%=(ynt&, const ynt&); friend ynt operator%(const ynt&, const ynt&);

	friend ynt& operator^=(ynt&, const ynt&); friend ynt operator^(ynt&, const ynt&);

	friend bool operator==(const ynt&, const ynt&);
	friend bool operator!=(const ynt&, const ynt&);

	friend bool operator>(const ynt&, const ynt&);
	friend bool operator>=(const ynt&, const ynt&);
	friend bool operator<(const ynt&, const ynt&);
	friend bool operator<=(const ynt&, const ynt&);

	friend ynt sqrt(ynt& a);

	friend std::ostream& operator<<(std::ostream&, const ynt&);
	friend std::istream& operator>>(std::istream&, ynt&);

	friend ynt NthCatalan(int n);
	friend ynt NthFibonacci(int n);
	friend ynt Factorial(int n);
};

ynt::ynt(unsigned long long nr) {
	do {
		digits.push_back(nr % 10);
		nr /= 10;
	} while (nr);
}
ynt::ynt(std::string& s) {
	digits = "";
	for (int i = s.size() - 1; i >= 0; i--)
	{
		if (!isdigit(s[i]))
			throw("ERROR");
		digits.push_back(s[i] - '0');
	}
}
ynt::ynt(const char* s) {
	digits = "";
	for (int i = strlen(s) - 1; i >= 0; i--)
	{
		if (!isdigit(s[i]))
			throw("ERROR");
		digits.push_back(s[i] - '0');
	}
}
ynt::ynt(ynt& a) {
	digits = a.digits;
}

bool Null(const ynt& a) {
	if (a.digits.size() == 1 && a.digits[0] == 0)
		return true;
	return false;
}
inline bool aredigits(const ynt& a)
{
	for (const auto& i : a.digits)
	{
		if (!isdigit(i))
			return false;
	}
	return true;
}
void divide_by_2(ynt& a) {
	int add = 0;
	for (int i = a.digits.size() - 1; i >= 0; i--) {
		int digit = (a.digits[i] >> 1) + add;
		add = ((a.digits[i] & 1) * 5);
		a.digits[i] = digit;
	}
	while (a.digits.size() > 1 && !a.digits.back())
		a.digits.pop_back();
}

ynt& ynt::operator=(const ynt& a) {
	digits = a.digits;
	return *this;
}

ynt& ynt::operator++() {
	int i, n = digits.size();
	for (i = 0; i < n && digits[i] == 9; i++)
		digits[i] = 0;
	if (i == n)
		digits.push_back(1);
	else
		digits[i]++;
	return *this;
}
ynt ynt::operator++(int) {
	ynt aux;
	aux = *this;
	++(*this);
	return aux;
}
ynt& ynt::operator--() {
	if (digits[0] == 0 && digits.size() == 1)
		throw("UNDERFLOW");
	int i, n = digits.size();
	for (i = 0; digits[i] == 0 && i < n; i++)
		digits[i] = 9;
	digits[i]--;
	if (n > 1 && digits[n - 1] == 0)
		digits.pop_back();
	return *this;
}
ynt ynt::operator--(int) {
	ynt aux;
	aux = *this;
	--(*this);
	return aux;
}

ynt& operator+=(ynt& a, const ynt& b) {
	int t = 0, s, i;
	int n = a.digits.length(), m = b.digits.length();
	if (m > n) a.digits.append(m - n, 0);
	n = a.digits.length();
	for (i = 0; i < n; i++)
	{
		s = a.digits[i] + t + ((i < m) * b.digits[i]);
		t = s / 10;
		a.digits[i] = (s % 10);
	}
	if (t) a.digits.push_back(t);
	return a;
}
ynt operator+(const ynt& a, const ynt& b) {
	ynt temp;
	temp = a;
	temp += b;
	return temp;
}
ynt& operator-=(ynt& a, const ynt& b) {
	if (a < b)
		throw("UNDERFLOW");
	int n = a.digits.length(), m = b.digits.length();
	int i, t = 0, s;
	for (i = 0; i < n; i++)
	{
		s = a.digits[i] + t - ((i < m) * b.digits[i]);
		if (s < 0)
			s += 10,
			t = -1;
		else t = 0;
		a.digits[i] = s;
	}
	while (n > 1 && a.digits[n - 1] == 0)
		a.digits.pop_back(),
		n--;
	return a;
}
ynt operator-(const ynt& a, const ynt& b) {
	ynt temp;
	temp = a;
	temp -= b;
	return temp;
}
ynt& operator*=(ynt& a, const ynt& b)
{
	if (Null(a) || Null(b)) {
		a = ynt();
		return a;
	}
	int n = a.digits.size(), m = b.digits.size();
	std::vector<int> v(n + m, 0);
	for (int i = 0; i < n; i++)
		for (int j = 0; j < m; j++) {
			v[i + j] += (a.digits[i]) * (b.digits[j]);
		}
	n += m;
	a.digits.resize(v.size());
	for (int s, i = 0, t = 0; i < n; i++)
	{
		s = t + v[i];
		v[i] = s % 10;
		t = s / 10;
		a.digits[i] = v[i];
	}
	for (int i = n - 1; i >= 1 && !v[i]; i--)
		a.digits.pop_back();
	return a;
}
ynt operator*(const ynt& a, const ynt& b) {
	ynt temp;
	temp = a;
	temp *= b;
	return temp;
}
ynt& operator/=(ynt& a, const ynt& b) {
	if (Null(b))
		throw("Arithmetic Error: Division By 0");
	if (a < b) {
		a = ynt();
		return a;
	}
	if (a == b) {
		a = ynt(1);
		return a;
	}
	int i, lgcat = 0, cc;
	int n = a.digits.length(), m = b.digits.length();
	std::vector<int> cat(n, 0);
	ynt t;
	for (i = n - 1; t * 10 + a.digits[i] < b; i--) {
		t *= 10;
		t += a.digits[i];
	}
	for (; i >= 0; i--) {
		t = t * 10 + a.digits[i];
		for (cc = 9; cc * b > t; cc--);
		t -= cc * b;
		cat[lgcat++] = cc;
	}
	a.digits.resize(cat.size());
	for (i = 0; i < lgcat; i++)
		a.digits[i] = cat[lgcat - i - 1];
	a.digits.resize(lgcat);
	return a;
}
ynt operator/(const ynt& a, const ynt& b) {
	ynt temp;
	temp = a;
	temp /= b;
	return temp;
}

ynt& operator%=(ynt& a, const ynt& b) {
	if (Null(b))
		throw("Arithmetic Error: Division By 0");
	if (a < b) {
		a = ynt();
		return a;
	}
	if (a == b) {
		a = ynt(1);
		return a;
	}
	int i, lgcat = 0, cc;
	int n = a.digits.length(), m = b.digits.length();
	std::vector<int> cat(n, 0);
	ynt t;
	for (i = n - 1; t * 10 + a.digits[i] < b; i--) {
		t *= 10;
		t += a.digits[i];
	}
	for (; i >= 0; i--) {
		t = t * 10 + a.digits[i];
		for (cc = 9; cc * b > t; cc--);
		t -= cc * b;
		cat[lgcat++] = cc;
	}
	a = t;
	return a;
}
ynt operator%(const ynt& a, ynt& b) {
	ynt temp;
	temp = a;
	temp %= b;
	return temp;
}

ynt& operator^=(ynt& a, const ynt& b) {
	ynt Exponent, Base(a);
	Exponent = b;
	a = 1;
	while (!Null(Exponent)) {
		if (Exponent.digits[0] & 1)
			a *= Base;
		Base *= Base;
		divide_by_2(Exponent);
	}
	return a;
}
ynt operator^(ynt& a, ynt& b) {
	ynt temp(a);
	temp ^= b;
	return temp;
}

bool operator==(const ynt& a, const ynt& b) {
	return a.digits == b.digits;
}
bool operator!=(const ynt& a, const ynt& b) {
	return !(a == b);
}

bool operator>(const ynt& a, const ynt& b) {
	return b < a;
}
bool operator>=(const ynt& a, const ynt& b) {
	return !(a < b);
}
bool operator<(const ynt& a, const ynt& b) {
	int n = a.digits.length(), m = b.digits.length();
	if (n != m)
		return n < m;
	while (n--)
		if (a.digits[n] != b.digits[n])
			return a.digits[n] < b.digits[n];
	return false;
}
bool operator<=(const ynt& a, const ynt& b) {
	return !(a > b);
}

ynt sqrt(ynt& a) {
	ynt left(1), right(a), v(1), mid, prod;
	divide_by_2(right);
	while (left <= right) {
		mid += left;
		mid += right;
		divide_by_2(mid);
		prod = (mid * mid);
		if (prod <= a) {
			v = mid;
			++mid;
			left = mid;
		}
		else {
			--mid;
			right = mid;
		}
		mid = ynt();
	}
	return v;
}

std::ostream& operator<<(std::ostream& out, const ynt& a) {
	for (int i = a.digits.size() - 1; i >= 0; i--)
		std::cout << (short)a.digits[i];
	return std::cout;
}
std::istream& operator>>(std::istream& in, ynt& a) {
	std::string s;
	in >> s;
	int n = s.size();
	for (int i = n - 1; i >= 0; i--) {
		if (!isdigit(s[i]))
			throw("INVALID NUMBER");
		a.digits[n - i - 1] = s[i];
	}
	return in;
}

ynt NthCatalan(int n) {
	ynt a(1), b;
	for (int i = 2; i <= n; i++)
		a *= i;
	b = a;
	for (int i = n + 1; i <= 2 * n; i++)
		b *= i;
	a *= a;
	a *= (n + 1);
	b /= a;
	return b;
}

ynt NthFibonacci(int n) {
	ynt a(1), b(1), c;
	if (!n)
		return c;
	n--;
	while (n--) {
		c = a + b;
		b = a;
		a = c;
	}
	return b;
}

ynt Factorial(int n) {
	ynt f(1);
	for (int i = 2; i <= n; i++)
		f *= i;
	return f;
}

#endif

