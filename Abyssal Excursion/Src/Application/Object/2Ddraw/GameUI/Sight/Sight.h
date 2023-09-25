#pragma once
#include "../../2Ddraw.h"

class Sight :public twoDdraw
{
public:
	Sight() { Init(); }
	~Sight() {}

	void PostUpdate()	override;
	void DrawSprite()	override;
	void Init()			override;
private:
	
};