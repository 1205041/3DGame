#pragma once
#include "../../../TextBase.h"

class TitleLogo :public TextBase
{
public:
	TitleLogo() { Init(); }
	~TitleLogo() {}

	void Update()	override;
	void Init()		override;
private:

};