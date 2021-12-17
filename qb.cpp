

#include <iostream>

int main()
{
	/*
		AaB		EeF		IiJ		MmN		QqR		UuV
		d b		h f		l j		p n		t r		x v
		DcC		HgG		LkK		PoO		TsS		XwW
	*/

	const char* corners[] = { "ANQ","BJM","CFI","DER","GLV","HSU","KPW","OTX" };
	const char* edges[] = { "am","bi","ce","dq","fl","gu","hr","jp","kv","nt","ow","sx" };

	const char* solved[] =
	{
		"ANQ","am","BJM","bi","CFI","ce","DER","dq",
		"fl","GLV","gu","HSU","hr",
		"jp","KPW","kv",
		"nt","OTX","ow",
		"sx",

	};

	const char* colors = "yobrgw";


	return 0;
}

