#pragma once

#include "vec3.h"


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
			return
				this->Position_->distanceTo(*cyl.Position_) <= (this->Radius_ + cyl.Radius_)
				&& (
					this->Position_->y == cyl.Position_->y ?
					1 : this->Position_->y > cyl.Position_->y ?
					(this->Position_->y - cyl.Position_->y) < cyl.Height_ : (cyl.Position_->y - this->Position_->y) < this->Height_);
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