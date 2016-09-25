#include "MJGameCards.h"

#include "MjCardSprite.h"


MjGameCards::MjGameCards() :m_dwCardHSpace(0), m_dwOffHSpace(-10), m_maxHCardsCount(17), m_dwCardVSpace(0), m_allMjCards(NULL),
m_mjStatus(MjStatus_Face), m_dwShootAltitude(40), m_nSelectIndex(-1), m_dwNewCatchSpace(10), m_mjDirection(MjDir_Bottom)
{
}


MjGameCards::~MjGameCards()
{
}

MjGameCards* MjGameCards::createWithDirection(emMjDirection mjDirection)
{
	MjGameCards* mJGameCards = MjGameCards::create();
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

bool MjGameCards::initWithDirection(emMjDirection mjDirection)
{
	setContentSize(Size(0, 0));
/*	loadCardsTextures()*/
	m_allMjCards = SpriteBatchNode::create("MjCards/mj_cards_small.png");
	m_allMjCards->getTexture()->setAliasTexParameters();
	setDirection(mjDirection);
	this->addChild(m_allMjCards);
	return true;
}

bool MjGameCards::init()
{
	if (!Layout::init())
	{
		return false;
	}

	return true;
}

MjCardVct MjGameCards::getAllMjCardData()
{
	MjCardVct mjCardDatas;
	for (int i = 0; i < m_allMjCards->getChildrenCount(); i++)
	{
		MjCardSprite* mjCardSprite = (MjCardSprite*)m_allMjCards->getChildren().at(i);
		if (mjCardSprite)
		{
			mjCardDatas.push_back(mjCardSprite->getMjCard());
		}
	}

	return mjCardDatas;
}

void MjGameCards::addMjCards(MjCardVct mjCards)
{
	for (int i = 0; i < mjCards.size(); i++)
	{
		addMjCard(mjCards[i]);
	}
}

MjCardSprite* MjGameCards::addMjCard(MJCARD mjCard, bool newCard)
{
	if (!mjCard)
	{
		return NULL;
	}

	std::string filePath = m_cardImageFileName;
	int realBegin = filePath.rfind('/') + 1;
	int realEnd = filePath.rfind('.');
	filePath = filePath.substr(realBegin, realEnd - realBegin);
	
	auto mjCards = MjCardSprite::createWithMjDataAndDir(mjCard, m_mjDirection, m_mjStatus, filePath.c_str());
	m_allMjCards->addChild(mjCards);
	mjCards->setNewCard(newCard);

	int lineIndex = 0;
	float lastPositonX = - m_dwCardHSpace;
	MjCardSprite* lastCardSprite = NULL;

	int maxLine = 2;
	int lastZoder = 0;
	int vIndex = 0;

	vIndex = (m_allMjCards->getChildrenCount() -1) % m_maxHCardsCount;
	lineIndex = (m_allMjCards->getChildrenCount() - 1) / m_maxHCardsCount;
	mjCards->set2DIndex(vIndex,lineIndex);

	mjCards->setPosition(vIndex * m_dwCardHSpace, -lineIndex * m_dwCardVSpace);
	//CCLOG("mjCards->setPositionx = %f,mjCards->setPositionY = %f", mjCards->getPositionX(), mjCards->getPositionY());
	if (newCard)
	{
		mjCards->setPositionX(mjCards->getPositionX() + m_dwNewCatchSpace);
	}

	if (m_mjDirection == MjDir_Right)
	{
		mjCards->setZOrder(m_maxHCardsCount* (maxLine - lineIndex) - vIndex);
	}
	else if(m_mjDirection == MjDir_Top)
	{
		mjCards->setZOrder(m_maxHCardsCount* (maxLine - lineIndex) + vIndex);
		//CCLOG("TopZorder %d ",mjCards->getZOrder());
	}
	else if (m_mjDirection == MjDir_Bottom)
	{
		mjCards->setZOrder(m_maxHCardsCount*  lineIndex + vIndex);
	}
	else if (m_mjDirection == MjDir_Left)
	{
		mjCards->setZOrder(m_maxHCardsCount* lineIndex + vIndex);
	}

	if (mjCard == m_hunCard)
	{
		mjCards->setHunCard(mjCards);
	}

	mjCards->setDirection(m_mjDirection);
	mjCards->setStatus(m_mjStatus);
	mjCards->updateSpriteView();

	return mjCards;
}

void MjGameCards::resetMJLayer()
{
	auto parent = dynamic_cast<MjGameCards*>(getParent());
	if (!parent)
		setRotation(-m_mjDirection * 90);
	else
		setRotation(0);

	sortAllGameCards();
}

void MjGameCards::sortAllGameCards()
{
	if (!m_allMjCards || m_allMjCards->getChildrenCount() == 0)
		return;

	MjCardVct cards = getAllMjCardData();

// 	if (!sortCards(cards))
// 		return;

	std::set<MjCardSprite*> reorderedSprites;
	MjCardSprite* cardSprite = NULL;
	Vector<Node*> children = m_allMjCards->getChildren();
	for (int i = 0; i < cards.size(); i++)
	{
		for (int j = 0; j < children.size(); ++j)
		{
			cardSprite = (MjCardSprite*)children.at(j);
			if (cardSprite->getMjCard() == cards[i] &&
				reorderedSprites.find(cardSprite) == reorderedSprites.end())
			{
				cardSprite->setZOrder(i);
				reorderedSprites.insert(cardSprite);
				break;
			}
		}
	}

//	m_allMjCards->reorderBatch(true);
/*	m_allMjCards->sortAllChildren();*/
	adjustAllCardSpriteSpace();
}

void MjGameCards::adjustAllCardSpriteSpace()
{
// 	for (int i = 0; i < m_allMjCards->getChildrenCount(); i++) \
// 	{
// 		adjustCardSpriteSpace((MjCardSprite*)m_allMjCards->getChildren().at(i));
// 	}
	updateCardsRect();
}

void MjGameCards::adjustCardSpriteSpace(MjCardSprite* mjCardSprite)
{

}

void MjGameCards::updateCardsRect()
{
	float lastPositonX = 0;
	int lineIndex = 0;
	float CardHSpace = 0;
	int maxZoder = m_allMjCards->getChildrenCount();
	MjCardSprite* newMjCardSprite = NULL;

	int maxLine = 2;
	for (int i = 0; i < m_allMjCards->getChildrenCount(); i++)
	{
		MjCardSprite* mjCardSprite = (MjCardSprite*)m_allMjCards->getChildren().at(i);
		mjCardSprite->setPosition(mjCardSprite->getHoriIndex()* m_dwCardHSpace, -mjCardSprite->getVertIndex()*m_dwCardVSpace);

		if (m_mjDirection == MjDir_Right)
		{
			mjCardSprite->setZOrder(m_maxHCardsCount* (maxLine - mjCardSprite->getVertIndex()) - mjCardSprite->getHoriIndex());
		}
		else if (m_mjDirection == MjDir_Top)
		{
			mjCardSprite->setZOrder(m_maxHCardsCount* (maxLine - mjCardSprite->getVertIndex()) + mjCardSprite->getHoriIndex());
/*			CCLOG("TopZorder %d ", mjCardSprite->getZOrder());*/
		}
		else if (m_mjDirection == MjDir_Bottom)
		{
			mjCardSprite->setZOrder(m_maxHCardsCount* mjCardSprite->getVertIndex() + mjCardSprite->getHoriIndex());
		}
		else if (m_mjDirection == MjDir_Left)
		{
			mjCardSprite->setZOrder(m_maxHCardsCount*  mjCardSprite->getVertIndex() + mjCardSprite->getHoriIndex());
		}

		mjCardSprite->setDirection(m_mjDirection);
		mjCardSprite->setStatus(m_mjStatus);
		mjCardSprite->updateSpriteView();

		if (mjCardSprite->isNewCard())
		{
			mjCardSprite->setPositionX(mjCardSprite->getPositionX() + m_dwNewCatchSpace);
		}

		if (mjCardSprite->isShooted())
		{
			mjCardSprite->setPositionY(-mjCardSprite->getVertIndex()*m_dwCardVSpace + m_dwShootAltitude);
		}
	}
}

void MjGameCards::setAnchorPoint(const Vec2& anchorPoint)
{
	Layout::setAnchorPoint(anchorPoint);
// 	if (m_allMjCards)
// 	{
// 		m_allMjCards->setAnchorPoint(anchorPoint);
// 	}
}

Widget* MjGameCards::createCloneInstance()
{
	return  MjGameCards::createWithDirection(m_mjDirection);
}

void MjGameCards::copySpecialProperties(Widget* model)
{

	MjGameCards* mJGameCards = static_cast<MjGameCards*>(model);
	if (mJGameCards)
	{
		m_mjDirection = mJGameCards->m_mjDirection;

		m_dwCardHSpace = mJGameCards->m_dwCardHSpace;			// 牌间距
		m_dwCardVSpace = mJGameCards->m_dwCardVSpace;			// 行间距
		m_dwShootAltitude = mJGameCards->m_dwShootAltitude;		// 弹起高度
		m_dwMaxLenth = mJGameCards->m_dwMaxLenth;			// 最大长度
		m_dwNewCatchSpace = mJGameCards->m_dwNewCatchSpace;		// 新抓牌间距
		m_nSelectIndex = mJGameCards->m_nSelectIndex;			// 当前已点击牌索引
		m_dwOffHSpace = mJGameCards->m_dwOffHSpace;			// 牌间距
		m_maxHCardsCount = mJGameCards->m_maxHCardsCount;		//
		m_mjStatus = mJGameCards->m_mjStatus;
		m_cardImageFileName = mJGameCards->m_cardImageFileName;

		addMjCards(mJGameCards->getAllMjCardData());
	}
}

void MjGameCards::setDirection(emMjDirection val)
{
	if (m_mjDirection == val)
		return;
	
	m_mjDirection = val;

	resetMJLayer();
}

void MjGameCards::setStatus(emMjStatus val)
{
	if (m_mjStatus == val)
		return;
		
	m_mjStatus = val;
	for (int i = 0; i < m_allMjCards->getChildrenCount() ; i++)
	{
		MjCardSprite* mjcard = (MjCardSprite*)m_allMjCards->getChildren().at(i);
		mjcard->setStatus(m_mjStatus);
	}
	resetMJLayer();
}

void MjGameCards::setCardHSpace(float val)
{
	m_dwCardHSpace = val; 

	resetMJLayer();
}

void MjGameCards::setCardVSpace(float val)
{
	m_dwCardVSpace = val; 
	resetMJLayer();
}

void MjGameCards::setMjCards(MjCardVct mjCards)
{
	m_allMjCards->removeAllChildren();

	addMjCards(mjCards);
}

void MjGameCards::removeAllCards()
{
	m_allMjCards->removeAllChildren();
}

void MjGameCards::removeCards(MjCardVct mjCards)
{
	for (int i = 0; i < mjCards.size(); i++)
	{
		for (int j = 0; j < m_allMjCards->getChildrenCount(); j++)
		{
			if (mjCards[i] == ((MjCardSprite*)m_allMjCards->getChildren().at(i))->getMjCard())
			{
				m_allMjCards->removeChildAtIndex(j,true);
				break;
			}
		}
	}

	resetMJLayer();
}

void MjGameCards::removeCardsByIndex(int index)
{
	for (int i = 0; i < m_allMjCards->getChildrenCount(); i++)
	{
		auto mj = (MjCardSprite*)m_allMjCards->getChildren().at(i);
		if (index == mj->getHoriIndex() + mj->getVertIndex() * m_maxHCardsCount)
		{
			m_allMjCards->removeChild(mj, true);
		}
	}
	
}

void MjGameCards::loadCardsTextures(const char* texturesPath)
{
	m_allMjCards = SpriteBatchNode::create(texturesPath);
	m_allMjCards->getTexture()->setAliasTexParameters();
	m_cardImageFileName = texturesPath;
}


MjGameCards* MjGameCards::createWithFile(const char* fileImage)
{
	MjGameCards* mJGameCards = MjGameCards::create();
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

bool MjGameCards::initWithFile(const char* fileImage)
{
	if (!Layout::init())
	{
		return false;
	}
	setContentSize(Size(0, 0));
	loadCardsTextures(fileImage);
	this->addChild(m_allMjCards);
	return true;
}

void MjGameCards::removeLastCard()
{
	removeCardsByIndex(m_allMjCards->getChildrenCount()-1);
}

void MjGameCards::setEmptyCards(int count)
{
	MjCardVct mjCards;
	for (int i = 0; i < count; i++)
	{
		MJCARD mj = MAKE_MJCARD(MJCARD_COLOR_HUA, MJCARD_VALUE_1);
		mjCards.push_back(mj);
	//	MjCardSprite* mjSprite = addMjCard(mj);
	}
	setMjCards(mjCards);
}

void MjGameCards::addEmptyCard(bool newCard)
{
	MJCARD mj = MAKE_MJCARD(MJCARD_COLOR_HUA, MJCARD_VALUE_1);
	addMjCard(mj, true);
}

void MjGameCards::removeNewCard()
{
	for (int i = 0; i < m_allMjCards->getChildrenCount() ; i++)
	{
		MjCardSprite* card = (MjCardSprite*)m_allMjCards->getChildren().at(i);
		if (card->isNewCard())
		{
			card->removeFromParent();
			return;
		}
	}
}

void MjGameCards::setLastCardToNewCard()
{
	for (int i = 0; i < m_allMjCards->getChildrenCount(); i++)
	{
		MjCardSprite* mj = getCardsByIndex(i);
		if (i == m_allMjCards->getChildrenCount() -1 )
			mj->setNewCard(true);
		else
			mj->setNewCard(false);
	}

	resetMJLayer();
}

MjCardSprite* MjGameCards::getCardsByIndex(int index)
{
	for (int i = 0; i < m_allMjCards->getChildrenCount(); i++)
	{
		auto mj = (MjCardSprite*)m_allMjCards->getChildren().at(i);
		if (index == mj->getHoriIndex() + mj->getVertIndex() * m_maxHCardsCount)
		{
			return mj;
		}
	}
}

MJCARD MjGameCards::getMjcardByIndex(int index)
{
	return ((MjCardSprite*)(m_allMjCards->getChildren().at(index)))->getMjCard();
}
