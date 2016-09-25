#pragma once

#include <cocos2d.h>
#include "PokerCardDef.h"
#include "CardSprite.h"

USING_NS_CC;

class CardSprite;

class CardView : public Layer
{
public:
	CardView(void);
	virtual ~CardView(void);
	
	enum class EventType
	{
		SHOOT_CHANGED
	};

	typedef std::function<void(Ref*, CardView::EventType)> CardViewCallback;

	static CardView* createWithFile(const char* cardImageFileName);

	virtual bool initWithFile(const char* cardImageFileName);

	void setCardImageFile(const char* cardImageFileName);

    virtual float getShootAltitude() const { return m_shootAltitude; }
	virtual float getMinHoriSpace() const { return m_minHoriSpace; }
	virtual float getMaxHoriSpace() const { return m_maxHoriSpace; }
	virtual float getMinVertSpace() const { return m_minVertSpace; }
	virtual float getMaxVertSpace() const { return m_maxVertSpace; }
	virtual float getMaxWidth() const { return m_maxWidth; }
	virtual float getMaxHeight() const { return m_maxHeight; }
	virtual float getCardWidth() const { return m_cardSize.width; }
	virtual float getCardHeight() const { return m_cardSize.height; }
	virtual Size getCardSize() const { return m_cardSize; }
	virtual float getCurrentHoriSpace() const { return m_currentHoriSpace; }
	virtual float getCurrentVertSpace() const { return m_currentVertSpace; }
	virtual void setCurrentHoriSpace(float val) { m_currentHoriSpace = val; }
	virtual void setShootAltitude(float altitude);
	virtual void setMinHoriSpace(float val);
	virtual void setMaxHoriSpace(float val);
	virtual void setMinVertSpace(float val);
	virtual void setMaxVertSpace(float val);
	virtual void setMaxWidth(float width);
	virtual void setMaxHeight(float height);
	virtual void setCardSize(const Size& szCard);
	virtual void setTouchExpandCount(int count);
	virtual void setExpandHoriSpace(float val) { m_expandHoriSpace = val; }

	virtual int	 getCardCount();
	virtual int  getCardsIndex(const CardVector &cards, std::vector<int> &indexs);
	virtual CARD getCard(uint16_t index);
	virtual int	 getCards(CardVector &cards);
	virtual CardSprite* getCardSprite(int index);
	virtual CardSprite* findCardSprite(CARD card);
	virtual Vector<Node*>& getAllCardSprites();
	virtual CardSprite* addCard(CARD card);
	virtual CardSprite* insertCard(int index, CARD card);
	virtual void setCards(const CardVector &cards);
	virtual void addCards(const CardVector &cards);
	virtual void insertCards(int index, const CardVector &cards);
	virtual bool removeCard(CARD card);
	virtual bool removeCardByIndex(int index);
	virtual void removeCards(const CardVector &cards);
	virtual void removeCardsByIndex(const std::vector<int> &indexs);
	virtual void removeShootedCards();
	virtual void clearCards();

	virtual bool isCardShooted(uint16_t index);
	virtual int	 getShootedCardCount();
	virtual int	 getShootedCards(CardVector &cards);
	virtual int	 getShootedCardsIndex(std::vector<int> &indexs);
	virtual int	 getShootedCardsByOrder(CardVector &cards);
	virtual int	 getUnshootedCards(CardVector &cards);
	virtual void setCardShoot(CARD card, bool shoot);
	virtual void setCardsShoot(const CardVector &cards, bool shoot);
	virtual void setCardsShoot(int beginIndex, int endIndex, bool shoot);
	virtual void setAllCardsShoot(bool shoot);
	virtual void reorderCardShootedOrder(CardSprite* sprite);
	virtual void sortShootedCards();
	virtual void flipCardsShoot(int beginIndex, int endIndex);
	virtual void setCardsSelect(int beginIndex, int endIndex, bool select = true);
	virtual void setCardsExpand(int beginIndex, int endIndex, bool expand = true);
	virtual void expandCards(int index, int count);
	
	virtual void setScale(float scale);

	bool isMoveAnimationEnabled() const { return m_moveAnimationEnabled; }
	void setMoveAnimationEnable(bool enable);

	virtual int	 hitTestCard(Point point);
	
	virtual void addEventListener(const CardViewCallback& cardViewCallback);

	virtual void onTouchesBegan(const std::vector<Touch*>& touches, Event *unused_event);
	virtual void onTouchesMoved(const std::vector<Touch*>& touches, Event *unused_event);
	virtual void onTouchesEnded(const std::vector<Touch*>& touches, Event *unused_event);

	virtual void visit(Renderer *renderer, const Mat4& parentTransform, uint32_t parentFlags);

protected:
	virtual CardSprite* createCardSprite(CARD card);

	virtual void updateCardMetrics();
	virtual void adjustAllCardSpriteSpace();
	virtual void adjustCardSpriteSpace(CardSprite* pSprite);
	virtual float calcAllCardSpriteHoriSpaceFactor();
	virtual float calcAllCardSpriteHoriFixedSpace();
	
	virtual void dispatchCardShootChangedEvent();
	
	virtual Point getTouchPosition(Node* node, Point touchPoint);

	static bool shootedCardSpriteComparisonLess(CardSprite* c1, CardSprite* c2);
	
protected:
	Size	m_cardSize;
	float	m_shootAltitude;					// 弹起高度
	float 	m_minHoriSpace;						// 水平
	float 	m_maxHoriSpace;
	float	m_currentHoriSpace;
	float 	m_minVertSpace;						// 垂直
	float 	m_maxVertSpace;
	float 	m_currentVertSpace;
	float 	m_maxWidth;
	float 	m_maxHeight;
	int		m_touchExpandCount;
	float	m_expandHoriSpace;
	int		m_lastShootedCardOrder;

	Color3B m_cardSelectColor;
	Color3B m_cardDisableColor;

	bool	m_moveAnimationEnabled;

	bool	m_reorderCardDirty;
	bool	m_cardMetricsDirty;
	bool	m_reorderShootedCardDirty;

	Point	m_touchBeganPoint;
	int		m_beginTouchCardIndex;
	int		m_endTouchCardIndex;
	
	std::string m_cardImageFileName;
	std::string m_cardImageFilePrefix;
	
	CardViewCallback m_callback;

	SpriteBatchNode* m_cardRootNode;
	
    bool    m_autoShootDown;

};