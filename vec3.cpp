#include "Backend.h"

vec3f::vec3f(float x, float y, float z) : x(x), y(y), z(z) {}

void vec3f::normalize() {
    float length = this->length();
    if (length == 0)
        *this = {0, 0, 0};
    else
        *this = *this / length;
}

vec3f vec3f::get_norm() {
    float length = this->length();
    if (length == 0) {
        return vec3f(1, 1, 1);
    }
    return *this / length;
}

float vec3f::length() {
    return sqrt(x * x + y * y + z * z);
}

float vec3f::distanceTo(vec3f vec) {
    return (*this - vec).length();
}

/*
void vec3f::set(float x, float y, float z) {
    this->x = x;
    this->y = y;
    this->z = z;
}
*/

void vec3f::operator = (vec3f vec) { set(vec.x, vec.y, vec.z); }
void vec3f::operator = (float num) {
    normalize();
    set(x * num, y * num, z * num);
}

vec3f vec3f::operator + (vec3f vec) { return vec3f(this->x + vec.x, this->y + vec.y, this->z + vec.z); }
vec3f vec3f::operator - (vec3f vec) { return vec3f(this->x - vec.x, this->y - vec.y, this->z - vec.z); }

vec3f vec3f::operator * (float num) const { return vec3f(x * num, y * num, z * num); }
vec3f vec3f::operator / (float num) const { return vec3f(x / num, y / num, z / num); }