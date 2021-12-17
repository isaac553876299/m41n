
#include <string>

int64_t karatsuba_algorithm(std::string str1, std::string str2)
{
	while (str1.size() < str2.size()) str1 = "0" + str1;
	while (str1.size() > str2.size()) str2 = "0" + str2;
	int64_t n = std::max(str1.size(), str2.size());
	if (n == 0) return 0;
	if (n == 1) return ((str1[0] - '0') * (str2[0] - '0'));
	int64_t fh = n / 2;
	int64_t sh = (n - fh);
	std::string Xl = str1.substr(0, fh);
	std::string Xr = str1.substr(fh, sh);
	std::string Yl = str1.substr(0, fh);
	std::string Yr = str1.substr(fh, sh);
	int64_t product1 = karatsuba_algorithm(Xl, Yl);
	int64_t product2 = karatsuba_algorithm(Xr, Yr);
	int64_t product3 = karatsuba_algorithm((Xl + Xr), (Yl + Yr));
	return (int64_t)(product1 * (1 << (2 * sh)) + (product3 - product1 - product2) * (1 << sh) + product2);
}

int main()
{

	return 0;
}