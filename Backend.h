#pragma once

#include "imgui.h"
#include "imgui-SFML.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <functional>

class vec3f {
public:
	float x = 0, y = 0, z = 0;

	vec3f(float x = 0, float y = 0, float z = 0);

	void normalize();
	vec3f get_norm();
	float length();
	void set(float x, float y, float z);

	void operator+(vec3f vec);
	void operator-(vec3f vec);

	vec3f operator*(float num) const;
	vec3f operator/(float num) const;

	void operator=(vec3f vec);
	void operator=(float num);

	friend std::ostream& operator<<(std::ostream& stream, const vec3f& object) {
		stream << object.x << " " << object.y << " " << object.z;
		return stream;
	}
};


/*
class Hitbox {
private:
public:
	bool isColliding(const Cylinder& cyl1, const Cylinder& cyl2);
};

class Cylinder : Hitbox {
private:
	float Radius_;
	float Height_;
	vec3f* Position_;
	friend class Hitbox;
public:
	Cylinder(float radius, float height, vec3f* position) : Radius_(radius), Height_(height), Position_(position) {};
};

bool Hitbox::isColliding(const Cylinder& cyl1, const Cylinder& cyl2) {
	//bool collXZ = 
	bool collY = ((cyl1.Position_->y >= (cyl2.Position_->y - cyl2.Height_)) && ((cyl1.Position_->y - cyl1.Height_) <= cyl2.Position_->y));
    return true;
} */

class Object{
private:
	//Base Info
	int ID_;	//Temporary
	float Health_;
	float Max_Health_;
	vec3f Position_;

	//List Of Methods that must be Constantly Activated
	std::vector<std::function<void()>> updatable_elements_;	//Not like this 

	//Special Ñharacteristics
	bool isSolid_;

	//Additional Info
	int lastDamageSourse;	//Not Best Solution (int)

	//Updatable Methods
	void checkForDeath();

public:

	//Activate Methods that need it
	void update();

	//Base Methods
	int getID();
	float getHealth();
	float getMax_Health();
    vec3f getPosition();

	//Check Special Ñharacteristic
	bool isImmortal();	//Max HP = -1 for immortal object
	bool isSolid();

	//Actions
	void kill(int);
    void teleport(vec3f);
	void teleport(float, float, float);
	void teleport(float, float);


	

};