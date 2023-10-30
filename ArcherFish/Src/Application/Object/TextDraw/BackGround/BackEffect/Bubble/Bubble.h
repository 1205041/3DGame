#pragma once
#include "../../../TextDraw.h"

class Bubble:public TextDraw
{
public:
	Bubble() {}
	~Bubble() {}

	void Update()		override;
	void PostUpdate()	override;
	void Init()			override;
private:

};