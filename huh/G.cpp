
#include <iostream>

constexpr float G = 0.00000000006674f;

struct b
{
	float m;
	struct f3 { float x, y, z; } p, v;

	void update()
	{
		p.x += v.x;
		p.y += v.y;
		p.z += v.z;
	}
};

void apply_force(b& b1, const b& b2)
{
	float rx = ((b1.p.x - b2.p.x) * (b1.p.x - b2.p.x));
	float ry = ((b1.p.y - b2.p.y) * (b1.p.y - b2.p.y));
	float rz = ((b1.p.z - b2.p.z) * (b1.p.z - b2.p.z));
	float r = rx + ry + rz;
	float aa = (-1) * G * /**/ b2.m / (r * r * r); // divided by b1.m
	float ax = aa * (b1.p.x - b2.p.x);
	float ay = aa * (b1.p.y - b2.p.y);
	float az = aa * (b1.p.z - b2.p.z);
	b1.v.x += ax;
	b1.v.y += ay;
	b1.v.z += az;
}

std::ostream& operator<<(std::ostream& os, const b& b_)
{
	printf(" m: %10.2f p: %10.2f,%10.2f,%10.2f, v: %10.2f,%10.2f,%10.2f \n", b_.m, b_.p.x, b_.p.y, b_.p.z, b_.v.x, b_.v.y, b_.v.z);
	return os;
}

int main()
{
	b b1{ 1000000,{1,0,0},{0,0,0} };
	b b2{ 1000000,{0,1,0},{0,0,0} };
	b b3{ 1000000,{0,0,1},{0,0,0} };

	while (1)
	{
		std::cout << "\033[3J" << b1 << b2 << b3 << std::endl;
		b1.update();
		b2.update();
		b3.update();
		apply_force(b1, b2);
	}

	return 0;
}
