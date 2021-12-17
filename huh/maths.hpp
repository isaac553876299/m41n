

#ifndef MATHS
#define MATHS

// namespace Maths

struct float_vector_2
{
	static const float_vector_2 zero;
	static const float_vector_2 one;
	float x;
	float y;
	float_vector_2() {};
	float_vector_2(float x_, float y_) : x(x_), y(y_) {}
	float_vector_2(const float_vector_2& copy) :x(copy.x), y(copy.y) {}
	float_vector_2(float_vector_2&& move) noexcept = default;
	float_vector_2 operator-() const { return operator*(-1); }
	float_vector_2 operator=(const float_vector_2& other) { x = other.x; y = other.y; return *this; }
	float_vector_2 operator+(const float_vector_2& other) const { return Add(*this, other); }
	float_vector_2& operator+=(const float_vector_2& other) { *this = Add(*this, other); return *this; }
	float_vector_2 operator-(const float_vector_2& other) const { return Substract(*this, other); }
	float_vector_2& operator-=(const float_vector_2& other) { *this = Substract(*this, other); return *this; }
	float_vector_2 operator*(float scalar) const { return Multiply(*this, scalar); }
	float_vector_2& operator*=(float scalar) { *this = Multiply(*this, scalar); return *this; }
	float_vector_2 operator/(float scalar) const { return Divide(*this, scalar); }
	float_vector_2& operator/=(float scalar) { *this = Divide(*this, scalar); return *this; }
	bool operator==(const float_vector_2& other) const { return x == other.x && y == other.y; }
	bool operator!=(const float_vector_2& other) const { return !operator==(other); }
	static float_vector_2 Add(const float_vector_2& a, const float_vector_2& b) { return float_vector_2(a.x + b.x, a.y + b.y); }
	static float_vector_2 Substract(const float_vector_2& a, const float_vector_2& b) { return float_vector_2(a.x - b.x, a.y - b.y); }
	static float_vector_2 Multiply(const float_vector_2& a, float scalar) { return float_vector_2(a.x * scalar, a.y * scalar); }
	static float_vector_2 Divide(const float_vector_2& a, float scalar) { return (scalar != 0) ? float_vector_2(a.x / scalar, a.y / scalar) : a; }
	static float Length(const float_vector_2& target) { return sqrtf(target.x * target.x + target.y * target.y); }
	static float_vector_2 Normalize(const float_vector_2& target) { float length = Length(target); return (length != 0) ? target / length : zero; }
	static float Dot(const float_vector_2& a, const float_vector_2& b) { return a.x * b.x + a.y * b.y; }
	static float_vector_2 Lerp(const float_vector_2& a, const float_vector_2& b, float alpha) { return a + (b - a) * alpha; }
	static float AngleBetween(const float_vector_2& a, const float_vector_2& b)
	{
		float lengthProduct = Length(a) * Length(b);
		if (lengthProduct != 0)
		{
			float fractionResult = Dot(a, b) / lengthProduct;
			if (fractionResult >= -1.0f && fractionResult <= 1.0f)
				return acosf(fractionResult);
		}
		return 0.0f;
	}
};
const float_vector_2 float_vector_2::zero(0.0f, 0.0f);
const float_vector_2 float_vector_2::one(1.0f, 1.0f);

