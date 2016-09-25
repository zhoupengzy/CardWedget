#pragma once

#include <cocos2d.h>
#include "PokerCardDef.h"

USING_NS_CC;

class CardView;

class CardSprite : public Sprite
{
	friend class CardView;
public:
	CardSprite(void);
	virtual ~CardSprite(void);

	CREATE_FUNC(CardSprite);
	static CardSprite* create(const char* imageFilePrefix, CARD card);
	virtual bool init();

	virtual bool isShooted() const { return m_shooted; }
	virtual bool isSelected() const { return m_selected; }
	virtual bool isExpanded() const { return m_expanded; }
	virtual bool isDisabled() const { return m_disabled; }
	virtual int getShootedOrder() const { return m_shootedOrder; }
	virtual void setShooted(bool shoot);
	virtual void setSelected(bool select);
	virtual void setExpanded(bool expand);
	virtual void setDisabled(bool disable);

	virtual CARD getCard() const { return m_card; }
	virtual float getHoriSpaceFactor() const { return m_horiSpaceFactor; }
	virtual float getVertSpaceFactor() const { return m_vertSpaceFactor; }
	virtual float getHoriFixedSpace() const { return m_horiFixedSpace; }
	virtual float getHoriRealSpace();
	virtual float getVertRealSpace();
	virtual int getHoriIndex() const { return m_horiIndex; }
	virtual int getVertIndex() const { return m_vertIndex; }
	virtual void setCard(CARD val);
	virtual void setHoriSpaceFactor(float val) { m_horiSpaceFactor = val; }
	virtual void setVertSpaceFactor(float val) { m_vertSpaceFactor = val; }
	virtual void setHoriFixedSpace(float val) { m_horiFixedSpace = val; }
	virtual void setHoriIndex(int val) { m_horiIndex = val; }
	virtual void setVertIndex(int val) { m_vertIndex = val; }
	virtual void setDimensionIndex(int horiIndex, int vertIndex);

	virtual Point getNormalPos() const { return m_normalPos; }
	virtual Point getDestPos() const { return m_destPos; }
	virtual float getDestPosX() const { return m_destPos.x; }
	virtual float getDestPosY() const { return m_destPos.y; }
	virtual void setNormalPos(const Point& pos);
	virtual void setDestPos(const Point& pos);

	virtual void setImageFilePrefix(const char* filePrefix);

	virtual void onEnter();

protected:
	virtual void calcDestPos(Point& pos);
	void updateSpriteFrame();
	
	void setCardView(CardView* cardView);

protected: 
	CARD		m_card;
	std::string m_imageFilePrefix;

	bool		m_shooted;				// 弹起的
	bool		m_selected;			    // 选中的
	bool		m_expanded;
	bool		m_disabled;				// 禁用
	int			m_shootedOrder;			// 弹起顺序
	
	float		m_horiSpaceFactor;		// 水平空间距离因子
	float		m_vertSpaceFactor;		// 垂直空间距离因子
	float		m_horiFixedSpace;
	int			m_horiIndex;
	int			m_vertIndex;
	
	Color3B		m_selectColor;
	Color3B		m_disableColor;

	Point		m_normalPos;			// 目标位置
	Point		m_destPos;				// 终点位置

	CardView*	m_cardView;
};


