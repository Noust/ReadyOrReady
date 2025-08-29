#pragma once
#include <cmath>
#include <corecrt_math_defines.h>
#include <DirectX/d3dx9math.h>




struct Vector2 {
	float x, y;

	Vector2 operator+(Vector2 V) {
		return { V.x + x, V.y + y };
	}

	Vector2 operator-(Vector2 V) {
		return { V.x - x, V.y - y };
	}

	Vector2 operator*(Vector2 V) {
		return { V.x * x, V.y * y };
	}

	Vector2 operator/(Vector2 V) {
		return { V.x / x, V.y / y };
	}

	float distance(Vector2 V) {
		return sqrt(pow(V.x - x, 2) + pow(V.y - y, 2));
	}
};

struct Vector3 {
	float x, y, z;

	// Default constructor
	Vector3() : x(0.0f), y(0.0f), z(0.0f) {}

	// Constructor with three float parameters
	Vector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}

	Vector3 operator+(Vector3 V) {
		return { V.x + x, V.y + y, V.z + z };
	}

	Vector3 operator-(Vector3 V) {
		return { V.x - x, V.y - y, V.z - z };
	}

	Vector3 operator*(Vector3 V) {
		return { V.x * x, V.y * y, V.z * z };
	}

	Vector3 operator/(Vector3 V) {
		return { V.x / x, V.y / y, V.z / z };
	}

	float dot(Vector3 V) {
		return x * V.x + y * V.y + z * V.z;
	}

	float distance(Vector3 V) {
		return sqrt(pow(V.x - x, 2) + pow(V.y - y, 2) + pow(V.z - z, 2));
	}
};

struct Vector4 {
	float x, y, z, w;

	Vector4 operator+(Vector4 V) {
		return { V.x + x, V.y + y, V.z + z, V.w + w };
	}

	Vector4 operator-(Vector4 V) {
		return { V.x - x, V.y - y, V.z - z, V.w - w };
	}

	Vector4 operator*(Vector4 V) {
		return { V.x * x, V.y * y, V.z * z, V.w * w };
	}

	Vector4 operator/(Vector4 V) {
		return { V.x / x, V.y / y, V.z / z, V.w / w };
	}

	float distance(Vector4 V) {
		return sqrt(pow(V.x - x, 2) + pow(V.y - y, 2) + pow(V.z - z, 2) + pow(V.w - w, 2));
	}
};

class fvector2d
{
public:
	fvector2d() : x(0.f), y(0.f)
	{

	}

	fvector2d(double _x, double _y) : x(_x), y(_y)
	{

	}
	~fvector2d()
	{

	}


	fvector2d operator-(fvector2d v)
	{
		return fvector2d(x - v.x, y - v.y);
	}


	double x;
	double y;

	inline double distance(fvector2d v)
	{
		return double(sqrtf(powf(v.x - x, 2.0) + powf(v.y - y, 2.0)));
	}
};


class fvector
{
public:
	fvector() : x(0.f), y(0.f), z(0.f)
	{

	}

	fvector(double _x, double _y, double _z) : x(_x), y(_y), z(_z)
	{

	}
	~fvector()
	{

	}

	double x, y, z;

	inline double dot(fvector v)
	{
		return x * v.x + y * v.y + z * v.z;
	}

	inline double distance(fvector v)
	{
		return double(sqrtf(powf(v.x - x, 2.0) + powf(v.y - y, 2.0) + powf(v.z - z, 2.0)));
	}

	inline double length() {
		return sqrt(x * x + y * y + z * z);
	}

	fvector operator+(fvector v)
	{
		return fvector(x + v.x, y + v.y, z + v.z);
	}

	fvector operator-(fvector v)
	{
		return fvector(x - v.x, y - v.y, z - v.z);
	}

	fvector operator/(double flNum)
	{
		return fvector(x / flNum, y / flNum, z / flNum);
	}


	fvector operator*(double flNum) { return fvector(x * flNum, y * flNum, z * flNum); }

};

struct FMinimalViewInfo
{
	fvector Location;
	fvector Rotation;
	float FOV;
};

extern FMinimalViewInfo POV;

#define PI 3.14159265358979323846f

D3DXMATRIX Matrix(fvector rot, fvector origin = fvector(0, 0, 0));



