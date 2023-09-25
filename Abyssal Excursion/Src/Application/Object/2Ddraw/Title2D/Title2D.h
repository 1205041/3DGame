#pragma once
#include "../2Ddraw.h"

class Title2D :public twoDdraw
{
public:
	Title2D() { Init(); }
	~Title2D() {}

	void PostUpdate()	override;
	void DrawSprite()	override;
	void Init()			override;
private:
	
};