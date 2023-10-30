#pragma once
#include "../../TextDraw.h"

class BackDrop :public TextDraw
{
public:
	BackDrop() { Init(); }
	~BackDrop() {}

	void Init()	override;
private:
	
};