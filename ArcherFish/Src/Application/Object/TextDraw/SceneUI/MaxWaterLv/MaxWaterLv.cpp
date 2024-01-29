#include "MaxWaterLv.h"

#include "../../../Player/Player.h"

void MaxWaterLv::Update()
{
	std::shared_ptr<Player> spPlayer = m_wpPlayer.lock();
	if (spPlayer) 
	{ 
		if (spPlayer->GetNowYPos() > 0.0f) { m_AlphC = true; }
		else { m_AlphC = false; }
	}
}

void MaxWaterLv::PostUpdate()
{
	SetColor({ 1.0f,1.0f,1.0f,(float)m_AlphC });
}

void MaxWaterLv::Init()
{
	SetLoadText("Asset/Textures/SceneUI/Game/MaxWaterLv.png");
	SetRectAng({ 0,0, 500, 64 });
}
