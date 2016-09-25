#include "MjHandCards.h"

#include "MjCard/MjCardSprite.h"

MjHandCards::MjHandCards() :m_fallCardsHSpace(10), m_outCardSprite(NULL), m_fallCardHSpace(10), m_touchHSpace(150)
{
}


MjHandCards::~MjHandCards()
{
}

bool MjHandCards::init()
{
	if (!Layout::init())
	{
		return false;
	}

	return true;
}


// MjHandCards* MjHandCards::createWithGameCards(MJGameCards* mjGameCards)
// {
// 	MjHandCards* majiang = (MjHandCards*)mjGameCards->clone();
// 	return majiang;
// }

void MjHandCards::AddFallCardChi(std::vector<MJCARD> cbCardData)
{
	MjGameCards* gameCards = createFallCards();
	gameCards->addMjCards(cbCardData);


	resetAllGameCardsVec();
}

void MjHandCards::AddFallCardChi(MJCARD cbCard)
{

	resetAllGameCardsVec();
}

void MjHandCards::AddFallCardMingGang(MJCARD cbCardData)
{
	MjGameCards* gameCards = createFallCards();

	MjCardVct cardVct;
	for (int i = 0; i < 4 ; i++)
	{
		cardVct.push_back(cbCardData);
	}
	gameCards->addMjCards(cardVct);

	updateFallCardGangPostion(gameCards);
	resetAllGameCardsVec();
}

void MjHandCards::AddFallCardPeng(MJCARD cbCardData)
{
	MjGameCards* gameCards = createFallCards();

	MjCardVct cardVct;
	for (int i = 0; i < 3; i++)
	{
		cardVct.push_back(cbCardData);
	}
	gameCards->addMjCards(cardVct);

	resetAllGameCardsVec();
}

void MjHandCards::AddFallCardAnGang(MJCARD cbCardData)
{
	MjGameCards* gameCards = createFallCards();
	gameCards->setStatus(MjStatus_Back);
	MjCardVct cardVct;
	for (int i = 0; i < 4; i++)
	{
		cardVct.push_back(cbCardData);
	}
	gameCards->addMjCards(cardVct);

	updateFallCardGangPostion(gameCards);

	resetAllGameCardsVec();
}

void MjHandCards::AddFallCardBuGang(MJCARD cbCardData)
{
	bool success = false;
	for (int i = 0; i < m_mJGameCardsVct.size(); i++)
	{
		MjGameCards* mJGameCards = m_mJGameCardsVct.at(i);
		if (mJGameCards->getMjcardByIndex(0) == cbCardData)
		{
			mJGameCards->addMjCard(cbCardData);
			updateFallCardGangPostion(mJGameCards);
			success = true;
			break;
		}
	}

	if (!success)
	{
		AddFallCardMingGang(cbCardData);
	}
	resetAllGameCardsVec();
}

void MjHandCards::ResetBuGang2Peng(MJCARD cbCardData)
{
	for (int i = 0; i < m_mJGameCardsVct.size(); i++)
	{
		MjGameCards* mJGameCards = m_mJGameCardsVct.at(i);
		if (((MjCardSprite*)mJGameCards->getChildren().at(0))->getMjCard() == cbCardData)
		{
			mJGameCards->removeCardsByIndex(3);
			break;
		}
	}

	resetAllGameCardsVec();
}

MjHandCards* MjHandCards::createWithDirection(emMjDirection mjDirection)
{
	MjHandCards* mJGameCards = MjHandCards::create();
	if (mJGameCards && mJGameCards->initWithDirection(mjDirection))
	{
		return mJGameCards;
	}
	else
	{
		if (mJGameCards)
		{
			delete mJGameCards;
			mJGameCards = NULL;
		}
		return NULL;
	}
}

void MjHandCards::resetAllGameCardsVec()
{
	int zOrder = 10;
	
	int fallCardsCount = m_mJGameCardsVct.size();
	float positionX = 0;
	for (int i = 0; i < fallCardsCount; i++)
	{
		m_mJGameCardsVct.at(i)->setPositionX(positionX);
		positionX = (i + 1) * (m_fallCardHSpace * 3 + m_fallCardsHSpace);
		if (m_mjDirection == MjDir_Left)
		{
			m_mJGameCardsVct.at(i)->setZOrder(i);
		}
	}
	m_allMjCards->setPositionX(positionX);
	if (m_mjDirection == MjDir_Left)
	{
		m_allMjCards->setZOrder(fallCardsCount);
	}

}

bool MjHandCards::initWithDirection(emMjDirection mjDirection)
{
	if (!MjGameCards::initWithDirection(mjDirection))
	{
		return false;
	}

	return true;
}

