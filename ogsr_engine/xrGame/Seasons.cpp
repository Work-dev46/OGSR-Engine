#include "stdafx.h"
#include "Seasons.h"
#include "level.h"
#include "..\xrSound\SoundRender_Core.h"
#include "ui/UILoadingScreen.h"
#include "..\xr_3da\x_ray.h"

LPSTR Seasons::currentSeason = "default";
void Seasons::swithSeason(LPCSTR newSeason, BOOL needReload, shared_str levelName)
{
	if (xr_strcmp(newSeason, currentSeason)) {
		LPCSTR locName = Level().bReady ? Level().name().c_str() : levelName.c_str();
		string_path tmp;
		FS_Path* gte = FS.get_path("$game_textures_ex$");
		FS_Path* lte = FS.get_path("$level_textures_ex$");
		FS_Path* snd = FS.get_path("$sound_ex$");
		gte->_set(xr_strconcat(tmp,  "seasons\\", newSeason, "\\textures"));
		snd->_set(xr_strconcat(tmp, "seasons\\", newSeason, "\\sounds"));
		lte->_set(xr_strconcat(tmp, "seasons\\", newSeason, "\\levels\\", locName));
		FS.rescan_path(gte->m_Path, TRUE);
		FS.rescan_path(lte->m_Path, TRUE);
		FS.rescan_path(snd->m_Path, TRUE);

		currentSeason = (LPSTR) newSeason;

		if (needReload) {
			Device.m_pRender->DeferredLoad(FALSE);
			Device.m_pRender->ResourcesDeferredUpload(TRUE);
			if (Device.b_is_Ready) Device.Reset();

			//SoundRender->reload();// Нормально не работает

#pragma todo("create reload all sounds from FS")
		}
	}
};

void Seasons::save(IWriter& stream)
{
	stream.open_chunk(SEASON_CHUNK_DATA);
	stream.w_stringZ(currentSeason);
	stream.close_chunk();
};

void Seasons::load(IReader& stream, shared_str levelName)
{
	R_ASSERT2(stream.find_chunk(SEASON_CHUNK_DATA), "Can't find chunk SEASON_CHUNK_DATA");
	shared_str saveSeason;
	stream.r_stringZ(saveSeason);

	swithSeason(saveSeason.c_str(), FALSE, levelName);
	currentSeason = (LPSTR) saveSeason.c_str();	
};

void Seasons::reset()
{
	currentSeason = "default";
}