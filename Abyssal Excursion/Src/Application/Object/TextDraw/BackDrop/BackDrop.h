#pragma once
#include "../TextBase.h"

class BackDrop :public TextBase
{
public:
	BackDrop() { Init(); }
	~BackDrop() {}

	void Init()			override;
private:
	
};