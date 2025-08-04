#pragma once
#include "include.h"
#include <corecrt_math_defines.h>




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
FMinimalViewInfo POV;


#define PI 3.14159265358979323846f

D3DXMATRIX Matrix(fvector rot, fvector origin = fvector(0, 0, 0))
{
	float radPitch = (rot.x * float(M_PI) / 180.f);
	float radYaw = (rot.y * float(M_PI) / 180.f);
	float radRoll = (rot.z * float(M_PI) / 180.f);

	float SP = sinf(radPitch);
	float CP = cosf(radPitch);
	float SY = sinf(radYaw);
	float CY = cosf(radYaw);
	float SR = sinf(radRoll);
	float CR = cosf(radRoll);

	D3DMATRIX matrix;
	matrix.m[0][0] = CP * CY;
	matrix.m[0][1] = CP * SY;
	matrix.m[0][2] = SP;
	matrix.m[0][3] = 0.f;

	matrix.m[1][0] = SR * SP * CY - CR * SY;
	matrix.m[1][1] = SR * SP * SY + CR * CY;
	matrix.m[1][2] = -SR * CP;
	matrix.m[1][3] = 0.f;

	matrix.m[2][0] = -(CR * SP * CY + SR * SY);
	matrix.m[2][1] = CY * SR - CR * SP * SY;
	matrix.m[2][2] = CR * CP;
	matrix.m[2][3] = 0.f;

	matrix.m[3][0] = origin.x;
	matrix.m[3][1] = origin.y;
	matrix.m[3][2] = origin.z;
	matrix.m[3][3] = 1.f;

	return matrix;
}



