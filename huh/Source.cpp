#include <iostream>
#include <time.h>

const int game(int l1, int l2)
{
	if (l1 < 0 || l2 < 0) std::cout << " ? ";
	int a = rand() % 3 - l1;
	int b = rand() % 3 - l2;
	if (a == 1 && b == 0) return 1;
	if (a == 1 && b == 2) return game(l1, --l2);
	if (b == 1 && a == 0) return 2;
	if (b == 1 && a == 2) return game(--l1, l2);
	return 0;
}

int main()
{
	const int games = 999;
	srand(time(0));
	float x, y;
	int g;
	while (1)
	{
		x = y = g = 0;
		for (unsigned int i = 0; i < games; ++i)
		{
			g = game(0, 0);
			if (g == 1) ++x;
			if (g == 2) ++y;
		}
		std::cout << " x: " << 100 * (x / games) << "%" << std::endl;
		std::cout << " y: " << 100 * (y / games) << "%" << std::endl;
		std::cout << " tied: " << 100 * ((games - x - y) / games) << "%" << std::endl;
		system("pause");
		system("cls");
	}

	return 0;
}