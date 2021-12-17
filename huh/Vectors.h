#ifndef __VECTORS_H__
#define __VECTORS_H__

#include <math.h>
#include <cmath>

struct vec2
{
	float x;
	float y;

	vec2& operator=(const vec2& v) { if (this != &v) { x = v.x; y = v.y; } return *this; }
	bool operator==(const vec2& v) const { return (x == v.x && y == v.y); }
	bool operator!=(const vec2& v) const { return !(*this == v); }

	vec2& operator+=(const vec2& v) { x += v.x; y += v.y; return *this; }
	const vec2& operator+(const vec2& v) const { return vec2(*this) += v; }
	vec2& operator-=(const vec2& v) { x -= v.x; y -= v.y; return *this; }
	const vec2& operator-(const vec2& v) const { return vec2(*this) -= v; }

	vec2& operator*=(const float v) { x *= v; y *= v; return *this; }
	const vec2& operator*(const float v) const { return vec2(*this) *= v; }
	vec2& operator/=(const float v) { if (v != 0) { x /= v; y /= v; } return *this; }
	const vec2& operator/(const float v) const { return vec2(*this) /= v; }

	float length() const { return sqrtf(x * x + y * y); }

	void normalize() { *this /= length(); }
	vec2& normalized() const { return vec2(*this) /= length(); }

	float dot(const vec2& v) const { return (x * v.x + y * v.y); }

	// if (this) and (v) are normalized so that |(this)| == |(v)| == 1
	float angle(const vec2& v) const { return acosf(dot(v) * length() * v.length()); }



};

struct vec3
{
	float x;
	float y;
	float z;

	vec3& operator=(const vec3& v) { if (this != &v) { x = v.x; y = v.y; z = v.z; } return *this; }
	bool operator==(const vec3& v) const { return (x == v.x && y == v.y && z == v.z); }
	bool operator!=(const vec3& v) const { return !(*this == v); }

	vec3& operator+=(const vec3& v) { x += v.x; y += v.y; z += v.z; return *this; }
	const vec3& operator+(const vec3& v) const { return vec3(*this) += v; }
	vec3& operator-=(const vec3& v) { x -= v.x; y -= v.y; z -= v.z; return *this; }
	const vec3& operator-(const vec3& v) const { return vec3(*this) -= v; }

	vec3& operator*=(const float v) { x *= v; y *= v; z *= v; return *this; }
	const vec3& operator*(const float v) const { return vec3(*this) *= v; }
	vec3& operator/=(const float v) { if (v != 0) { x /= v; y /= v; z /= v; } return *this; }
	const vec3& operator/(const float v) const { return vec3(*this) /= v; }

	float length() const { return sqrtf(x * x + y * y + z * z); }

	void normalize() { *this /= length(); }
	vec3& normalized() const { return vec3(*this) /= length(); }

	float dot(const vec3& v) const { return (x * v.x + y * v.y + z * v.z); }

	// if (this) and (v) are normalized so that |(this)| == |(v)| == 1
	float angle(const vec3& v) const { return acosf(dot(v) * length() * v.length()); }



};

struct vec4
{
	float x;
	float y;
	float z;
	float w;

	vec4& operator=(const vec4& v) { if (this != &v) { x = v.x; y = v.y; z = v.z; w = v.w; } return *this; }
	bool operator==(const vec4& v) const { return (x == v.x && y == v.y && z == v.z && w == v.w); }
	bool operator!=(const vec4& v) const { return !(*this == v); }

	vec4& operator+=(const vec4& v) { x += v.x; y += v.y; z += v.z; w += v.w; return *this; }
	const vec4& operator+(const vec4& v) const { return vec4(*this) += v; }
	vec4& operator-=(const vec4& v) { x -= v.x; y -= v.y; z += v.z; w -= v.w; return *this; }
	const vec4& operator-(const vec4& v) const { return vec4(*this) -= v; }

	vec4& operator*=(const float v) { x *= v; y *= v; z *= v; w *= v; return *this; }
	const vec4& operator*(const float v) const { return vec4(*this) *= v; }
	vec4& operator/=(const float v) { if (v != 0) { x /= v; y /= v; z /= v; w /= v; } return *this; }
	const vec4& operator/(const float v) const { return vec4(*this) /= v; }

	float length() const { return sqrtf(x * x + y * y + z * z + w * w); }

	void normalize() { *this /= length(); }
	vec4& normalized() const { return vec4(*this) /= length(); }

	float dot(const vec4& v) const { return (x * v.x + y * v.y + z * v.z + w * v.w); }

	// if (this) and (v) are normalized so that |(this)| == |(v)| == 1
	float angle(const vec4& v) const { return acosf(dot(v) * length() * v.length()); }



};

#endif
