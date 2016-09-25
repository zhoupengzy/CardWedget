#pragma once
#include <cocos2d.h>

#include "MjCardDef.h"
using namespace cocos2d;

enum emMjDirection
{
	MjDir_Null = -1,
	MjDir_Bottom = 0,
	MjDir_Right = 1,
	MjDir_Top = 2,
	MjDir_Left = 3,
	MjDir_Count
};

class MjCardSprite :
	public Sprite
{
public:
	MjCardSprite();
	~MjCardSprite();
	
	CREATE_FUNC(MjCardSprite);
	virtual bool init();
	static MjCardSprite* createWithMjDataAndDir(MJCARD card, emMjDirection dir = MjDir_Null, emMjStatus mjStatus = MjStatus_Stand,const char* fileName = "");
	bool initWithMjDataAndDir(MJCARD card, emMjDirection dir = MjDir_Null, emMjStatus mjStatus = MjStatus_Stand, const char* fileName = "");


	virtual bool isShooted() const { return m_shooted; }
	virtual bool isSelected() const { return m_selected; }
	virtual bool isExpanded() const { return m_expanded; }
	virtual void setShooted(bool shoot);
	virtual void setSelected(bool val);
	virtual void setExpanded(bool val);
	bool isNewCard() const { return m_isNewCard; }
	bool getHunCard() const { return m_isHunCard; }

	void setHunCard(bool val);
	void setNewCard(bool val) { m_isNewCard = val; }
	virtual float getHoriSpaceFactor() const { return m_horiSpaceFactor; }
	virtual float getVertSpaceFactor() const { return m_vertSpaceFactor; }
	virtual uint32_t getData() const { return m_data; }
	virtual float getHoriFixedSpace() const { return m_horiFixedSpace; }
	virtual float getHoriRealSpace();
	virtual float getVertRealSpace();

	virtual int getHoriIndex() const { return m_horiIndex; }
	virtual int getVertIndex() const { return m_vertIndex; }
	virtual void setHoriIndex(int val) { m_horiIndex = val; }
	virtual void setVertIndex(int val) { m_vertIndex = val; }
	virtual void setHoriSpaceFactor(float val) { m_horiSpaceFactor = val; }
	virtual void setVertSpaceFactor(float val) { m_vertSpaceFactor = val; }
	virtual void setData(uint32_t data) { m_data = data; }
	virtual void setHoriFixedSpace(float val) { m_horiFixedSpace = val; }
	virtual void set2DIndex(int horiIndex, int vertIndex){ m_horiIndex = horiIndex; m_vertIndex = vertIndex; }
	void setOffFaceDraw(Size val);

	virtual Size getOffFaceDraw() const { return m_szOffFaceDraw; }
	virtual Vec2 getNormalPos() const { return m_normalPos; }
	virtual Vec2 getDestPos() const { return m_destPos; }
	virtual float getDestPosX() const { return m_destPos.x; }
	virtual float getDestPosY() const { return m_destPos.y; }
	virtual void setNormalPos(const Vec2& pos);
	virtual void setDestPos(const Vec2& pos);
	virtual int getSelectedIndex() const { return m_selectedIndex; }
	virtual void setSelectedIndex(int index);


	emMjStatus getStatus() const { return m_mjStatus; }
	emMjDirection getDirection() const { return m_mjDirection; }
	MJCARD getMjCard() const { return m_mjCard; }

	void setDirection(emMjDirection val);
	void setStatus(emMjStatus val);
	void setMjCard(MJCARD val);

	void updateSpriteView();
protected:

private:
	emMjStatus m_mjStatus;
	emMjDirection m_mjDirection;
	MJCARD		m_mjCard;

	bool		m_shooted;
	Vec2		m_normalPos;			// 目标位置
	Vec2		m_destPos;				// 目标位置
	float		m_horiSpaceFactor;		// 水平空间距离因子
	float		m_vertSpaceFactor;		// 垂直空间距离因子
	uint32_t	m_data;					// 用于保存自定义数据
	bool		m_selected;
	bool		m_expanded;
	float		m_horiFixedSpace;
	int			m_horiIndex;
	int			m_vertIndex;
	bool		m_isNewCard;
	bool		m_isHunCard;

	Size		m_szOffFaceDraw;


	int m_selectedIndex;
	Sprite* m_imgViewBg;
	Sprite* m_imgViewFace;
	std::string m_fileName;
};