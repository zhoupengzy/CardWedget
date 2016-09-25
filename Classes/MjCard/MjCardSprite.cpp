#include "MjCardSprite.h"
#include "MJGameCards.h"

#define OffSize_Vertical_Stand {Size(0,-10),Size(0,-10)};
#define OffSize_Vertical_Face  Size(0,10)
#define OffSize_Horizontal_Face  Size(10, 0)
//emMjDirection * emMjStatus 3*4的2维数组	
// MjStatus_Stand,
// MjStatus_Face,
// MjStatus_Back
static Size	m_offSizeMap[MjDir_Count][MjStatus_Count] = { { Size(0, -10.5), Size(0, 5), Size(0, 0) },
{ Size(0, 0), Size(7, 0), Size(0, 0) },
{ Size(0, 0), Size(0, 6.5), Size(0, 0) },
{ Size(0, 0), Size(7, 0), Size(0, 0) } };

MjCardSprite::MjCardSprite():
m_szOffFaceDraw(Size(0, 0)), m_shooted(false), m_isNewCard(false), m_mjDirection(MjDir_Bottom), m_mjStatus(MjStatus_Face)
{
}


MjCardSprite::~MjCardSprite()
{
}

void MjCardSprite::setDirection(emMjDirection val)
{
	if (m_mjDirection == val)
		return;
	
	m_mjDirection = val;

	updateSpriteView();
}

void MjCardSprite::setStatus(emMjStatus val)
{
	if (m_mjStatus == val)
		return;

	m_mjStatus = val;

	updateSpriteView();
}

void MjCardSprite::setMjCard(MJCARD val)
{
	m_mjCard = val;

	updateSpriteView();
}

bool MjCardSprite::initWithMjDataAndDir(MJCARD card, emMjDirection dir /*= MjDir_Null*/, emMjStatus mjStatus /*= MjStatus_Stand*/, const char* fileName)
{
	m_mjCard = card;
	m_mjDirection = dir;
	m_mjStatus = mjStatus;
	m_fileName = fileName;
	setAnchorPoint(Vec2(0.5, 0.5));
	m_imgViewFace = Sprite::create();
	m_imgViewFace->setAnchorPoint(Vec2(0.5, 0.5));
	addChild(m_imgViewFace);

	updateSpriteView();
	return true;
}

MjCardSprite* MjCardSprite::createWithMjDataAndDir(MJCARD card, emMjDirection dir /*= MjDir_Null*/, emMjStatus mjStatus /*= MjStatus_Stand*/, const char* fileName)
{
	MjCardSprite* mjCardSprite = MjCardSprite::create();
	if (mjCardSprite && mjCardSprite->initWithMjDataAndDir(card, dir, mjStatus, fileName))
	{
		return mjCardSprite;
	}
	else
	{
		if (mjCardSprite)
		{
			delete mjCardSprite;
			mjCardSprite = NULL;
		}
		return NULL;
	}
	
}

bool MjCardSprite::init()
{
	if (!Sprite::init())
	{
		return false;
	}

	return true;
}

void MjCardSprite::setShooted(bool shoot)
{
	MjGameCards* parent = static_cast<MjGameCards*>(getParent());
	if (parent)
	{
		m_shooted = shoot;
		setPositionY(shoot * parent->getShootAltitude());
	}
}

void MjCardSprite::setSelected(bool val)
{

}

void MjCardSprite::setExpanded(bool val)
{

}

float MjCardSprite::getHoriRealSpace()
{
	return 0;

}	

float MjCardSprite::getVertRealSpace()
{
	return 0;
}

void MjCardSprite::setNormalPos(const Vec2& pos)
{

}

void MjCardSprite::setDestPos(const Vec2& pos)
{

}

void MjCardSprite::setSelectedIndex(int index)
{

}

void MjCardSprite::setOffFaceDraw(Size val)
{
	m_szOffFaceDraw = val;
	m_imgViewFace->setPosition((getContentSize() / 2 + val));

}
void MjCardSprite::updateSpriteView()
{
	emMjDirection mjDirectionIndex;
	switch (m_mjDirection)
	{
	case MjDir_Left:mjDirectionIndex = MjDir_Right;
		break;
	case MjDir_Top:mjDirectionIndex = MjDir_Bottom;
		break;
	default:
		mjDirectionIndex = m_mjDirection;
		break;
	}

	if (m_mjStatus == MjStatus_Stand && m_mjDirection == MjDir_Top)
	{
		mjDirectionIndex = m_mjDirection;
	}
	if (m_mjStatus == MjStatus_Stand && m_mjDirection == MjDir_Left)
	{
		mjDirectionIndex = m_mjDirection;
	}

	std::string pngPath = String::createWithFormat("%s_%d_%d.png", m_fileName.c_str(), mjDirectionIndex, m_mjStatus)->getCString();
	SpriteFrame* framebg = SpriteFrameCache::getInstance()->getSpriteFrameByName(pngPath.c_str());
	framebg->getTexture()->setAntiAliasTexParameters();
	setSpriteFrame(framebg);
	if (m_mjStatus == MjStatus_Face || (m_mjStatus == MjStatus_Stand && m_mjDirection==MjDir_Bottom))
	{
		m_imgViewFace->setVisible(true);
		SpriteFrame* frame = SpriteFrameCache::getInstance()->
			getSpriteFrameByName(String::createWithFormat("%s_%x.png", m_fileName.c_str(), m_mjCard)->getCString());
		frame->getTexture()->setAntiAliasTexParameters();
		m_imgViewFace->setSpriteFrame(frame);
	}
	else
	{
		m_imgViewFace->setVisible(false);
	}

	if (m_mjDirection == MjDir_Top && (m_mjStatus == MjStatus_Face || m_mjStatus == MjStatus_Back))
	{
		setRotation(180);
		m_imgViewFace->setRotation(180);
	}

	if (m_mjDirection == MjDir_Left && (m_mjStatus == MjStatus_Face || m_mjStatus == MjStatus_Back))
	{
		setRotation(180);
		m_imgViewFace->setRotation(180);
	}

	if ((m_mjDirection == MjDir_Top || m_mjDirection == MjDir_Bottom) && m_mjStatus == MjStatus_Face)
	{
		m_imgViewFace->setScaleY(0.9);
	}
// 
// 	switch (m_mjDirection)
// 	{
// 	case MjDir_Right: m_imgViewFace->setRotation(90);
// 		break;
// 	case MjDir_Top:m_imgViewFace->setRotation(180);
// 		break;
// 	case MjDir_Left:m_imgViewFace->setRotation(-90);
// 		break;
// 	default:
// 		break;
// 	}
	setOffFaceDraw(m_offSizeMap[m_mjDirection][m_mjStatus]);
}

void MjCardSprite::setHunCard(bool val)
{
	m_isHunCard = val;
	if (m_isHunCard)
	{
		setColor(Color3B(150, 150, 150));
		m_imgViewFace->setColor(Color3B(150, 150, 150));
	}
	else
	{
		setColor(Color3B(255, 255, 255));
		m_imgViewFace->setColor(Color3B(255, 255, 255));
	}
}
