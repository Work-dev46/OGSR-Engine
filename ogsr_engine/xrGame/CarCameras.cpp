#include "stdafx.h"

#ifdef DEBUG
#include "ode_include.h"
#include "../xr_3da/StatGraph.h"
#include "PHDebug.h"
#include "phworld.h"
#endif
#include "alife_space.h"
#include "hit.h"
#include "PHDestroyable.h"
#include "car.h"
#include "actor.h"
#include "cameralook.h"
#include "camerafirsteye.h"
#include "level.h"
#include "../xr_3da/cameramanager.h"

bool CCar::HUDView() const		
{
	return active_camera->tag==ectFirst;
}

void	CCar::cam_Update			(float dt, float fov)
{
	VERIFY(!ph_world->Processing());
	Fvector							P,Da;
	Da.set							(0,0,0);
	//bool							owner = !!Owner();

	XFORM().transform_tiny			(P, current_camera_position);

	switch(active_camera->tag) {
	case ectFirst:
		// rotate head
		if(OwnerActor()) OwnerActor()->Orientation().yaw	= -active_camera->yaw;
		if(OwnerActor()) OwnerActor()->Orientation().pitch	= -active_camera->pitch;
		break;
	case ectChase:					break;
	case ectFree:					break;
	}
	active_camera->f_fov			= fov;
	active_camera->Update			(P,Da);
	Level().Cameras().UpdateFromCamera(active_camera);
}

void	CCar::OnCameraChange		(int type)
{
	if(Owner())
	{
		if	(type==ectFirst)
		{
			Owner()->setVisible(FALSE);
			current_camera_position = m_camera_position;
		}
		else
		{
			current_camera_position = m_camera_position_2;
			if (active_camera->tag == ectFirst) Owner()->setVisible(TRUE);
		}
	}
	
	if (!active_camera||active_camera->tag!=type){
		active_camera	= camera[type];
		if (ectFree==type){
			Fvector xyz;
			XFORM().getXYZi(xyz);
			active_camera->yaw		= xyz.y;
		}
	}

}