void MjHandCards::updateFallCardGangPostion(MjGameCards* mJGameCards)
{
	MjCardSprite* mjSpriteLast;
	MjCardSprite* mjSpriteMid;
	for (int i = 0; i < mJGameCards->getAllMjCards()->getChildrenCount(); i++)
	{
		MjCardSprite * mjSprite = (MjCardSprite*)mJGameCards->getAllMjCards()->getChildren().at(i);
		if (mjSprite->getHoriIndex() == 3)
		{
			mjSpriteLast = mjSprite;
		}
		if (mjSprite->getHoriIndex() == 1)
		{
			mjSpriteMid = mjSprite;
		}
	}
	
	if (m_mjDirection == MjDir_Left || m_mjDirection == MjDir_Right)
	{
		mjSpriteLast->setPosition(mjSpriteMid->getPositionX() + m_gangSpace, mjSpriteMid->getPositionY());
	}
	else
	{
		mjSpriteLast->setPosition(mjSpriteMid->getPositionX(), mjSpriteMid->getPositionY() + m_gangSpace);
	}
	int maxZOder = 50;
	mjSpriteLast->setLocalZOrder(maxZOder);
}

MjGameCards* MjHandCards::createFallCards()
{
	MjGameCards* gameCards = MjGameCards::createWithFile(m_cardImageFileName.c_str());
	addChild(gameCards);
	gameCards->setDirection(m_mjDirection);
	gameCards->setCardHSpace(m_fallCardHSpace);			// 牌间距
	gameCards->setCardVSpace(m_dwCardVSpace);			// 行间距
	gameCards->removeAllCards();
	gameCards->setStatus(MjStatus_Face);
	gameCards->setPosition(Vec2(0, 0));
	gameCards->setUserData((void*)"FallCards");
	m_mJGameCardsVct.push_back(gameCards);
	return gameCards;
}

void MjHandCards::ClearFallCard()
{
	for (int i = 0; i < m_mJGameCardsVct.size(); i++)
	{
		m_mJGameCardsVct.at(i)->removeFromParent();
	}
}

void MjHandCards::copySpecialProperties(Widget* model)
{
	MjGameCards::copySpecialProperties(model);
	MjHandCards* mjHandCards = dynamic_cast < MjHandCards*>(model);
	if (mjHandCards)
	{
		m_gangSpace = mjHandCards->m_gangSpace;
		m_fallCardsHSpace = mjHandCards->m_fallCardsHSpace; //不同fallCards中的间距
		m_fallCardHSpace = mjHandCards->m_fallCardHSpace;// 统一fallCards中单张的间距
	}
// 	if (this)
// 	{
// 		for (int i = 0; i < this->getChildrenCount(); i++)
// 		{
// 			MJGameCards* mJGameCards = static_cast<MJGameCards*>(this->getChildren().at(i));
// 			if (mJGameCards)
// 			{
// 				m_mJGameCardsVct.push_back(mJGameCards);
// 			}
// 		}
// 	}
}

Widget* MjHandCards::createCloneInstance()
{
	return  MjHandCards::createWithDirection(m_mjDirection);
}

void MjHandCards::setDirection(emMjDirection val)
{
	MjGameCards::setDirection(val);
	switch (val)
	{
	case MjDir_Bottom:m_gangSpace = 15;
		break;
	case MjDir_Right:m_gangSpace = 10;
		break;
	case MjDir_Top:m_gangSpace = -15;
		break;
	case MjDir_Left:m_gangSpace = -10;
		break;
	}
}

bool MjHandCards::initWithFile(const char* fileImage)
{
	if (!MjGameCards::initWithFile(fileImage))
	{
		return false;
	}

	return true;
}

MjHandCards* MjHandCards::createWithFile(const char* fileImage)
{
	MjHandCards* mJGameCards = MjHandCards::create();
	if (mJGameCards && mJGameCards->initWithFile(fileImage))
	{
		return mJGameCards;
	}
	else
	{
		if (mJGameCards)
		{
			delete mJGameCards;
			mJGameCards = NULL;
		}
		return NULL;
	}
}

bool MjHandCards::onTouchBegan(Touch *touch, Event *unusedEvent)
{
	Vec2 PointTouchBegan = touch->getLocation();
	PointTouchBegan = m_allMjCards->convertToNodeSpace(PointTouchBegan);
	if (m_nSelectIndex == hitTest(PointTouchBegan) && m_nSelectIndex != -1)
	{
		MjCardSprite* cardSpite = getCardSpriteByIndex(m_nSelectIndex);
		if (cardSpite && cardSpite->isShooted())
		{
			MJCARD card = cardSpite->getMjCard();
			std::string filePath = m_cardImageFileName;
			int realBegin = filePath.rfind('/') + 1;
			int realEnd = filePath.rfind('.');
			filePath = filePath.substr(realBegin, realEnd - realBegin);
			m_outCardSprite = MjCardSprite::createWithMjDataAndDir(card, m_mjDirection, m_mjStatus, filePath.c_str());
			m_outCardSprite->setVisible(false);
			m_outCardSprite->setOpacity(200);
			addChild(m_outCardSprite);
			m_outCardSprite->setPosition(PointTouchBegan + m_allMjCards->getPosition());
			m_outCardSprite->setPositionY(m_outCardSprite->getPositionY() + m_touchHSpace);
			m_hasShowOutCards = false;
		}
	}
	else
	{
		m_nSelectIndex = hitTest(PointTouchBegan);
		if (m_nSelectIndex != -1)
		{
			setCardShootByIndex(m_nSelectIndex);
		}
		else
		{
			setCardShootByIndex(-1);
		}
	}

	return true;
}

