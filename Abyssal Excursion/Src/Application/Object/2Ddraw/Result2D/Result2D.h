#pragma once
#include "../2Ddraw.h"

class Result2D :public twoDdraw
{
public:
	Result2D() { Init(); }
	~Result2D() {}

	void PostUpdate()	override;
	void DrawSprite()	override;
	void Init()			override;
private:
	
};