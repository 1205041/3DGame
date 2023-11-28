#pragma once
#include "../../TextDraw.h"

class Player;

class MaxWaterLv :public TextDraw
{
public:
	MaxWaterLv() { Init(); }
	~MaxWaterLv() {}

	void Update()		override;
	void PostUpdate()	override;
	void Init()			override;

	void SetPlayer(const std::shared_ptr<Player>& _player) { m_wpPlayer = _player; }
private:
	std::weak_ptr<Player> m_wpPlayer;

	// 色チェンジ
	bool m_AlphC = false;
};