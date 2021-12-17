#include <iostream>
#include <random>

unsigned int assignment_count;
unsigned int comparison_count;

bool check_order(unsigned int n, int a[])
{
	for (unsigned int i = 1; i < n; i++)
	{
		if (a[i] < a[i - 1]) return false;
	}
	return true;
}

void print_arr(unsigned int n, int a[])
{
	for (unsigned int i = 0; i < n; i++)
	{
		std::cout << "|" << a[i];
	}
	std::cout << "| " << (check_order(n, a) ? "order" : "chaos") << std::endl;
}

void radix_sort(unsigned int n, int a[])
{
	int max = a[0];
	for (unsigned int i = 1; i < n; i++) if (a[i] > max) max = a[i];
	for (int exp = 1; max / exp > 0; exp *= 10)
	{
		int* output = new int[n];
		int count[10] = { 0 };

		for (unsigned int i = 0; i < n; i++) count[(a[i] / exp) % 10]++;

		for (int i = 1; i < 10; i++) count[i] += count[i - 1];

		for (int i = n - 1; i >= 0; i--) output[--count[(a[i] / exp) % 10]] = a[i];
		
		for (unsigned int i = 0; i < n; i++) a[i] = output[i];

		delete[] output;
	}
}

void radix_sort_debug_count(unsigned int n, int a[])
{
	int max = a[0];
	for (unsigned int i = 1; i < n; i++)
	{
		if (a[i] > max)
		{
			max = a[i];
		}
	}
	for (int exp = 1; max / exp > 0; exp *= 10)
	{
		int* output = new int[n];
		int count[10] = { 0 };

		for (unsigned int i = 0; i < n; i++)
		{
			count[(a[i] / exp) % 10]++;
			comparison_count++;
			assignment_count += 2;
		}

		for (int i = 1; i < 10; i++)
		{
			count[i] += count[i - 1];
			comparison_count++;
			assignment_count += 2;
		}

		for (int i = n - 1; i >= 0; i--)
		{
			output[--count[(a[i] / exp) % 10]] = a[i];
			comparison_count++;
			assignment_count += 3;
		}

		for (unsigned int i = 0; i < n; i++)
		{
			a[i] = output[i];
			comparison_count++;
			assignment_count += 2;
		}

		delete[] output;
	}
}

int main()
{
	const unsigned int size = 20;
	int myarray[size];
	srand(time(0));
	while (1)
	{
		assignment_count = 0;
		comparison_count = 0;
		for (unsigned int i = 0; i < size; i++) myarray[i] = rand() % size;
		print_arr(size, myarray);
		radix_sort_debug_count(size, myarray);
		print_arr(size, myarray);
		std::cout << " assignments: " << assignment_count << " comparisons: " << comparison_count << std::endl;
		system("pause");
	}
	return 0;
}
