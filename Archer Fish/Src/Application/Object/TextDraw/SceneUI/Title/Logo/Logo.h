#pragma once
#include "../../../TextDraw.h"

class Logo :public TextDraw
{
public:
	Logo() { Init(); }
	~Logo() {}

	void Init()		override;
private:
	
};