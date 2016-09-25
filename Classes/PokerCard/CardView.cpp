#include "CardView.h"
#include "CardSprite.h"

#define DEFAULT_CARD_WIDTH					120
#define DEFAULT_CARD_HEIGHT					159
#define DEFAULT_SHOOT_ALTITUDE				27
#define DEFAULT_TOUCH_EXPAND_COUNT			5

CardView::CardView(void) 
: m_shootAltitude(DEFAULT_SHOOT_ALTITUDE), m_maxWidth(0), m_autoShootDown(true),
m_beginTouchCardIndex(-1), m_endTouchCardIndex(-1),  m_maxHeight(0),
m_touchExpandCount(DEFAULT_TOUCH_EXPAND_COUNT),
m_cardMetricsDirty(true), m_reorderCardDirty(false), m_moveAnimationEnabled(false), 
m_cardSelectColor(Color3B(170, 170, 170)), m_cardDisableColor(Color3B(100, 100, 100)), 
m_lastShootedCardOrder(1), m_reorderShootedCardDirty(false)
{
	setCardSize(Size(DEFAULT_CARD_WIDTH, DEFAULT_CARD_HEIGHT));
}

CardView::~CardView(void)
{
}

CardView * CardView::createWithFile(const char* cardImageFileName)
{
	CardView *pRet = new CardView();
	if (pRet && pRet->initWithFile(cardImageFileName))
	{
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

bool CardView::initWithFile(const char* cardImageFileName)
{
	if (!Layer::init())
	{
		return false;
	}
	
	m_maxWidth = Director::getInstance()->getVisibleSize().width - 140;

	setCardImageFile(cardImageFileName);
	
	m_cardRootNode = SpriteBatchNode::create(cardImageFileName);
	m_cardRootNode->getTexture()->setAliasTexParameters();
	addChild(m_cardRootNode);
	
	return true;
}

void CardView::onTouchesBegan(const std::vector<Touch*>& touches, Event *pEvent)
{
	if (touches.size() == 1)
	{
		Touch* touch = touches[0];
        m_touchBeganPoint = touch->getLocation();
        m_touchBeganPoint = convertToNodeSpace(m_touchBeganPoint);
		m_beginTouchCardIndex = hitTestCard(m_touchBeganPoint);
	}
}

void CardView::onTouchesMoved(const std::vector<Touch*>& touches, Event *pEvent)
{
	if (touches.size() == 1 && m_beginTouchCardIndex != -1)
	{
		Touch* touch = touches[0];
		Point touchPoint ;
        touchPoint = touch->getLocation();
        touchPoint = convertToNodeSpace(touchPoint);
		
		int cardIdex = hitTestCard(touchPoint);
		if (cardIdex != -1)
		{
			setCardsSelect(m_beginTouchCardIndex, m_endTouchCardIndex, false);
			m_endTouchCardIndex = cardIdex;
			setCardsSelect(m_beginTouchCardIndex, m_endTouchCardIndex, true);
		}
	}
}

void CardView::onTouchesEnded(const std::vector<Touch*>& touches, Event *pEvent)
{
	if (touches.size() == 1)
	{
		Touch* touch = touches[0];
		Point touchPoint;
        touchPoint = touch->getLocation();
        touchPoint = convertToNodeSpace(touchPoint);
        
		m_endTouchCardIndex = hitTestCard(touchPoint);
		setCardsSelect(0, m_cardRootNode->getChildrenCount() - 1, false);

		if (m_beginTouchCardIndex != -1 && m_endTouchCardIndex != -1)
		{
			if (m_beginTouchCardIndex == m_endTouchCardIndex)
			{
				CardSprite* pSprite = getCardSprite(m_beginTouchCardIndex);
				if (!pSprite->isExpanded())
				{
					expandCards(m_beginTouchCardIndex, m_touchExpandCount);
				}
			}
			flipCardsShoot(m_beginTouchCardIndex, m_endTouchCardIndex);
			dispatchCardShootChangedEvent();
		}
		else
		{
			if (m_autoShootDown)
			{
				setAllCardsShoot(false);
				dispatchCardShootChangedEvent();

				if ((m_touchBeganPoint.x - touchPoint.x) < -100.0 || (m_touchBeganPoint.x - touchPoint.x) > 100.0)
				{
					adjustAllCardSpriteSpace();
				}
			}
		}

		// 重置数据
		m_beginTouchCardIndex = -1;
		m_endTouchCardIndex = -1;
		m_touchBeganPoint.setPoint(0, 0);
	}
}

void CardView::setMaxWidth(float width)
{
	if (m_maxWidth == width)
		return;

	m_maxWidth = width;

	m_cardMetricsDirty = true;
}

void CardView::setMaxHeight(float height)
{
	if (m_maxHeight == height)
		return;

	m_maxHeight = height;

	m_cardMetricsDirty = true;
}

void CardView::setShootAltitude(float altitude)
{
	if (m_shootAltitude == altitude)
		return;

	m_shootAltitude = altitude;
}

void CardView::setCardSize(const Size& szCard)
{
	m_cardSize = szCard;
	m_minHoriSpace = m_cardSize.width / 5;
	m_maxHoriSpace = m_cardSize.width * 3 / 5;
	m_minVertSpace = m_cardSize.height / 5;
	m_maxVertSpace = m_cardSize.height / 2;
	m_expandHoriSpace = m_cardSize.width * 0.15;

	m_cardMetricsDirty = true;
}

int CardView::getCardCount()
{
	return m_cardRootNode->getChildrenCount();
}

int CardView::getCardsIndex(const CardVector &cards, std::vector<int> &indexs)
{
	indexs.clear();

	CardSprite* cardSprite = NULL;
	Vector<Node*>& children = m_cardRootNode->getChildren();
	for (int i = 0, n = 0; i < children.size() && n < cards.size(); ++i)
	{
		cardSprite = (CardSprite*)children.at(i);
		if (cardSprite->getCard() == cards[n])
		{
			indexs.push_back(cardSprite->getLocalZOrder());
			n++;
		}
	}
	return indexs.size();
}

CARD CardView::getCard(uint16_t index)
{
	if (index >= m_cardRootNode->getChildrenCount())
		return CARD_NULL;


	return ((CardSprite*)m_cardRootNode->getChildren().at(index))->getCard();
}

int CardView::getShootedCardCount()
{
 	int count = 0;
	CardSprite* cardSprite = NULL;
	Vector<Node*>& children = m_cardRootNode->getChildren();
 	for (int i = 0; i < children.size(); ++i)
 	{
		cardSprite = (CardSprite*)children.at(i);
 		if(cardSprite->isShooted())
 			count++;
 	}
 	return count;
}

int	CardView::getCards(CardVector &cards)
{
	Vector<Node*>& children = m_cardRootNode->getChildren();
	cards.resize(children.size());
	for (int i = 0; i < children.size(); ++i)
	{
		cards[i] = ((CardSprite*)children.at(i))->getCard();
	}
	return cards.size();
}

int	CardView::getShootedCards(CardVector &cards)
{
	cards.clear();
    
	CardSprite* cardSprite = NULL;
	Vector<Node*>& children = m_cardRootNode->getChildren();
	for (int i = 0; i < children.size(); ++i)
	{
		cardSprite = (CardSprite*)children.at(i);
		if (cardSprite->isShooted())
			cards.push_back(cardSprite->getCard());
	}
	return cards.size();
}

int CardView::getUnshootedCards(CardVector &cards)
{
	cards.clear();

	CardSprite* cardSprite = NULL;
	Vector<Node*>& children = m_cardRootNode->getChildren();
	for (int i = 0; i < children.size(); ++i)
	{
		cardSprite = (CardSprite*)children.at(i);
		if (!cardSprite->isShooted())
			cards.push_back(cardSprite->getCard());
	}
	return cards.size();
}

void CardView::setCards(const CardVector &cards)
{
	Vector<Node*>& children = m_cardRootNode->getChildren();
	for (int i = 0; i < cards.size(); ++i)
	{
		if (i < children.size())
		{
			CardSprite* cardSprite = (CardSprite *)children.at(i);
			cardSprite->setCard(cards[i]);
			cardSprite->setLocalZOrder(i);
		}
		else
		{
			CardSprite* cardSprite = createCardSprite(cards[i]);
			cardSprite->setLocalZOrder(i);
			m_cardRootNode->addChild(cardSprite, i);
		}
	}
	
	if (children.size() > cards.size())
	{
		for (int j = children.size() - 1; j >= cards.size() ; j--)
		{
			m_cardRootNode->removeChildAtIndex(j, true);
		}
	}
	m_reorderCardDirty = true;
	m_cardMetricsDirty = true;
}

bool CardView::isCardShooted(uint16_t index)
{
	if (index >= m_cardRootNode->getChildrenCount())
		return false;

	return ((CardSprite*)m_cardRootNode->getChildren().at(index))->isShooted();
}

 void CardView::clearCards()
 {
	 m_cardRootNode->removeAllChildren();
 }

int CardView::hitTestCard(Point point)
{
	CardSprite* cardSprite;
	Vector<Node*>& children = m_cardRootNode->getChildren();
	for (Vector<Node*>::reverse_iterator iter = children.rbegin(); iter != children.rend(); ++iter)
    {
		cardSprite = (CardSprite*)(*iter);
		if (cardSprite->isVisible() && cardSprite->getBoundingBox().containsPoint(point) && !cardSprite->isDisabled())
			return cardSprite->getLocalZOrder();
    }

	return -1;
}

void CardView::setAllCardsShoot(bool shoot)
{
	setCardsShoot(0, m_cardRootNode->getChildrenCount() - 1, shoot);
}

CardSprite* CardView::createCardSprite(CARD card)
{
	CardSprite* cardSprite = CardSprite::create(m_cardImageFilePrefix.c_str(), card);
	cardSprite->setCardView(this);
	return cardSprite;
}

void CardView::setCardShoot(CARD card, bool shoot)
{
	CardSprite* cardSprite = findCardSprite(card);
	if (cardSprite != NULL)
	{
		cardSprite->setShooted(shoot);

		m_cardMetricsDirty = true;
	}
}

CardSprite* CardView::getCardSprite(int index)
{
	if (index < 0 || index >= m_cardRootNode->getChildrenCount())
	{
		return NULL;
	}

	return (CardSprite*)m_cardRootNode->getChildren().at(index);
}

CardSprite* CardView::findCardSprite(CARD card)
{
	CardSprite* cardSprite = NULL;
	Vector<Node*>& children = m_cardRootNode->getChildren();
	for (int i = 0; i < children.size() ; i++)
	{
		cardSprite = (CardSprite*)children.at(i);
		if (cardSprite->getCard() == card)
		{
			return cardSprite;
		}
	}
	return NULL;
}
	
void CardView::adjustCardSpriteSpace(CardSprite* pSprite)
{
    pSprite->setHoriFixedSpace(0.0);
    pSprite->setHoriSpaceFactor(1.0);
    pSprite->setExpanded(false);
}

CardSprite* CardView::addCard(CARD card)
{
	return insertCard(m_cardRootNode->getChildrenCount(), card);
}

CardSprite* CardView::insertCard(int index, CARD card)
{
	index = std::max(index, 0);
	index = std::min(index, (int)m_cardRootNode->getChildrenCount());
	
	CardSprite* newCardSprite = createCardSprite(card);
	m_cardRootNode->addChild(newCardSprite, index);
	m_reorderCardDirty = true;
	m_cardMetricsDirty = true;

	return newCardSprite;
}

bool CardView::removeCard(CARD card)
{
	CardSprite* cardSprite = findCardSprite(card);
	if (cardSprite == NULL)
		return false;
		
	m_cardRootNode->removeChild(cardSprite, true);
	
	m_reorderCardDirty = true;
	m_cardMetricsDirty = true;
	return true;
}

void CardView::insertCards(int index, const CardVector &cards)
{
	index = std::max(index, 0);
	index = std::min(index, (int)m_cardRootNode->getChildrenCount());

	for (int i = 0; i < cards.size(); i++)
	{
		m_cardRootNode->addChild(createCardSprite(cards[i]), index + i);
	}

	m_reorderCardDirty = true;
	m_cardMetricsDirty = true;
}

void CardView::removeCards(const CardVector &cards)
{
	for (int i = 0; i < cards.size() ; i++)
	{
		removeCard(cards[i]);
	}
	
	m_reorderCardDirty = true;
	m_cardMetricsDirty = true;
}

void CardView::updateCardMetrics()
{
	Vector<Node*>& children = m_cardRootNode->getChildren();
	if (children.empty())
	{
		return;
	}
	
	int maxRow = (m_maxHeight != 0 && m_minVertSpace != 0) ? ceil(m_maxHeight / m_minVertSpace) : 1;
	float horiFixedSpace = calcAllCardSpriteHoriFixedSpace();
	float horiSpaceFactor = calcAllCardSpriteHoriSpaceFactor();
	Point anchorPoint = getAnchorPoint();
	CardSprite* cardSprite = NULL;

	// 计算单位空间
	m_currentHoriSpace = horiSpaceFactor != 0 ? (m_maxWidth * maxRow - horiFixedSpace) / horiSpaceFactor : 
		(m_maxWidth * maxRow - horiFixedSpace);
	m_currentVertSpace = m_maxHeight / maxRow;
	m_currentHoriSpace = std::min(m_currentHoriSpace, m_maxHoriSpace);
	m_currentHoriSpace = std::max(m_currentHoriSpace, m_minHoriSpace);
	m_currentVertSpace = std::min(m_currentVertSpace, m_maxVertSpace);
	m_currentVertSpace = std::max(m_currentVertSpace, m_minVertSpace);

	// 确定牌的水平/垂直位置及宽度、高度
	std::vector<int> rowWidths;
	float width = 0.0f, height = 0.0f;
	int horiIndex = 0, vertIndex = 0;
	for (Vector<Node*>::iterator iter = children.begin(); iter != children.end(); ++iter)
    {
		cardSprite = (CardSprite*)(*iter);

		if ((width + cardSprite->getHoriRealSpace()) > m_maxWidth + 0.1)
		{
			vertIndex++;
			horiIndex = 0;
			rowWidths.push_back(width);
			width = 0.0f;
			height += cardSprite->getVertRealSpace();
		}
		
		cardSprite->setDimensionIndex(horiIndex, vertIndex);

		if (horiIndex != 0)
		{
			width += cardSprite->getHoriRealSpace();
		}
		horiIndex++;
	}
	rowWidths.push_back(width);
	
	float x = 0.0;
	float y = height * anchorPoint.y;
	vertIndex = -1;
	for (Vector<Node*>::iterator iter = children.begin(); iter != children.end(); ++iter)
	{
		cardSprite = (CardSprite*)(*iter);
		if (cardSprite->getVertIndex() != vertIndex)
		{
			x = -rowWidths[cardSprite->getVertIndex()] * anchorPoint.x;
			vertIndex = cardSprite->getVertIndex();
			if (vertIndex > 0)
			{
				y -= cardSprite->getVertRealSpace();
			}
		}
		
		if (cardSprite->getHoriIndex() != 0)
		{
			x += cardSprite->getHoriRealSpace();
		}
		cardSprite->setNormalPos(Point(x, y));
	}
}

void CardView::setCardsShoot(const CardVector &cards, bool shoot)
{
	CardSprite* cardSprite = NULL;
	for (int i = 0; i < cards.size() ; i++)
	{
		Vector<Node*>& children = m_cardRootNode->getChildren();
		for (int index = 0; index < children.size() ; index++)
		{
			cardSprite = (CardSprite*)children.at(index);
			if (cardSprite->getCard() == cards[i] && cardSprite->isShooted() != shoot)
			{
				cardSprite->setShooted(shoot);
				
				m_cardMetricsDirty = true;
				break;
			}
		}
	}
}

void CardView::setCardsShoot(int beginIndex, int endIndex, bool shoot)
{
	if (beginIndex > endIndex)
		std::swap(beginIndex, endIndex);
	beginIndex = std::max(0, beginIndex);
	endIndex = std::min((int)m_cardRootNode->getChildrenCount() - 1, endIndex);

	CardSprite* pSprite = NULL;
	Vector<Node*>& children = m_cardRootNode->getChildren();
	for (int i = beginIndex; i <= endIndex; ++i)
	{
		pSprite = (CardSprite*)children.at(i);
		pSprite->setShooted(shoot);
	}

	m_cardMetricsDirty = true;
}

void CardView::flipCardsShoot(int beginIndex, int endIndex)
{
	if (beginIndex > endIndex)
		std::swap(beginIndex, endIndex);
	beginIndex = std::max(0, beginIndex);
	endIndex = std::min((int)m_cardRootNode->getChildrenCount() - 1, endIndex);

	CardSprite* cardSprite = NULL;
	Vector<Node*>& children = m_cardRootNode->getChildren();
	for (int i = beginIndex; i <= endIndex; ++i)
    {
		cardSprite = (CardSprite*)children.at(i);	
		if (!cardSprite->isDisabled())
		{
			cardSprite->setShooted(!cardSprite->isShooted());
		}
    }

	m_cardMetricsDirty = true;
}

void CardView::setCardsSelect(int beginIndex, int endIndex, bool select /* = true */)
{
	if (m_cardRootNode->getChildrenCount() == 0)
	{
		return;
	}

	if (beginIndex > endIndex)
		std::swap(beginIndex, endIndex);
	beginIndex = std::max(0, beginIndex);
	endIndex = std::min((int)m_cardRootNode->getChildrenCount() - 1, endIndex);

	CardSprite* cardSprite = NULL;
	Vector<Node*>& children = m_cardRootNode->getChildren();
	for (int i = beginIndex; i <= endIndex; ++i)
	{
		cardSprite = (CardSprite*)children.at(i);
		if (!cardSprite->isDisabled())
		{
			cardSprite->setSelected(select);
		}
	}
}

void CardView::setMinHoriSpace(float val)
{
	if (m_minHoriSpace == val)
		return;

	m_minHoriSpace = val;
	m_maxHoriSpace = std::max(val, m_minHoriSpace);

	m_cardMetricsDirty = true;
}

void CardView::setMaxHoriSpace(float val)
{
	if (m_maxHoriSpace == val)
		return;

	m_maxHoriSpace = val;
	m_minHoriSpace = std::min(val, m_maxHoriSpace);

	m_cardMetricsDirty = true;
}

void CardView::setMinVertSpace(float val)
{
	if (m_minVertSpace == val)
		return;

	m_minVertSpace = val;

	m_cardMetricsDirty = true;
}

void CardView::setMaxVertSpace(float val)
{
	if (m_maxVertSpace == val)
		return;

	m_maxVertSpace = val;

	m_cardMetricsDirty = true;
}

void CardView::dispatchCardShootChangedEvent()
{	
	if (m_callback)
		m_callback(this, EventType::SHOOT_CHANGED);
}

float CardView::calcAllCardSpriteHoriFixedSpace()
{
	float space = 0;
    for (int i = 0; i < m_cardRootNode->getChildrenCount(); i++)
    {
		CardSprite* cardSprite = (CardSprite*)m_cardRootNode->getChildren().at(i);
		if (cardSprite->getHoriFixedSpace() && cardSprite->getLocalZOrder() != 0)
			space += cardSprite->getHoriFixedSpace();
	}
	return space;
}

void CardView::addCards(const CardVector &cards)
{
	insertCards(m_cardRootNode->getChildrenCount(), cards);
}

void CardView::adjustAllCardSpriteSpace()
{
    for (int i = 0; i < m_cardRootNode->getChildrenCount(); i++) 
    {
    	adjustCardSpriteSpace((CardSprite*)m_cardRootNode->getChildren().at(i));
	}
    updateCardMetrics();
}

float CardView::calcAllCardSpriteHoriSpaceFactor()
{
	float factor = 0;
    for (int i = 0; i < m_cardRootNode->getChildrenCount(); i++)
    {
    	CardSprite* cardSprite = (CardSprite*)m_cardRootNode->getChildren().at(i) ;
		if (cardSprite->getHoriFixedSpace() == 0 && cardSprite->getLocalZOrder() != 0)
		{
			factor += cardSprite->getHoriSpaceFactor();
		}
	}
	return factor;
}

void CardView::setCardsExpand(int beginIndex, int endIndex, bool expand /*= true*/)
{
	if (beginIndex > endIndex)
		std::swap(beginIndex, endIndex);
	beginIndex = std::max(0, beginIndex);
	endIndex = std::min((int)m_cardRootNode->getChildrenCount() - 1, endIndex);
	
	CardSprite* cardSprite = NULL;
	Vector<Node*> children= m_cardRootNode->getChildren();
	for (int i = beginIndex ; i <= endIndex; ++i)
	{
		cardSprite = (CardSprite*)children.at(i);
		if (expand && !cardSprite->isExpanded())
		{
			cardSprite->setHoriFixedSpace(m_maxHoriSpace);
		}
		else if (!expand && cardSprite->isExpanded())
		{
			cardSprite->setHoriFixedSpace(0.0f);
		}
		cardSprite->setExpanded(expand);
	}

	m_cardMetricsDirty = true;
}

void CardView::expandCards(int index, int count)
{
	int beginIndex = index - count / 2;
	int endIndex = index + count / 2;
	adjustAllCardSpriteSpace();
	setCardsExpand(beginIndex, endIndex, true);
}

Vector<Node*>& CardView::getAllCardSprites()
{
	return m_cardRootNode->getChildren();
}

void CardView::removeShootedCards()
{
	CardSprite* cardSprite;
    Vector<Node*>& children = m_cardRootNode->getChildren();
	Vector<CardSprite*> shooted;
	for (Vector<Node*>::iterator iter = children.begin(); iter != children.end(); ++iter)
	{
		cardSprite = (CardSprite*)(*iter);
		if (cardSprite->isShooted())
		{
			shooted.pushBack(cardSprite);
		}
	}
	if (!shooted.empty())
	{
		for (Vector<CardSprite*>::iterator iter = shooted.begin(); iter != shooted.end(); ++iter)
		{
			m_cardRootNode->removeChild(*iter, true);
		}
	}
	m_reorderCardDirty = true;
	m_cardMetricsDirty = true;
}

void CardView::setScale(float scale)
{
	Layer::setScale(scale);

	setCardSize(m_cardSize * scale);
}

Point CardView::getTouchPosition(Node* node, Point touchPoint)
{
    Point tPoint = touchPoint;
    return tPoint;
}

void CardView::setTouchExpandCount(int count)
{
	m_touchExpandCount = count;
}

void CardView::setCardImageFile(const char* cardImageFileName)
{
	if (m_cardImageFileName == cardImageFileName)
	{
		return;
	}

	m_cardImageFileName = cardImageFileName;
	m_cardImageFilePrefix = m_cardImageFileName.substr(0, m_cardImageFileName.rfind('.')) + "/";
	CardSprite* cardSprite = createCardSprite(CARD_NULL);
	setCardSize(cardSprite->getContentSize());
}

void CardView::visit(Renderer *renderer, const Mat4& parentTransform, uint32_t parentFlags)
{
	if (!_visible)
	{
		return;
	}

	if (m_reorderCardDirty)
	{
		CardSprite* cardSprite;
		Vector<Node*>& children = m_cardRootNode->getChildren();
		for (int i = 0; i < children.size(); ++i)
		{
			cardSprite = (CardSprite*)children.at(i);
			cardSprite->setLocalZOrder(i);
		}

		m_reorderCardDirty = false;
	}

	if (m_reorderShootedCardDirty)
	{
		sortShootedCards();
	}
	
	if (m_cardMetricsDirty)
	{
		updateCardMetrics();
		m_cardMetricsDirty = false;
	}
		
	Layer::visit(renderer, parentTransform, parentFlags);
}

void CardView::addEventListener(const CardViewCallback& cardViewCallback)
{
	m_callback = cardViewCallback;
}

void CardView::setMoveAnimationEnable(bool enable)
{
	m_moveAnimationEnabled = enable;
}

void CardView::reorderCardShootedOrder(CardSprite* sprite)
{
	m_reorderShootedCardDirty = true;
	if (!sprite->isShooted())
	{
		sprite->m_shootedOrder = -1;
	}
	else
	{
		sprite->m_shootedOrder = m_lastShootedCardOrder++;
	}
}

void CardView::sortShootedCards()
{
	if (!m_reorderShootedCardDirty)
	{
		return;
	}
	
	CardSprite* cardSprite;
	std::vector<CardSprite*> shootdCardSprites;
	Vector<Node*>& children = m_cardRootNode->getChildren();
	for (int i = 0; i < children.size(); ++i)
	{
		cardSprite = (CardSprite*)children.at(i);
		if (cardSprite->isShooted())
		{
			shootdCardSprites.push_back(cardSprite);
		}
	}

	std::sort(shootdCardSprites.begin(), shootdCardSprites.end(), shootedCardSpriteComparisonLess);

	for (int i = 0; i < shootdCardSprites.size(); ++i)
	{
		cardSprite = shootdCardSprites[i];
		cardSprite->m_shootedOrder = i + 1;
	}

	m_lastShootedCardOrder = shootdCardSprites.size() + 1;
	m_reorderShootedCardDirty = false;

	dispatchCardShootChangedEvent();
}

bool CardView::shootedCardSpriteComparisonLess(CardSprite* c1, CardSprite* c2)
{
	return c1->getShootedOrder() < c2->getShootedOrder();
}

int CardView::getShootedCardsByOrder(CardVector &cards)
{
	cards.clear();

	CardSprite* cardSprite = NULL;
	std::vector<CardSprite*> shootdCardSprites;
	Vector<Node*>& children = m_cardRootNode->getChildren();
	for (int i = 0; i < children.size(); ++i)
	{
		cardSprite = (CardSprite*)children.at(i);
		if (cardSprite->isShooted())
			shootdCardSprites.push_back(cardSprite);
	}

	std::sort(shootdCardSprites.begin(), shootdCardSprites.end(), shootedCardSpriteComparisonLess);

	for (int i = 0; i < shootdCardSprites.size(); ++i)
	{
		cardSprite = shootdCardSprites[i];
		cards.push_back(cardSprite->getCard());
	}

	return cards.size();
}

bool CardView::removeCardByIndex(int index)
{
	CardSprite* cardSprite = getCardSprite(index);
	if (!cardSprite)
		return false;

	m_cardRootNode->removeChild(cardSprite, true);

	m_cardMetricsDirty = true;
	return true;
}

void CardView::removeCardsByIndex(const std::vector<int> &indexs)
{
	std::vector<CardSprite*> indexsCard;
	for (auto & child : indexs)
	{
		indexsCard.push_back(getCardSprite(child));
	}
	for (auto & child : indexsCard)
	{
		m_cardRootNode->removeChild(child, true);
	}

	m_reorderCardDirty = true;
	m_cardMetricsDirty = true;
}

int CardView::getShootedCardsIndex(std::vector<int> &indexs)
{
	indexs.clear();

	CardSprite* cardSprite = NULL;
	Vector<Node*>& children = m_cardRootNode->getChildren();
	for (int i = 0; i < children.size(); ++i)
	{
		cardSprite = (CardSprite*)children.at(i);
		if (cardSprite->isShooted())
			indexs.push_back(cardSprite->getLocalZOrder());
	}
	return indexs.size();
}
