#pragma once
#include "../ObjBase.h"

class CameraBase;

class Player :public ObjBase
{
public:
	Player() {}
	~Player() {}

	void Init()		override;
	void Update()	override;
	void DrawLit()	override;

private:
	
};