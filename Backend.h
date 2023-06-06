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

	vec3f(float, float, float);

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



class Hitbox {
protected:
	vec3f* Position_;
public:
};

namespace HitBox {

	class Cylinder : Hitbox {
		//friend class Hitbox;
	public:
		float Height_ = 1;
		float Radius_ = 1;

		Cylinder(float radius, float height, vec3f* position) {
			Position_ = position;
			Height_ = height;
			Radius_ = radius;
		};
		Cylinder(vec3f* pos) {
			Position_ = pos;
		}

		bool isColliding(Cylinder cyl) {
			bool collXZ = this->Position_->distanceTo(*cyl.Position_) <= (Radius_ + cyl.Radius_);
			bool collY = ((this->Position_->y >= (cyl.Position_->y - cyl.Height_)) && ((this->Position_->y - this->Height_) <= cyl.Position_->y));
			return collXZ && collY;
		}
	};

	class Cube : Hitbox {
		//friend class Hitbox;
	public:
		float Height_ = 1;
		float Radius_ = 1;

		Cube(float radius, float height, vec3f* position) {
			Position_ = position;
			Height_ = height;
			Radius_ = radius;
		};
		Cube(vec3f* pos) {
			Position_ = pos;
		}

		bool isColliding(Cube cyl) {
			bool collXZ = this->Position_->distanceTo(*cyl.Position_) <= (Radius_ + cyl.Radius_);
			bool collY = ((this->Position_->y >= (cyl.Position_->y - cyl.Height_)) && ((this->Position_->y - this->Height_) <= cyl.Position_->y));
			return collXZ && collY;
		}
	};
}


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