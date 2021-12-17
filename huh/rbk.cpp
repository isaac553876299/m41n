#include <iostream>

struct rbk
{
	/*
	*          |00|01|02|                   *
	*          |03|04|05|                   *
	*          |06|07|08|                   *
	* |36|37|38|09|10|11|18|19|20|27|28|29| *
	* |39|40|41|12|13|14|21|22|23|30|31|32| *
	* |42|43|44|15|16|17|24|25|26|33|34|35| *
	*          |45|46|47|                   *
	*          |48|49|50|                   *
	*          |51|52|53|                   *
	*/
	int stickers[54];

	rbk()
	{
		reset();
	}

	void reset()
	{
		for (int i = 0; i < 54; ++i) stickers[i] = i / 9;
	}

	bool check() const
	{
		for (int i = 0; i < 54; ++i) if (stickers[i] != i / 9) return false;
		return true;
	}


	// adjacent faces' stickers to swap
	const int a[6][12] =
	{
		/*U*/29,28,27,20,19,18,11,10,9,38,37,36,
		/*F*/6,7,8,18,21,24,47,46,45,44,41,38,
		/*R*/8,5,2,27,30,33,53,50,47,17,14,11,
		/*B*/2,1,0,36,39,42,51,52,53,26,23,20,
		/*L*/0,3,6,9,12,15,45,48,51,35,32,29,
		/*D*/15,16,17,24,25,26,33,34,35,42,43,44,
	};

	// to the right i1->i2->i3->i4->i1
	void swap(const bool direction, const int i1, const int i2, const int i3, const int i4)
	{
		int tmp = stickers[i1];
		if (direction)
		{
			stickers[i1] = stickers[i4];
			stickers[i4] = stickers[i3];
			stickers[i3] = stickers[i2];
			stickers[i2] = tmp;
		}
		else
		{
			stickers[i1] = stickers[i2];
			stickers[i2] = stickers[i3];
			stickers[i3] = stickers[i4];
			stickers[i4] = tmp;
		}
	}

	void twist(int i, bool clockwise)
	{
		int j = 9 * i;
		swap(clockwise, 0 + j, 2 + j, 8 + j, 6 + j);
		swap(clockwise, 1 + j, 5 + j, 7 + j, 3 + j);
		swap(clockwise, a[i][0], a[i][3], a[i][6], a[i][9]);
		swap(clockwise, a[i][1], a[i][4], a[i][7], a[i][10]);
		swap(clockwise, a[i][2], a[i][5], a[i][8], a[i][11]);
	}

	void print() const
	{
		char string[151];
		unsigned int ix = 0;
		string[ix++] = '\n';
		for (int i = 0; i < 3; ++i)
		{
			for (int j = 0; j < 6; ++j)
			{
				string[ix++] = ' ';
				for (int k = 0; k < 3; ++k)
				{
					string[ix++] = '|';
					string[ix++] = '0' + stickers[k + 9 * j + 3 * i];
				}
				string[ix++] = '|';
			}
			string[ix++] = '\n';
		}
		string[ix++] = '\n';
		string[ix++] = ';';
		string[ix++] = '\0';

		std::cout << string << (check() ? "solved" : "scrambled") << std::endl;
	}

	void input_string(char* c)
	{
		int t = -1;
		for (int i = 0, size = strlen(c); i < size; ++i)
		{
			switch (c[i])
			{
			case 'U':case 'u': twist(t = 0, true); break;
			case 'F':case 'f': twist(t = 1, true); break;
			case 'R':case 'r': twist(t = 2, true); break;
			case 'B':case 'b': twist(t = 3, true); break;
			case 'L':case 'l': twist(t = 4, true); break;
			case 'D':case 'd': twist(t = 5, true); break;
			case '2': if (t != -1) { twist(t, true); t = -1; } break;
			case '\'': if (t != -1) { twist(t, false); twist(t, false); t = -1; } break;
			case '0': reset(); break;
			default: break;
			}
		}
	}
};

int main()
{
	rbk cube;

	while (1)
	{
		system("cls");
		cube.print();
		char c[20];
		std::cin >> c;
		cube.input_string(c);
	}
	
	return 0;
}
