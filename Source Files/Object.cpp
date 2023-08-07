#include "Object.h"

//Activate Methods that need it
void Object::update() {
	for (auto func : updatable_elements_) {
		func();
	}
}

//Updatable Methods
void Object::checkForDeath() {
	if (!isImmortal() && Health_ <= 0) kill(0); //Add Reason Check
}

//Base Methods
int			Object::getID()			{ return ID_; }
float		Object::getHealth()		{ return Health_; }
float		Object::getMax_Health()	{ return Max_Health_; }
vec3f Object::getPosition()	{ return Position_; }

//Check Special Ñharacteristic
bool Object::isImmortal() { return Max_Health_ == -1 ? true : false; }	//Max HP = -1 for immortal object
bool Object::isSolid()	{ return isSolid_; }

//Actions
void Object::teleport(vec3f vec) { Position_ = vec; }
void Object::teleport(float x, float z) {
	Position_.x = x;
	Position_.z = z;
}
void Object::teleport(float x, float y, float z) {
	Position_ = { x, y, z };
}
void Object::kill(int reason = 0) {
	Health_ = 0;
	delete this; //IDK is this ok
}