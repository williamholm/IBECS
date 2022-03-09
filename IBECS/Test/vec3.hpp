#pragma once
//test class only
class vec3
{
public:
	float x, y, z;
	float magnitude() const { return (sqrt(x * x + y * y + z * z)); }
	float distance(vec3 vec) { return(sqrt((x - vec.x) * (x - vec.x) + (y - vec.y) * (y - vec.y) + (z - vec.z) * (z - vec.z))); }
	void normalise()
	{
		float magnitude = this->magnitude();
		x /= magnitude;
		y /= magnitude;
		z /= magnitude;
	}
	bool operator< (const vec3& vec) //sort by x position, then y, then z.
	{
		if (this->x == vec.x)
		{
			if (this->y == vec.y)
			{
				return this->z < vec.z;
			}
			else
			{
				return this->y < vec.y;
			}
		}
		return this->x < vec.x;
	}

	vec3 scalarMulti(float scalar)
	{
		return vec3(x * scalar, y * scalar, z * scalar);
	}
	vec3 operator+=(const vec3& vec)
	{
		x += vec.x;
		y += vec.y;
		z += vec.z;
		return *this;
	}
	vec3(float x, float y, float z) :x(x), y(y), z(z) {}
	vec3() = default;
};
