#pragma once
#include "../ObjBase.h"

class Enemy :public ObjBase
{
public:
	Enemy() { Init(); }
	~Enemy() {}

	void Update()					override;
	void PostUpdate()				override;

	void GenerateDepthMapFromLight()override;
	void DrawLit()					override;

	void Init()						override;

	void SetActFlg(const bool _flg) { m_act = _flg; }
private:
	bool m_act = true;
};