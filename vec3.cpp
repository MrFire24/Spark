#include "Backend.h"

vec3f::vec3f(float x, float y, float z) : x(x), y(y), z(z) {}

void vec3f::normalize() {
    float length = this->length();
    if (length == 0)
        set(1, 1, 1);
    else
        set(x / length, y / length, z / length);
}

vec3f vec3f::get_norm() {
    float length = this->length();
    if (length == 0) {
        return vec3f(1, 1, 1);
    }
    return vec3f(x / length, y / length, z / length);
}

float vec3f::length() {
    return sqrt(x * x + y * y + z * z);
}

void vec3f::set(float x, float y, float z) {
    this->x = x;
    this->y = y;
    this->z = z;
}

void vec3f::operator + (vec3f vec) { set(this->x + vec.x, this->y + vec.y, this->z + vec.z); }
void vec3f::operator - (vec3f vec) { set(this->x - vec.x, this->y - vec.y, this->z - vec.z); }

vec3f vec3f::operator * (float num) const { return vec3f(x * num, y * num, z * num); }
vec3f vec3f::operator / (float num) const { return vec3f(x / num, y / num, z / num); }

void vec3f::operator = (vec3f vec) { set(vec.x, vec.y, vec.z); }
void vec3f::operator = (float num) {
    normalize();
    set(x * num, y * num, z * num);
}