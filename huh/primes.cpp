#include <iostream>
#include <fstream>

bool is_prime(unsigned int n)
{
	for (unsigned int i = 2; i <= n / 2; ++i)
		if (!(n % i))
			return false;
	return true;
}

bool prime_recursive(unsigned int n, unsigned int i = 2)
{
	if (n <= 2) return n == 2;
	if (n % i == 0) return false;
	if (i * i > n) return true;
	return prime_recursive(n, ++i);
}

void factors(unsigned int n)
{
	std::cout << n << ": ";
	if (n > 2)
	{
		while (n % 2 == 0)
		{
			std::cout << 2 << " ";
			n /= 2;
		}
		for (unsigned int i = 3; i <= sqrt(n); i += 2)
		{
			while (n % i == 0)
			{
				std::cout << i << " ";
				n /= i;
			}
		}
		if (n > 2)
		{
			std::cout << n << " ";
		}
	}
	else std::cout << n;
	std::cout << std::endl;
}

int main()
{
	for (unsigned int i = 0; i < 999; ++i)
	{
		factors(i);
	}

	return 0;
}
// 

void file(unsigned int up_to)
{
	std::fstream file("primes.txt", std::ios::in | std::ios::out | std::ios::app);
	if (file)
	{
		file.seekp(std::ios_base::end);
		for (unsigned int n = 0; n < up_to; ++n)
		{
			if (is_prime(n))
			{
				file << n << std::endl;
			}
		}
	}
	else
	{
		std::cout << "cannot open file" << std::endl;
	}
	file.flush();
	file.close();
}
