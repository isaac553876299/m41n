#include <iostream>
#include <random>

float get_random()
{
	std::random_device rd;
	std::default_random_engine e(rd());
	std::uniform_real_distribution<float> dis(0.f, 1.f);
	return dis(e);
}

int main()
{
	srand(time(0));
	float iterations, pi;
	while (1)
	{
		iterations = 1000000.f; pi = 0;
		for (float i = 0.f; i < iterations; ++i)
		{
			float x = 1.f - (float(rand()) / float((RAND_MAX)) * 1.f);
			float y = 1.f - (float(rand()) / float((RAND_MAX)) * 1.f);
			pi += (1.f / iterations) * (sqrt(x * x + y * y) < 1.f);
		}
		std::cout << pi * 4 << std::endl;
		system("pause");
	}
	return 0;
}