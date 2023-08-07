#pragma once

#include "vec3.h"
#include <vector>
#include <functional>

class Object {
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