struct float_vector_3
{
	static const float_vector_3 zero;
	static const float_vector_3 one;
	static const float_vector_3 forward;
	static const float_vector_3 right;
	static const float_vector_3 up;
	float x;
	float y;
	float z;
	float_vector_3() {};
	float_vector_3(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}
	float_vector_3(const float_vector_3& copy) :x(copy.x), y(copy.y), z(copy.z) {}
	float_vector_3(float_vector_3&& move) noexcept = default;
	float_vector_3 operator-() const { return operator*(-1); }
	float_vector_3 operator=(const float_vector_3& other) { x = other.x; y = other.y; z = other.z; return *this; }
	float_vector_3 operator+(const float_vector_3& other) const { return Add(*this, other); }
	float_vector_3& operator+=(const float_vector_3& other) { *this = Add(*this, other); return *this; }
	float_vector_3 operator-(const float_vector_3& other) const { return Substract(*this, other); }
	float_vector_3& operator-=(const float_vector_3& other) { *this = Substract(*this, other); return *this; }
	float_vector_3 operator*(float scalar) const { return Multiply(*this, scalar); }
	float_vector_3& operator*=(float scalar) { *this = Multiply(*this, scalar); return *this; }
	float_vector_3 operator/(float scalar) const { return Divide(*this, scalar); }
	float_vector_3& operator/=(float scalar) { *this = Divide(*this, scalar); return *this; }
	bool operator==(const float_vector_3& other) const { return x == other.x && y == other.y && z == other.z; }
	bool operator!=(const float_vector_3& other) const { return !operator==(other); }
	static float_vector_3 Add(const float_vector_3& a, const float_vector_3& b) { return float_vector_3(a.x + b.x, a.y + b.y, a.z + b.z); }
	static float_vector_3 Substract(const float_vector_3& a, const float_vector_3& b) { return float_vector_3(a.x - b.x, a.y - b.y, a.z - b.z); }
	static float_vector_3 Multiply(const float_vector_3& a, float scalar) { return float_vector_3(a.x * scalar, a.y * scalar, a.z * scalar); }
	static float_vector_3 Divide(const float_vector_3& a, float scalar) { return (scalar != 0) ? float_vector_3(a.x / scalar, a.y / scalar, a.z / scalar) : a; }
	static float Length(const float_vector_3& target) { return sqrtf(target.x * target.x + target.y * target.y + target.z * target.z); }
	static float_vector_3 Normalize(const float_vector_3& target) { float length = Length(target); return (length != 0) ? target / length : zero; }
	static float Dot(const float_vector_3& a, const float_vector_3& b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
	static float_vector_3 Lerp(const float_vector_3& a, const float_vector_3& b, float alpha) { return a + (b - a) * alpha; }
	static float AngleBetween(const float_vector_3& a, const float_vector_3& b)
	{
		float lengthProduct = Length(a) * Length(b);
		if (lengthProduct != 0)
		{
			float fractionResult = Dot(a, b) / lengthProduct;
			if (fractionResult >= -1.0f && fractionResult <= 1.0f)
				return acosf(fractionResult);
		}
		return 0.0f;
	}
	static float Distance(const float_vector_3& a, const float_vector_3& b) { return sqrtf((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y) + (a.z - b.z) * (a.z - b.z)); }
	static float_vector_3 Cross(const float_vector_3& a, const float_vector_3& b) { return float_vector_3(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x); }
};
const float_vector_3 float_vector_3::zero(0.0f, 0.0f, 0.0f);
const float_vector_3 float_vector_3::one(1.0f, 1.0f, 1.0f);
const float_vector_3 float_vector_3::forward(0.0f, 0.0f, 1.0f);
const float_vector_3 float_vector_3::right(1.0f, 0.0f, 0.0f);
const float_vector_3 float_vector_3::up(0.0f, 1.0f, 0.0f);

struct float_vector_4
{
	static const float_vector_4 zero;
	static const float_vector_4 one;
	float x;
	float y;
	float z;
	float w;
	float_vector_4() {};
	float_vector_4(float x_, float y_, float z_, float w_) : x(x_), y(y_), z(z_), w(w_) {}
	float_vector_4(const float_vector_4& copy) :x(copy.x), y(copy.y), z(copy.z), w(copy.w) {}
	float_vector_4(float_vector_4&& move) noexcept = default;
	float_vector_4 operator-() const { return operator*(-1); }
	float_vector_4 operator=(const float_vector_4& other) { x = other.x; y = other.y; z = other.z; w = other.w; return *this; }
	float_vector_4 operator+(const float_vector_4& other) const { return Add(*this, other); }
	float_vector_4& operator+=(const float_vector_4& other) { *this = Add(*this, other); return *this; }
	float_vector_4 operator-(const float_vector_4& other) const { return Substract(*this, other); }
	float_vector_4& operator-=(const float_vector_4& other) { *this = Substract(*this, other); return *this; }
	float_vector_4 operator*(float scalar) const { return Multiply(*this, scalar); }
	float_vector_4& operator*=(float scalar) { *this = Multiply(*this, scalar); return *this; }
	float_vector_4 operator/(float scalar) const { return Divide(*this, scalar); }
	float_vector_4& operator/=(float scalar) { *this = Divide(*this, scalar); return *this; }
	bool operator==(const float_vector_4& other) const { return x == other.x && y == other.y && z == other.z && w == other.w; }
	bool operator!=(const float_vector_4& other) const { return !operator==(other); }
	static float_vector_4 Add(const float_vector_4& a, const float_vector_4& b) { return float_vector_4(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w); }
	static float_vector_4 Substract(const float_vector_4& a, const float_vector_4& b) { return float_vector_4(a.x - b.x, a.y - b.y, a.z - b.z, a.w + b.w); }
	static float_vector_4 Multiply(const float_vector_4& a, float scalar) { return float_vector_4(a.x * scalar, a.y * scalar, a.z * scalar, a.w * scalar); }
	static float_vector_4 Divide(const float_vector_4& a, float scalar) { return (scalar != 0) ? float_vector_4(a.x / scalar, a.y / scalar, a.z / scalar, a.w / scalar) : a; }
	static float Length(const float_vector_4& target) { return sqrtf(target.x * target.x + target.y * target.y + target.z * target.z + target.w * target.w); }
	static float_vector_4 Normalize(const float_vector_4& target) { float length = Length(target); return (length != 0) ? target / length : zero; }
	static float Dot(const float_vector_4& a, const float_vector_4& b) { return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w; }
	static float_vector_4 Lerp(const float_vector_4& a, const float_vector_4& b, float alpha) { return a + (b - a) * alpha; }
};
const float_vector_4 float_vector_4::zero(0.0f, 0.0f, 0.0f, 0.0f);
const float_vector_4 float_vector_4::one(1.0f, 1.0f, 1.0f, 1.0f);

struct float_matrix_3
{
	static const float_matrix_3 identity;
	float data[9];
	float_matrix_3();
	float_matrix_3(float value) { for (float& m : data) m = value; }
	float_matrix_3(float m11, float m12, float m13, float m21, float m22, float m23, float m31, float m32, float m33)
	{
		data[0] = m11; data[1] = m12; data[2] = m13;
		data[3] = m21; data[4] = m22; data[5] = m23;
		data[6] = m31; data[7] = m32; data[8] = m33;
	}
	float_matrix_3(const float_matrix_3& other) { *this = other; }
	float_matrix_3& operator=(const float_matrix_3& other);
	bool operator==(const float_matrix_3& other);
	bool operator!=(const float_matrix_3& other);
	float_matrix_3 operator+(const float_matrix_3& other) const;
	float_matrix_3& operator+=(const float_matrix_3& other);
	float_matrix_3 operator-(const float_matrix_3& other) const;
	float_matrix_3& operator-=(const float_matrix_3& other);
	float_matrix_3 operator*(float scalar) const;
	float_matrix_3& operator*=(float scalar);
	float_vector_3 operator*(const float_vector_3& vector) const;
	float_matrix_3 operator*(const float_matrix_3& other) const;
	float_matrix_3& operator*=(const float_matrix_3& other);
	float_matrix_3 operator/(float scalar) const;
	float_matrix_3& operator/=(float scalar);
	//float_vector_3 operator/(const float_vector_3& vector) const;
	float_matrix_3 operator/(const float_matrix_3& other) const;
	float_matrix_3& operator/=(const float_matrix_3& other);
	//check mxn * nxp, results in mxp
	static float_matrix_3 Add(const float_matrix_3& target, float scalar);
	static float_matrix_3 Add(const float_matrix_3& a, const float_matrix_3& b);
	static float_matrix_3 Substract(const float_matrix_3& target, float scalar);
	static float_matrix_3 Substract(const float_matrix_3& a, const float_matrix_3& b);
	static float_matrix_3 Multiply(const float_matrix_3& target, float scalar);
	static float_matrix_3 Multiply(const float_matrix_3& a, const float_vector_3& b);
	static float_matrix_3 Multiply(const float_matrix_3& a, const float_matrix_3& b)
	{
		float_matrix_3 result;
		for (int i = 0; i < 3; ++i)
		{
			for (int j = 0; j < 3; ++j)
			{
				for (int k = 0; k < 3/*n*/; ++k)
				{
					result.data[i][j] += a.data[i][k] * b.data[k][j];
				}
			}
		}
		return result;
	}
	static float_matrix_3 Divide(const float_matrix_3& target, float scalar);
	static float_matrix_3 Divide(const float_matrix_3& a, const float_matrix_3& b);

	static float Determinant(const float_matrix_3& target);
	static float_matrix_3 Transpose(const float_matrix_3& target);
	static float_matrix_3 Cofactor(const float_matrix_3& target);
	static float_matrix_3 Minor(const float_matrix_3& target);
	static float_matrix_3 Adjoint(const float_matrix_3& target);
	static float_matrix_3 Inverse(const float_matrix_3& target);

	static float_matrix_3 Translation(const float_vector_2& translation);
	static float_matrix_3 Translate(const float_matrix_3& target, const float_vector_2& translation);
	static float_matrix_3 Rotation(float rotation);
	static float_matrix_3 Rotate(const float_matrix_3& target, float rotation);
	static float_matrix_3 Scaling(const float_vector_2& scale);
	static float_matrix_3 Scale(const float_matrix_3& target, const float_vector_2& scale);
};

#endif

