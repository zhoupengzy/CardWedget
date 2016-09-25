#include "CardSprite.h"
#include "CardView.h"

CardSprite::CardSprite(void)
: m_shooted(false), m_horiSpaceFactor(1.0f), m_card(CARD_NULL), m_destPos(-1, -1),
m_selected(false), m_vertSpaceFactor(1.0f), m_horiFixedSpace(0), m_horiIndex(0), m_vertIndex(0),m_disabled(false),
m_expanded(false), m_cardView(NULL), m_selectColor(Color3B(170, 170, 170)), m_disableColor(Color3B(100, 100, 100)), 
m_shootedOrder(-1)
{
}

CardSprite::~CardSprite(void)
{
}

bool CardSprite::init()
{
	if (!Sprite::init())
	{
		return false;
	}
	
	return true;
}

CardSprite* CardSprite::create(const char* imageFilePrefix, CARD card)
{
	CardSprite *pRet = new CardSprite(); 
	if (pRet&& pRet->init()) 
	{
		pRet->setImageFilePrefix(imageFilePrefix);
		pRet->setCard(card);
		pRet->autorelease();
		return pRet; 
	} 
	else 
	{ 
		delete pRet; 
		pRet = NULL;
		return NULL; 
	} 
}

void CardSprite::setCard(CARD card)
{
	if (m_card == card)
		return;

	m_card = card;

	updateSpriteFrame();
}

void CardSprite::setSelected(bool val)
{
	if (m_selected == val)
		return;
	
	m_selected = val;

	if (m_selected)
		setColor(m_selectColor);
	else
		setColor(Color3B(255, 255, 255));
}

float CardSprite::getHoriRealSpace()
{
	if (m_horiFixedSpace != 0.0f)
	{
		return m_horiFixedSpace;
	}
	else
	{
		if (m_cardView)
		{
			return m_horiSpaceFactor * m_cardView->getCurrentHoriSpace();
		}
		else
		{
			return 0.0f;
		}
	}
}

void CardSprite::setDisabled(bool val)
{
	if (m_disabled == val)
		return;

	m_disabled = val;

	if (m_disabled)
		setColor(m_disableColor);
	else
		setColor(Color3B(255, 255, 255));
}

void CardSprite::setExpanded(bool val)
{
	m_expanded = val;
}

void CardSprite::setDestPos(const Point& pos)
{
	m_destPos = pos;

	stopAllActions();
	if (m_cardView && m_cardView->isMoveAnimationEnabled())
	{
		MoveTo* moveTo = MoveTo::create(0.3, pos);
		EaseExponentialOut* moveToEaseOut = EaseExponentialOut::create(moveTo);
		runAction(moveToEaseOut);
	}
	else
	{
		setPosition(pos);
	}
}

void CardSprite::setNormalPos(const Point& pos)
{
	m_normalPos = pos;
	Point destPos = pos;
	calcDestPos(destPos);
	setDestPos(destPos);
}

void CardSprite::calcDestPos(Point& pos)
{
	if (m_cardView && m_shooted)
	{
		pos.y += m_cardView->getShootAltitude();
	}
}

void CardSprite::setShooted(bool shoot)
{
	if (m_shooted == shoot)
		return;

	m_shooted = shoot;
	m_cardView->reorderCardShootedOrder(this);

	Point destPos = m_normalPos;
	calcDestPos(destPos);
	setDestPos(destPos);
}

float CardSprite::getVertRealSpace()
{
	if (m_cardView)
	{
		return m_vertSpaceFactor * m_cardView->getCurrentVertSpace();
	}
	else
	{
		return 0.0f;
	}
}

void CardSprite::onEnter()
{
	Sprite::onEnter();
}

void CardSprite::setDimensionIndex(int horiIndex, int vertIndex)
{
	m_horiIndex = horiIndex;
	m_vertIndex = vertIndex;
}

void CardSprite::setCardView(CardView* cardView)
{
	m_cardView = cardView;
}

void CardSprite::updateSpriteFrame()
{
	std::string imageFileName = m_imageFilePrefix + "card_%02x.png";
	std::string s = StringUtils::format(imageFileName.c_str(), m_card);
	SpriteFrame* frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(s);
	if (frame == NULL)
	{
		s = m_imageFilePrefix + "card_00.png";
		frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(s);
	}
	frame->getTexture()->setAntiAliasTexParameters();
	setSpriteFrame(frame);
}

void CardSprite::setImageFilePrefix(const char* filePrefix)
{
	m_imageFilePrefix = filePrefix;

	updateSpriteFrame();
}

