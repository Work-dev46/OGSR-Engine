#pragma once
class Seasons
{
public:
	static LPCSTR currentSeason;
	static void swithSeason(LPCSTR newSeason, BOOL needReload);
	static void load(IReader& stream);
	static void save(IWriter& stream);
};

