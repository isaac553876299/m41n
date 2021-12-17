#include <iostream>
#include <fstream>
#include <string>

void shorten(std::string& in_path, std::string& out_path)
{
	std::ifstream file_in(in_path);
	std::ofstream file_out(out_path);

	std::string str1, str2;

	for (int i = 0; std::getline(in, str1); ++i)
	{
		switch (i)
		{
		case 0:

			break;
		case 1:
		case 2:
		case 3:
		case 4:
			str2 += str1 += " ; ";
			break;
		case 5:
			i = -1;
			out << str2 << std::endl;
			str2.clear();
			break;
		}
	}

	file_in.close();
	file_out.close();
}

void extract(std::string& in_path, std::string& out_path)
{
	std::ifstream file_in(in_path);
	std::ofstream file_out(out_path);

	std::string str1;

	for (int i = 0; std::getline(in, str1); ++i)
	{
		if (i == 4) out << str1 << std::endl;
		if (i == 6) i = 0;
	}

	file_in.close();
	file_out.close();
}

void apend(std::string& in_path, std::string& ap_path, std::string& out_path)
{
	std::ifstream file_in(in_path);
	std::ifstream file_ap(ap_path);
	std::ofstream file_out(out_path);

	std::string str1, str2;

	for (int i = 0; std::getline(in, str1); ++i)
	{
		if (i == 4)
		{
			std::getline(ap, str2);
			out << str2 << std::endl;
		}
		if (i == 6) i = 0;
		out << str1 << std::endl;
	}

	file_in.close();
	file_ap.close();
	file_out.close();
}

int main()
{


	return 0;
}