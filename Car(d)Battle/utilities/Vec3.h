#pragma once
struct Vec3
{
	float x, y, z;
	Vec3();
	Vec3(float xx, float yy, float zz);
	void operator+=(Vec3 c);
	void operator-=(Vec3 c);
	void operator*=(Vec3 c);
	void operator/=(Vec3 c);
	void operator+=(int c);
	void operator-=(int c);
	void operator*=(int c);
	void operator/=(int c);
	void operator+=(float c);
	void operator-=(float c);
	void operator*=(float c);
	void operator/=(float c);
	void normalize();
	Vec3 to_RAD();
	float length();
};

Vec3 operator+(Vec3 v, int c);
Vec3 operator-(Vec3 v, int c);
Vec3 operator*(Vec3 v, int c);
Vec3 operator/(Vec3 v, int c);
Vec3 operator+(Vec3 v, float c);
Vec3 operator-(Vec3 v, float c);
Vec3 operator*(Vec3 v, float c);
Vec3 operator/(Vec3 v, float c);
Vec3 operator+(Vec3 v, Vec3 c);
Vec3 operator-(Vec3 v, Vec3 c);
Vec3 operator*(Vec3 v, Vec3 c);
Vec3 operator/(Vec3 v, Vec3 c);
bool operator==(Vec3 v, Vec3 c);

Vec3 two_angles_to_Vec3(float x, float y);