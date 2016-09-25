#pragma once

#include <vector>
#include <stdint.h>

#include "MjCardDef.h"

typedef std::vector<MJCARD> CardVector;
typedef CardVector::iterator CardIter;
typedef CardVector::const_iterator const_CardIter;


//吃牌类型
enum emEatType
{
	ET_LEFT	= 1,
	ET_MID ,
	ET_RIGHT ,
};
struct tagEatData
{
	CardVector	eatCards;
	emEatType	eatType;
};
typedef std::vector<tagEatData> EatCards;

enum emMjPitchType
{
	Pitch_Null = 0,
	Pitch_Shun,
	Pitch_Peng,
	Pitch_Gang,
	Pitch_BuGang,
	Pitch_AnGang,
};

struct tagPitchData
{
	uint8_t pitchType;
	MJCARD	flagCard;
	uint16_t	wTargetChair;

	template<class Archive>
	void serialize(Archive &ar, const unsigned int version)
	{
		ar & pitchType;
		ar & flagCard;
		ar & wTargetChair;
	}

	template<class Archive>
	void save(Archive &ar)
	{
		ar << pitchType << flagCard << wTargetChair;
	}

	template<class Archive>
	void load(Archive &ar)
	{
		uint16_t tmp = 0;
		ar >> pitchType >> flagCard >> wTargetChair;
	}
};
typedef std::vector<tagPitchData> PitchDatas;

enum emMjAction
{
	Action_Null = 0,
	Action_Chi,
	Action_Peng,
	Action_Gang,
	Action_BuGang,
	Action_AnGang,
	Action_Hu,
	Action_ZhaHu,
	Action_MAX
};

enum emMjHuType
{
	HT_Null = 0,
	HT_Normal,
	HT_4Baida,
	HT_7Duizi,
};

struct tagActionSaveData 
{
	uint16_t wChair;
	emMjAction action;
	CardVector cards;
};

struct tagHuData
{
	uint8_t cbBaidaCount;
	uint8_t cbRemainCount;
	CardVector remainCardIndexs;
	CardVector headCards;
	CardVector pitchCardDatas;
};


//游戏逻辑类
class CGameLogic
{
public:
	CGameLogic();
	~CGameLogic();

	void SetBaida(MJCARD cardBaida){ m_cardBaida = cardBaida; }
	void SetBaida_Show(MJCARD cardBaidaShow);
	MJCARD GetBaida(){ return m_cardBaida; }
	uint16_t GetTotalCount(){ return MJCARD_COUNT_NOFLOWER; }

	void ShuffleCards(CardVector& shuffledCards);
	void SortCards(CardVector & cards);
	uint16_t RemoveCard(CardVector &cards,const MJCARD rmvcard);
	uint16_t RemoveCard(CardVector &cards,const CardVector rmvcards);
	uint16_t RemoveCardByIndex(CardVector &cards,const std::vector<uint16_t> rmvindex);

	bool Index2Card(const CardVector cards,CardVector &data);
	bool Card2Index(const CardVector cards,CardVector &data);

	emMjAction CheckAction(const CardVector cards,MJCARD card,bool bCheckHu,bool bCheckEat);
	bool CanEat(const CardVector cards,MJCARD cardEat, EatCards &Eatcards);
	bool CheckEat(const CardVector cards);
	bool CanPeng(const CardVector cards,MJCARD cardEat);
	bool CheckPeng(const CardVector cards);
	bool CanGang(const CardVector cards,MJCARD cardEat);
	bool CheckGang(const CardVector cards);
 	uint16_t CheckBuGang(const PitchDatas pitchDatas,MJCARD card);
	bool CheckBuGang(const PitchDatas pitchDatas,const CardVector cards,CardVector &result);
	bool CanAnGang(const CardVector cards,MJCARD card);
	MJCARD CanAnGang(const CardVector cards);
	bool CheckAnGang(const CardVector cards);
 	bool CheckTing(const CardVector cards, CardVector &tingCards);	//
	bool CheckCanFraudHu(const CardVector cards, MJCARD newCatchCard);	//检测是否可以诈和
	emMjHuType CheckHu(const CardVector cards,MJCARD newCatchCard,CardVector &result);	//客户端用,服务端尽量别用
	emMjHuType CheckHu(const CardVector cards);					//服务端用

	uint16_t FindCardCount(CardVector cards,MJCARD card);

	MJCARD SearchOutCard(const CardVector cards);
	uint16_t GetRandUnsocialCard(const CardVector cards);
	uint8_t GetCardsDistance(const CardVector cards, int nIndex);
	uint8_t GetCardsDistance(MJCARD card1, MJCARD card2);
protected:
	bool _Hu(tagHuData &huData);				//深度优先
	bool _CheckHu_Duizi(const CardVector cards);
	emMjPitchType _CheckHu_Three(const CardVector cards);

protected:
	MJCARD m_cardBaida;	//百搭(财神)
};