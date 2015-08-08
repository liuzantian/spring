/* This file is part of the Spring engine (GPL v2 or later), see LICENSE.html */

/* based on original los code in LosHandler.{cpp,h} and RadarHandler.{cpp,h} */

#ifndef LOS_MAP_H
#define LOS_MAP_H

#include <vector>
#include "System/type2.h"
#include "System/myMath.h"


struct SLosInstance;


/// map containing counts of how many units have Line Of Sight (LOS) to each square
class CLosMap
{
	CR_DECLARE_STRUCT(CLosMap)

public:
	CLosMap(int2 size_, bool sendReadmapEvents_, const float* heightmap_)
	: size(size_)
	, map(size.x * size.y, 0)
	, sendReadmapEvents(sendReadmapEvents_)
	, heightmap(heightmap_)
	{ }

public:
	/// circular area, for airLosMap, circular radar maps, jammer maps, ...
	void AddCircle(SLosInstance* instance, int amount);

	/// arbitrary area, for losMap, non-circular radar maps, ...
	void AddRaycast(SLosInstance* instance, int amount);

public:
	int operator[] (int square) const { return map[square]; }

	int At(int2 p) const {
		p.x = Clamp(p.x, 0, size.x - 1);
		p.y = Clamp(p.y, 0, size.y - 1);
		return map[p.y * size.x + p.x];
	}

	// FIXME temp fix for CBaseGroundDrawer and AI interface, which need raw data
	unsigned short& front() { return map.front(); }

private:
	void LosAdd(int2 pos, int radius, float baseHeight, std::vector<int>& squares);
	void UnsafeLosAdd(int2 pos, int radius, float baseHeight, std::vector<int>& squares);
	void SafeLosAdd(int2 pos, int radius, float baseHeight, std::vector<int>& squares);

	inline void CastLos(std::vector<int>* squares, float* maxAng, const int2 pos, const int2 off, const float invR, float losHeight, std::vector<bool>& squaresMap, const int radius) const;

protected:
	int2 size;
	std::vector<unsigned short> map;
	bool sendReadmapEvents;
	const float* const heightmap;
};

#endif // LOS_MAP_H
