#pragma once
#include "../../../TextDraw.h"

class Sight :public TextDraw
{
public:
	Sight() { Init(); }
	~Sight() {}

	void Init()	override;
private:
	
};