#pragma once

#include <cocos2d.h>

#include "MjCard//MjCardDef.h"

class MjHandCards;
class MjGameCards;


class MjCardsControlPanel : public cocos2d::Layer
{
public:
	MjCardsControlPanel();
	~MjCardsControlPanel();

	CREATE_FUNC(MjCardsControlPanel);

	static cocos2d::Scene * createScene();

	virtual bool init() override;

private:
	MjHandCards * m_pMjHandCards[GAME_PLAYER];

	MjGameCards * m_outGameCards[GAME_PLAYER];
//	MJGameCards*	m_baidaCards;
};

