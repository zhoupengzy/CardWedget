#pragma once
#include <cocos2d.h>
#include <cocostudio/CocoStudio.h>

#include "MjCard/MjCardDef.h"

USING_NS_CC;
using namespace ui;

typedef std::vector<MJCARD> MjCardVct;

class MjCardSprite;
enum emMjDirection;

class MjGameCards : public Layout
{
public:
	MjGameCards();
	~MjGameCards();

	CREATE_FUNC(MjGameCards);
	virtual bool init();
	static MjGameCards* createWithDirection(emMjDirection mjDirection);
	virtual bool initWithDirection(emMjDirection mjDirection);

	static MjGameCards* createWithFile(const char* fileImage);
	virtual bool initWithFile(const char* fileImage);

	MjCardSprite * addMjCard(MJCARD mjCard,bool newCard = false);
	void addMjCards(MjCardVct mjCards);
	void setMjCards(MjCardVct mjCards);
	virtual void removeAllCards();
	void removeCards(MjCardVct mjCards);
	void removeCardsByIndex(int index);
	void removeLastCard();
	MjCardSprite * getCardsByIndex(int index);

	MjCardVct getCards();
	MjCardVct getAllMjCardData();
	void PlayPitchCardAni();
	int getMaxHCardsCount() const { return m_maxHCardsCount; }
	emMjDirection getDirection() const { return m_mjDirection; }
	emMjStatus getStatus() const { return m_mjStatus; }
	float getCardHSpace() const { return m_dwCardHSpace; }
	float getCardVSpace() const { return m_dwCardVSpace; }
	float getShootAltitude() const { return m_dwShootAltitude; }
	MJCARD getMjcardByIndex(int index);// { return ((MjCardSprite*)(m_allMjCards->getChildren().at(index)))->getMjCard(); }
	MJCARD getHunCard() const { return m_hunCard; }

	void setHunCard(MJCARD val) { m_hunCard = val; }
	void setShootAltitude(float val) { m_dwShootAltitude = val; }
	void setCardVSpace(float val);
	void setCardHSpace(float val);
	void setEmptyCards(int count);
	void addEmptyCard(bool newCard);
	void setStatus(emMjStatus val);
	void removeNewCard();
	virtual void setDirection(emMjDirection val);
	void setMaxHCardsCount(int val) { m_maxHCardsCount = val; } 
	void adjustCardSpriteSpace(MjCardSprite* mjCardSprite);
	void adjustAllCardSpriteSpace();
	void sortAllGameCards();
	void resetMJLayer();
	void updateCardsRect();
	void setAnchorPoint(const Vec2& anchorPoint);
	void loadCardsTextures(const char* texturesPath);
	void setLastCardToNewCard();
	
	SpriteBatchNode* getAllMjCards() { return m_allMjCards; }
	MjCardSprite* getCardSpriteByIndex(int index) { return (MjCardSprite*)(m_allMjCards->getChildren().at(index)); }
protected:
	virtual Widget* createCloneInstance();
	virtual void copySpecialProperties(Widget* model);


protected:
	emMjDirection m_mjDirection;
	emMjStatus	  m_mjStatus;

	float 	m_dwCardHSpace;			// 牌间距

	float 	m_dwCardVSpace;			// 行间距

	float 	m_dwShootAltitude;		// 弹起高度

	float 	m_dwMaxLenth;			// 最大长度
	float	m_dwNewCatchSpace;		// 新抓牌间距
	int		m_nSelectIndex;			// 当前已点击牌索引
	float 	m_dwOffHSpace;			// 牌间距

	int		m_maxHCardsCount;		//

	SpriteBatchNode* m_allMjCards;
	std::string m_cardImageFileName;
	
	MJCARD	m_hunCard;

};

