#include "Vec3.h"
#include <math.h>
#define TO_RAD 0.01745329252f

void Vec3::operator+=(Vec3 c)
{
	x += c.x;
	y += c.y;
	z += c.z;
}

void Vec3::operator-=(Vec3 c)
{
	x -= c.x;
	y -= c.y;
	z -= c.z;
}

void Vec3::operator*=(Vec3 c)
{
	x *= c.x;
	y *= c.y;
	z *= c.z;
}

void Vec3::operator/=(Vec3 c)
{
	x /= c.x;
	y /= c.y;
	z /= c.z;
}

void Vec3::operator+=(int c)
{
	x += c;
	y += c;
	z += c;
}

void Vec3::operator-=(int c)
{
	x -= c;
	y -= c;
	z -= c;
}

void Vec3::operator*=(int c)
{
	x *= c;
	y *= c;
	z *= c;
}

void Vec3::operator/=(int c)
{
	x /= c;
	y /= c;
	z /= c;
}

void Vec3::operator+=(float c)
{
	x += c;
	y += c;
	z += c;
}

void Vec3::operator-=(float c)
{
	x -= c;
	y -= c;
	z -= c;
}

void Vec3::operator*=(float c)
{
	x *= c;
	y *= c;
	z *= c;
}

void Vec3::operator/=(float c)
{
	x /= c;
	y /= c;
	z /= c;
}

Vec3 Vec3::to_RAD()
{
	Vec3 ret(x, y, z);
	ret *= TO_RAD;
	return ret;
}

Vec3 two_angles_to_Vec3(float angx, float angy)
{
	angx *= TO_RAD;
	angy *= TO_RAD;
	return Vec3(sinf(angy) * cosf(angx), sinf(angx), cosf(angy) * cosf(angx));
}

float Vec3::length()
{
	return sqrtf(x * x + y * y + z * z);
}

void Vec3::normalize()
{
	float len = this->length();
	if (len)
		(*this) /= len;
}

Vec3::Vec3()
{
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
}

Vec3::Vec3(float xx = 0, float yy = 0, float zz = 0)
{
	x = xx;
	y = yy;
	z = zz;
}

Vec3 operator+(Vec3 v, int c)
{
	v.x += c;
	v.y += c;
	v.z += c;
	return v;
}

Vec3 operator-(Vec3 v, int c)
{
	v.x -= c;
	v.y -= c;
	v.z -= c;
	return v;
}

Vec3 operator*(Vec3 v, int c)
{
	v.x *= c;
	v.y *= c;
	v.z *= c;
	return v;
}

Vec3 operator/(Vec3 v, int c)
{
	v.x /= c;
	v.y /= c;
	v.z /= c;
	return v;
}

Vec3 operator+(Vec3 v, float c)
{
	v.x += c;
	v.y += c;
	v.z += c;
	return v;
}

Vec3 operator-(Vec3 v, float c)
{
	v.x -= c;
	v.y -= c;
	v.z -= c;
	return v;
}

Vec3 operator*(Vec3 v, float c)
{
	v.x *= c;
	v.y *= c;
	v.z *= c;
	return v;
}

Vec3 operator/(Vec3 v, float c)
{
	v.x /= c;
	v.y /= c;
	v.z /= c;
	return v;
}

Vec3 operator+(Vec3 v, Vec3 c)
{
	v.x += c.x;
	v.y += c.y;
	v.z += c.z;
	return v;
}

Vec3 operator-(Vec3 v, Vec3 c)
{
	v.x -= c.x;
	v.y -= c.y;
	v.z -= c.z;
	return v;
}

Vec3 operator*(Vec3 v, Vec3 c)
{
	v.x *= c.x;
	v.y *= c.y;
	v.z *= c.z;
	return v;
}

Vec3 operator/(Vec3 v, Vec3 c)
{
	v.x /= c.x;
	v.y /= c.y;
	v.z /= c.z;
	return v;
}

bool operator==(Vec3 v, Vec3 c)
{
	return v.x == c.x && v.y == c.y && v.z == c.z;
}
