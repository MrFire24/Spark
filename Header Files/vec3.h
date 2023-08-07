#pragma once

#include <iostream>

class vec3f {
public:
	float x = 0, y = 0, z = 0;

	vec3f(float x, float y, float z);

	void normalize();
	vec3f get_norm();
	float length();
	float distanceTo(vec3f);
	//void set(float x, float y, float z);

	void operator=(vec3f vec);
	void operator=(float num);

	vec3f operator+(vec3f vec);
	vec3f operator-(vec3f vec);

	vec3f operator*(float num) const;
	vec3f operator/(float num) const;

	friend std::ostream& operator<<(std::ostream& stream, const vec3f& object) {
		stream << object.x << " " << object.y << " " << object.z;
		return stream;
	}
};