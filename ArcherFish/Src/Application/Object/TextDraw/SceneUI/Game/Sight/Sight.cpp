#include "Sight.h"

void Sight::Init()
{
	// テクスチャ表示
	SetText("Asset/Textures/SceneUI/Game/SightTP.png");
	SetPixel({ -16.0f,16.0f });
	SetRecAng({ 0,0,32, 32 });
	SetColor({ 1.0f,1.0f,1.0f,0.5f });
}
