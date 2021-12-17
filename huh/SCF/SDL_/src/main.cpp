

#include "SystemComplexFriendship.hpp"

int main(int argc, char** argv)
{
	SystemComplexFriendship* demo = new SystemComplexFriendship;

	while (demo->Update());

	delete demo;
	demo = nullptr;

	return 0;
}