void MjHandCards::onTouchMoved(Touch *touch, Event *unusedEvent)
{
	Vec2 PointTouchBegan = touch->getLocation();
	PointTouchBegan = m_allMjCards->convertToNodeSpace(PointTouchBegan);
	if (m_outCardSprite)
	{
		if (PointTouchBegan.y > 50)
		{
			m_outCardSprite->setVisible(true);
			m_outCardSprite->setPosition(PointTouchBegan + m_allMjCards->getPosition());
			m_outCardSprite->setPositionY(m_outCardSprite->getPositionY() + m_touchHSpace);
			m_hasShowOutCards = true;
		}
		else 
		{
			m_outCardSprite->setVisible(false);
		}
		
	}
	else
	{
		if (m_nSelectIndex != hitTest(PointTouchBegan))
		{
			if (hitTest(PointTouchBegan) != -1)
			{
				m_nSelectIndex = hitTest(PointTouchBegan);
			}
			if (m_nSelectIndex != -1)
			{
				setCardShootByIndex(m_nSelectIndex);
			}
		}
	}
}

void MjHandCards::onTouchEnded(Touch *touch, Event *unusedEvent)
{
	Vec2 PointTouchBegan = touch->getLocation();
	PointTouchBegan = m_allMjCards->convertToNodeSpace(PointTouchBegan);
	if (m_outCardSprite)
	{
		if ((m_nSelectIndex == hitTest(PointTouchBegan) && !m_hasShowOutCards ) || PointTouchBegan.y > 50)
		{
			NotificationCenter::getInstance()->postNotification("outCards");
			m_outCardSprite->removeFromParent();
			m_outCardSprite = NULL;
		}
		else
		{
			m_outCardSprite->removeFromParent();
			m_outCardSprite = NULL;
			setCardShootByIndex(-1);
		}
	}
	else
	{
		if (hitTest(PointTouchBegan) != -1)
		{
			m_nSelectIndex = hitTest(PointTouchBegan);
		}
		if (m_nSelectIndex != -1)
		{
			setCardShootByIndex(m_nSelectIndex);
		}
	}
}

void MjHandCards::setCardShootByIndex(int index)
{
	for (int i = 0; i < m_allMjCards->getChildrenCount() ; i++)
	{
		MjCardSprite* cardSprite = (MjCardSprite*)m_allMjCards->getChildren().at(i);
		if (i == index)
			cardSprite->setShooted(true);
		else
			cardSprite->setShooted(false);
	}
	updateCardsRect();
}

int MjHandCards::hitTest(Vec2 point)
{
	for (int i = m_allMjCards->getChildrenCount() - 1; i >= 0; i--)
	{
		MjCardSprite* sprite = (MjCardSprite*)m_allMjCards->getChildren().at(i);
		Rect rect = sprite->getBoundingBox();
		if (sprite->isVisible() && sprite->getBoundingBox().containsPoint(point))
			return i;
	}

	return -1;
}

void MjHandCards::addNewCard(MJCARD cbCardData)
{
	if (!cbCardData)
	{
		return;
	}
	MjCardSprite* mjCardSprite =  addMjCard(cbCardData);
	mjCardSprite->setNewCard(true);
	resetMJLayer();
}

void MjHandCards::removeNewCard()
{
	for (int i = 0; i < m_allMjCards->getChildrenCount() ; i++)
	{
		MjCardSprite* mjCardSprite = (MjCardSprite*)m_allMjCards->getChildren().at(i);
		if (mjCardSprite->isNewCard())
		{
			mjCardSprite->removeFromParent();
		}
	}
}

void MjHandCards::removeAllCards()
{
	for (int i = 0; i < m_mJGameCardsVct.size(); i++)
	{
		m_mJGameCardsVct.at(i)->removeFromParent();
	}
	m_mJGameCardsVct.clear();
	m_allMjCards->setPosition(0, 0);
	MjGameCards::removeAllCards();
}

MjCardSprite* MjHandCards::getShootCard(int& index)
{
	for (int i = 0; i < m_allMjCards->getChildrenCount() ; i++)
	{
		MjCardSprite* mj = (MjCardSprite*)m_allMjCards->getChildren().at(i);
		if (mj->isShooted())
		{
			index = i;
			return mj;
		}
	}

	return NULL;
}

void MjHandCards::setFallCardHSpace(float val)
{
	 m_fallCardHSpace = val; 

}
