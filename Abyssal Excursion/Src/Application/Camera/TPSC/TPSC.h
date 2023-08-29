#pragma once
#include "../CameraBase.h"

class TPSC : public CameraBase
{
public:
	TPSC() { Init(); }
	virtual ~TPSC()override {}

	void Init()			override;
	void Update()		override;

private:
	void UpdateRotateByMouse();
	POINT m_FixMousePos;
};