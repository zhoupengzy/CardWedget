#pragma once

#include "MjCard/MjGameCards.h"

struct tagMjFallCard
{
	enum { CT_NULL, CT_SHUN, CT_PENG, CT_GANG, CT_ANGANG };
	uint8_t cbType;
	uint8_t cbFallCount;
	std::vector<MJCARD> cbCardData;
	tagMjFallCard() :cbType(CT_NULL), cbFallCount(0){}
};

class MjHandCards : public MjGameCards
{
public:
	MjHandCards();
	~MjHandCards();

	CREATE_FUNC(MjHandCards);
    virtual	bool init();

	static MjHandCards* createWithDirection(emMjDirection mjDirection);
	virtual bool initWithDirection(emMjDirection mjDirection);
	static MjHandCards* createWithFile(const char* fileImage);
	virtual bool initWithFile(const char* fileImage);

	float getFallCardsHSpace() const { return m_fallCardsHSpace; }
	float getFallCardHSpace() const { return m_fallCardHSpace; }
	void setFallCardHSpace(float val); 
	void setFallCardsHSpace(float val) { m_fallCardsHSpace = val; }
	void AddFallCardChi(std::vector<MJCARD>  cbCardData);
	void AddFallCardChi(MJCARD cbCard);
	void AddFallCardPeng(MJCARD cbCardData);
	void AddFallCardMingGang(MJCARD cbCardData);
	void AddFallCardAnGang(MJCARD cbCardData);
	void AddFallCardBuGang(MJCARD cbCardData);
	void ResetBuGang2Peng(MJCARD cbCardData);
	void ClearFallCard();
	void updateFallCardGangPostion(MjGameCards* mJGameCards);
	void resetAllGameCardsVec();
	virtual void setDirection(emMjDirection val);

	void addNewCard(MJCARD cbCardData); 
	void removeNewCard();
	virtual bool onTouchBegan(Touch *touch, Event *unusedEvent);
	virtual void onTouchMoved(Touch *touch, Event *unusedEvent);
	virtual void onTouchEnded(Touch *touch, Event *unusedEvent);

	void setCardShootByIndex(int index);
	int hitTest(Vec2 point);
	virtual void removeAllCards();
	MjCardSprite* getShootCard(int& index);
protected:
	MjGameCards* createFallCards();
	virtual void copySpecialProperties(Widget* model);
	Widget* createCloneInstance();


	std::vector<MjGameCards*> m_mJGameCardsVct;

	float m_gangSpace;
	float m_fallCardsHSpace;	//不同fallCards中的间距
	float m_fallCardHSpace;		//统一fallCards中单张的间距

	float m_touchHSpace;
	MjCardSprite * m_outCardSprite;
	bool  m_hasShowOutCards;
};

