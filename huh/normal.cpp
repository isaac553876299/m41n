#include <iostream>
#include <time.h>

const int game(const int a, const int b)
{
	if (a == 2 && b == 0) return 1;
	if (b == 2 && a == 0) return 2;
	return 0;
}

int main()
{
	const int games = 999999;
	srand(time(0));
	float x, y;
	int g;
	while (1)
	{
		x = y = g = 0;
		for (unsigned int i = 0; i < games; ++i)
		{
			g = game(rand() % 3, rand() % 3);
			if (g == 1) ++x;
			if (g == 2) ++y;
		}
		std::cout << " x: " << 100 * (x / games) << "%" << std::endl;
		std::cout << " y: " << 100 * (y / games) << "%" << std::endl;
		std::cout << " tied: " << 100 * ((games - x - y) / games) << "%" << std::endl;
		system("pause");
	}

	return 0;
}