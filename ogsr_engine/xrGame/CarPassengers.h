#pragma once
#include "ai\stalker\ai_stalker.h"

class CarPassengers
{
public: 
	void create(IKinematics* pKinematics);
	const Fmatrix* addPassenger(CAI_Stalker* npc);
	void removePassenger(CAI_Stalker* npc);
	std::map<CAI_Stalker*, const Fmatrix*> getOccupiedPlaces();

private:
	std::list<Fmatrix> list;
	std::map<CAI_Stalker*, const Fmatrix*> occupiedPlaces;
};

