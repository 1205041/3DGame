#pragma once
#include "../CamBase.h"

class TPSCam :public CamBase
{
public:
	TPSCam() { Init(); }
	virtual ~TPSCam() {}

	void Init()			override;
	void PostUpdate()	override;

private:
	void UpdateRotateByMouse();

};