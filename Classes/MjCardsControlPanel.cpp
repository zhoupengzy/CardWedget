#include "MjCardsControlPanel.h"

#include "MjCard/MjHandCards.h"

USING_NS_CC;

MjCardsControlPanel::MjCardsControlPanel()
{
}


MjCardsControlPanel::~MjCardsControlPanel()
{
}

cocos2d::Scene * MjCardsControlPanel::createScene()
{
	auto scene = Scene::create();

	auto layer = MjCardsControlPanel::create();

	scene->addChild(layer);

	return scene;
}

bool MjCardsControlPanel::init()
{
	if (!Layer::init())
		return false;

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("MjCards/mj_cards_large.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("MjCards/mj_cards_small.plist");

	Point handCardPosition[GAME_PLAYER];
	Point outCardPosition[GAME_PLAYER];

	handCardPosition[0] = Point(200, 100);
	handCardPosition[1] = Point(1100, 210);
	handCardPosition[2] = Point(800, 680);
	handCardPosition[3] = Point(270, 610);
	outCardPosition[0] = Point(550, 300);
	outCardPosition[1] = Point(920, 250);
	outCardPosition[2] = Point(850, 500);
	outCardPosition[3] = Point(480, 550);
	

	MjCardVct cardVct;
	for (int i = 0; i < 9; i++)
	{
		cardVct.push_back(MAKE_MJCARD(MJCARD_COLOR_WAN, i + MJCARD_VALUE_1));
	}
	for (int i = 0; i < GAME_PLAYER; i++)
	{
		if (i == 0)
		{
			m_pMjHandCards[i] = MjHandCards::createWithFile("MjCards/mj_cards_large.png");
			m_pMjHandCards[i]->setContentSize(Size(0, 0));
			m_pMjHandCards[i]->setCardHSpace(90);
			m_pMjHandCards[i]->setCardVSpace(50);
			m_pMjHandCards[i]->setFallCardHSpace(90);
			m_pMjHandCards[i]->setTouchEnabled(true);
			m_pMjHandCards[i]->setScale(0.8);
		}
		else
		{
			m_pMjHandCards[i] = MjHandCards::createWithFile("MjCards/mj_cards_small.png");
			m_pMjHandCards[i]->setCardHSpace(42);
			m_pMjHandCards[i]->setCardVSpace(50);
		}

		if (i == 1)
		{
			m_pMjHandCards[i]->setCardHSpace(30);
			m_pMjHandCards[i]->setFallCardHSpace(40);
			m_pMjHandCards[i]->setScale(0.8);
		}
		else if (i == 0)
		{
			//m_pMjHandCards[i]->setCardVSpace(55);
		}
		else if (i == 3)
		{
			m_pMjHandCards[i]->setCardHSpace(30);
			m_pMjHandCards[i]->setFallCardHSpace(40);
			m_pMjHandCards[i]->setScale(0.8);
		}
		else
		{
			m_pMjHandCards[i]->setCardHSpace(50);
			m_pMjHandCards[i]->setFallCardHSpace(43);
			m_pMjHandCards[i]->setScale(0.7);
		}

		m_pMjHandCards[i]->setMaxHCardsCount(14);
		m_pMjHandCards[i]->setDirection((emMjDirection)i);
		m_pMjHandCards[i]->setPosition(handCardPosition[i]);
		m_pMjHandCards[i]->setStatus(MjStatus_Stand);

		m_outGameCards[i] = MjGameCards::createWithFile("MjCards/mj_cards_small.png");
		m_outGameCards[i]->setContentSize(Size(0, 0));
		m_outGameCards[i]->setCardHSpace(42);
		if (i == 1 || i == 3)
		{
			m_outGameCards[i]->setCardVSpace(60);
			m_outGameCards[i]->setCardHSpace(39);
		}
		else if (i == 0)
		{
			m_outGameCards[i]->setCardVSpace(55);
		}
		else
			m_outGameCards[i]->setCardVSpace(53);

		m_outGameCards[i]->setPosition(outCardPosition[i]);
		m_outGameCards[i]->setMaxHCardsCount(8);
		m_outGameCards[i]->setStatus(MjStatus_Face);
		m_outGameCards[i]->setDirection((emMjDirection)i);
		addChild(m_outGameCards[i]);
		addChild(m_pMjHandCards[i]);
	}
// 	for (int i = GAME_PLAYER - 1; i >= 0; i--)
// 	{
// 		addChild(m_pMjHandCards[i]);
// 	}
// 	m_outBigCard = MjCardSprite::createWithMjDataAndDir(0x51, MjDir_Bottom, MjStatus_Stand, "mj_cards_large");
// 	m_outBigCard->setVisible(false);
// 	addChild(m_outBigCard, 10);

	m_pMjHandCards[0]->addEmptyCard(true);
	m_pMjHandCards[0]->addEmptyCard(true);
	m_pMjHandCards[0]->addEmptyCard(true);
	m_pMjHandCards[0]->addEmptyCard(true);
	m_pMjHandCards[0]->addEmptyCard(true);
	
	return true;
}
