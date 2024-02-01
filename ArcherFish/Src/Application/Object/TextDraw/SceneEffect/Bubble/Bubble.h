#pragma once
#include "../../TextDraw.h"

class Bubble :public TextDraw
{
public:
	Bubble() { Init(); }
	virtual ~Bubble() {}

	void Update()		override;
	void Init()			override;
	void DrawSprite()	override;

	void SetPixel(const Math::Vector2& _pixel) { m_pixel.x = _pixel.x, m_pixel.y = _pixel.y; }
	const int GetBubbleNum() const { return m_bubbleNum; }

private:
	static const int m_bubbleNum = 250;		// �A����
	int				 m_radius	 = 30;		// ���a
	bool			 m_fill		 = false;	// �h��ׂ�
};