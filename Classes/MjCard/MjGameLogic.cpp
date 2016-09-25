#include <time.h>
//#include <boost/bind.hpp>
#include <algorithm>
#include <bitset>
#include <functional>
//#include <stdafx.h>
#include "MjGameLogic.h"

#define INVALID_INDEX				((uint16_t)(-1))

//扑克数据
static MJCARD	m_cbCardData[MJCARD_COUNT_NOFLOWER]=
{
	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,	//万
	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,	//万
	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,	//万
	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,	//万
	0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,	//筒
	0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,	//筒
	0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,	//筒
	0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,	//筒
	0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,	//条
	0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,	//条
	0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,	//条
	0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,	//条
	0x41,0x42,0x43,0x44,0x45,0x46,0x47,				//风字
	0x41,0x42,0x43,0x44,0x45,0x46,0x47,				//风字
	0x41,0x42,0x43,0x44,0x45,0x46,0x47,				//风字
	0x41,0x42,0x43,0x44,0x45,0x46,0x47,				//风字
// 	0x51,0x52,0x53,0x54,0x55,0x56,0x57,0x58			//花
};

//////////////////////////////////////////////////////////////////////////


CGameLogic::CGameLogic()
:m_cardBaida(0)
{

}

CGameLogic::~CGameLogic()
{

}

void CGameLogic::SetBaida_Show(MJCARD cardBaidaShow)
{
	if(cardBaidaShow == 0x47)
		m_cardBaida = 0x41;
	else if(CARD_VALUE(cardBaidaShow) == MJCARD_VALUE_9)
		m_cardBaida = MAKE_MJCARD(CARD_COLOR(cardBaidaShow),MJCARD_VALUE_1);
	else
		m_cardBaida = cardBaidaShow+1;
}

void CGameLogic::ShuffleCards(CardVector& shuffledCards)
{
	uint16_t wCardCount = GetTotalCount();
	shuffledCards.resize(wCardCount);
	srand((uint32_t)time(NULL));
	size_t pos = 0, randPos;
	for (CardIter itr = shuffledCards.begin(); itr != shuffledCards.end(); ++itr)
	{
		randPos = rand() % (wCardCount - pos++);
		MJCARD& randCard = m_cbCardData[randPos];
		MJCARD& lastCard = m_cbCardData[wCardCount - pos];
		(*itr) = randCard;
		randCard = lastCard;
		lastCard = (*itr);
	}
}

bool FuncSort(CGameLogic *pGameLogic,MJCARD card1,MJCARD card2)
{
	if(pGameLogic->GetBaida() == card1 && card2 != pGameLogic->GetBaida())
		return true;
	else if(pGameLogic->GetBaida() == card2 && card1 != pGameLogic->GetBaida())
		return false;
	return card1 < card2;
}

void CGameLogic::SortCards( CardVector & cards)
{
	std::sort(cards.begin(),cards.end()/*,boost::BOOST_BIND(FuncSort,this,_1,_2)*/);
}

uint16_t CGameLogic::RemoveCard(CardVector &cards,const MJCARD rmvcard)
{
	CardIter itr = std::find(cards.begin(),cards.end(),rmvcard);
	if(itr == cards.end())
		return 0 ;
	cards.erase(itr);
	return 1;
}

uint16_t CGameLogic::RemoveCard(CardVector &cards,const CardVector rmvcards)
{
	CardVector vecIndex(rmvcards.begin(),rmvcards.end());
	if(!Card2Index(cards,vecIndex))
		return 0;
	std::vector<uint16_t> rmvindex(vecIndex.begin(),vecIndex.end());
	return RemoveCardByIndex(cards,rmvindex);
}

uint16_t CGameLogic::RemoveCardByIndex(CardVector &cards,const std::vector<uint16_t> rmvindex)
{
	uint16_t wCount = 0;
	std::vector<uint32_t> tmpIndex(rmvindex.begin(),rmvindex.end());
	std::sort(tmpIndex.begin(),tmpIndex.end(),std::greater<uint16_t>());
	for (size_t i = 0; i < tmpIndex.size(); ++i)
	{
		if(tmpIndex[i] >= cards.size())
			continue;
		cards[tmpIndex[i]] = * --cards.end();
		cards.pop_back();
		++wCount;
	}
	return wCount;
}

bool CGameLogic::Index2Card(const CardVector cards,CardVector &data)
{
	std::bitset<MJCARD_MAX_HAND_COUNT> maskHit;
	for (CardIter itr = data.begin(); itr != data.end(); ++itr)
	{
		if(maskHit.test(*itr) || (*itr) >= cards.size())
			return false;
		maskHit.set(*itr);
		*itr = cards[*itr];		
	}
	return true;
	
}

bool CGameLogic::Card2Index(const CardVector cards,CardVector &data)
{
	CardVector tmpCards(cards.begin(),cards.end());
	for (CardIter itr = data.begin(); itr != data.end(); ++itr)
	{
		if(*itr == 0)
			return false;
		CardIter itrFind = std::find(tmpCards.begin(),tmpCards.end(),*itr);
		if(itrFind == tmpCards.end())
			return false;
		
		uint16_t wIndex = std::distance(tmpCards.begin(),itrFind);
		*itrFind = 0;
		*itr = (uint8_t)wIndex;		
	}
	return true;
}

emMjAction CGameLogic::CheckAction(const CardVector cards,MJCARD card,bool bCheckHu,bool bCheckEat)
{
	EatCards eatCards;
	CardVector retCards;
	if(bCheckHu) 
	{
		CardVector resultHu,tmpCards(cards.begin(),cards.end());
		tmpCards.push_back(card);
		if(CheckHu(tmpCards,card,resultHu))
			return Action_Hu;
	}
	else if(CanGang(cards,card))
		return Action_Gang;
	else if(CanPeng(cards,card))
		return Action_Peng;
	else if(bCheckEat && CanEat(cards,card,eatCards))
		return Action_Chi;
	return Action_Null;
}

bool CGameLogic::CanEat(const CardVector cards,MJCARD card ,EatCards &Eatcards)
{
	if(m_cardBaida != 0 &&card == m_cardBaida)
		return false;

	Eatcards.clear();
	std::vector<MJCARD> cardValues(10,0);
	if(CARD_COLOR(card) < MJCARD_COLOR_WAN || CARD_COLOR(card) > MJCARD_COLOR_TIAO)
		return false;
	uint8_t cbColor = CARD_COLOR(card);
	uint8_t cbValue = CARD_VALUE(card);
	for(const_CardIter itr = cards.begin();itr != cards.end();++itr)
	{
		if(CARD_COLOR(*itr) == cbColor)
			++cardValues[CARD_VALUE(*itr)];
	}
	if(cbValue > MJCARD_VALUE_2 && cardValues[cbValue-2] > 0 && cardValues[cbValue-1] > 0)
	{
		tagEatData EatData;
		EatData.eatCards.push_back(card-2);
		EatData.eatCards.push_back(card-1);
		EatData.eatType = ET_RIGHT;
		Eatcards.push_back(EatData);
		return true;
	}
	if(cbValue > MJCARD_VALUE_1 && cbValue < MJCARD_VALUE_9 && cardValues[cbValue-1] > 0 && cardValues[cbValue+1] > 0)
	{
		tagEatData EatData;
		EatData.eatCards.push_back(card-1);
		EatData.eatCards.push_back(card+1);
		EatData.eatType = ET_MID;
		Eatcards.push_back(EatData);
		return true;
	}
	if(cbValue < MJCARD_VALUE_8 && cardValues[cbValue+1] > 0 && cardValues[cbValue+2] > 0)
	{
		tagEatData EatData;
		EatData.eatCards.push_back(card+1);
		EatData.eatCards.push_back(card+2);
		EatData.eatType = ET_LEFT;
		Eatcards.push_back(EatData);
		return true;
	}
	return false;
}

bool CGameLogic::CheckEat( const CardVector cards )
{
	if(cards.size() != 3)
		return false;
	if(m_cardBaida != 0 && FindCardCount(cards,m_cardBaida) != 0)
		return false;
	CardVector tmpCard(cards.begin(),cards.end());
	SortCards(tmpCard);
	return (CARD_COLOR(tmpCard[0]) >= MJCARD_COLOR_WAN && CARD_COLOR(tmpCard[0]) <= MJCARD_COLOR_TIAO
		&& tmpCard[0] + 1 == tmpCard[1] && tmpCard[1] + 1 == tmpCard[2]);
}

bool CGameLogic::CanPeng( const CardVector cards,MJCARD card )
{
	if(m_cardBaida != 0 &&card == m_cardBaida)
		return false;
	return FindCardCount(cards,card) >= 2;
}

bool CGameLogic::CheckPeng( const CardVector cards )
{
	if(cards.size() != 3)
		return false;
	if(m_cardBaida != 0 && cards[0] == m_cardBaida)
		return false;
	return cards[0] == cards[1] && cards[1] == cards[2];
}

bool CGameLogic::CanGang( const CardVector cards,MJCARD card )
{
	if(m_cardBaida != 0 &&card == m_cardBaida)
		return false;

	return FindCardCount(cards,card) >= 3;
}

bool CGameLogic::CheckGang( const CardVector cards )
{
	if(cards.size() != 4)
		return false;
	if(m_cardBaida != 0 && FindCardCount(cards,m_cardBaida) != 0)
		return false;
	return cards[0] == cards[1] && cards[1] == cards[2] && cards[2] == cards[3];
}

uint16_t CGameLogic::CheckBuGang(const PitchDatas pitchDatas,MJCARD card)
{
	if(pitchDatas.size() <= 0 )
		return INVALID_INDEX;
	for(uint16_t i = 0; i < pitchDatas.size(); ++i)
	{
		if(pitchDatas[i].pitchType == Pitch_Peng && pitchDatas[i].flagCard == card)
			return i;
	}
	return INVALID_INDEX;
}
bool CGameLogic::CheckBuGang(const PitchDatas pitchDatas,const CardVector cards,CardVector &result)
{
	result.clear();
	if(pitchDatas.size() == 0)
		return false;
	for (size_t i=0;i<pitchDatas.size();++i)
	{
		if(pitchDatas[i].pitchType != Pitch_Peng)
			continue;
		if(std::find(cards.begin(),cards.end(),pitchDatas[i].flagCard) != cards.end())
			result.push_back(pitchDatas[i].flagCard);
	}
	return result.size() != 0;
}

bool CGameLogic::CanAnGang( const CardVector cards,MJCARD card )
{
	if(m_cardBaida !=0 && card == m_cardBaida)
		return false;
	return FindCardCount(cards, card) == 4;
}
MJCARD CGameLogic::CanAnGang(const CardVector cards)
{
	if(cards.size() < 4)
		return MJCARD_NULL;
	CardVector tmpCards(cards.begin(),cards.end());
	SortCards(tmpCards);
	int nCount = 1;
	MJCARD oldCard = tmpCards[0];
	for (size_t i=1;i<tmpCards.size();++i)
	{
		if(m_cardBaida != 0 && tmpCards[i] == m_cardBaida)
			continue;
		if(tmpCards[i] == oldCard)
		{
			if(++nCount == 4)
				return oldCard;
		}
		else 
		{
			nCount = 1;
			oldCard = tmpCards[i];
		}
	}
	return MJCARD_NULL;
}

bool CGameLogic::CheckAnGang( const CardVector cards )
{
	if(cards.size() != 4)
		return false;
	if(m_cardBaida != 0 && FindCardCount(cards,m_cardBaida) != 0)
		return false;
	return cards[0] == cards[1] && cards[1] == cards[2] && cards[2] == cards[3];
}


 bool CGameLogic::CheckTing( const CardVector cards, CardVector &tingCards)
 {
 	return false;
 }

emMjHuType CGameLogic::CheckHu( const CardVector cards, MJCARD newCatchCard ,CardVector &result)
{
	if(m_cardBaida != 0 && FindCardCount(cards,m_cardBaida) == 4)
	{
		result.assign(cards.begin(),cards.end());
		return HT_4Baida;
	}
	tagHuData huData;
	huData.cbBaidaCount = 0;
	huData.cbRemainCount = 0;
	huData.headCards.clear();
	huData.pitchCardDatas.clear();
	huData.remainCardIndexs.clear();

	for (const_CardIter itr=cards.begin();itr !=cards.end();++itr)
	{
		if(CARD_COLOR(*itr) < MJCARD_COLOR_WAN || CARD_COLOR(*itr) > MJCARD_COLOR_ZI)
			return HT_Null;

		if(m_cardBaida != 0 && *itr == m_cardBaida)
		{
			++huData.cbBaidaCount;
			continue;
		}
		if(*itr >= huData.remainCardIndexs.size())
			huData.remainCardIndexs.resize(*itr+1);
		++huData.remainCardIndexs[*itr];
		++huData.cbRemainCount;
	}
	//检测胡七对子
	if(cards.size() == 14)
	{
		bool b7dui = true;

		for(size_t i=0;i<huData.remainCardIndexs.size();++i)
		{
			if(huData.remainCardIndexs[i] & 1)
			{
				b7dui = false;
				break;
			}
		}
		if(b7dui)
		{
			result.assign(cards.begin(),cards.end());
			SortCards(result);
			return HT_7Duizi;
		}
	}

	//优先爆头
	bool bBaotou = false;
	if(huData.cbBaidaCount > 1 && newCatchCard < huData.remainCardIndexs.size() && huData.remainCardIndexs[newCatchCard] > 0)
	{
		--huData.cbBaidaCount;
		--huData.remainCardIndexs[newCatchCard];
		huData.headCards.push_back(newCatchCard);
		huData.headCards.push_back(m_cardBaida);
		bBaotou = _Hu(huData);
		if(!bBaotou)
		{
			++huData.cbBaidaCount;
			++huData.remainCardIndexs[newCatchCard];
			huData.headCards.clear();
		}
	}
	if(!bBaotou)
	{
		if(!_Hu(huData))
			return HT_Null;
	}

	result.assign(huData.headCards.begin(),huData.headCards.end());
	result.insert(result.end(),huData.pitchCardDatas.rbegin(),huData.pitchCardDatas.rend());
	return HT_Normal;
}

//要求牌顺序为一对在最前面,后跟各墩数
emMjHuType CGameLogic::CheckHu( const CardVector cards )
{
	//四百搭
	if(m_cardBaida != 0 && FindCardCount(cards,m_cardBaida) == 4)
		return HT_4Baida;
	//七对子
	if(cards.size() == 14)
	{
		bool b7dui = true;
		for(int i=0;i<=12;i+=2)
		{
			if(cards[i] != cards[i+1])
			{
				b7dui = false;
				break;
			}
		}
		if(b7dui) return HT_7Duizi;

	}


	if(cards.size()%3 != 2)
		return HT_Null;
	const_CardIter itr1 = cards.begin();
	const_CardIter itr2 = cards.begin()+2;
	CardVector temCards(itr1,itr2);
	if(!_CheckHu_Duizi(temCards))
		return HT_Null;
	if(cards.size() == 2)
		return HT_Normal;
	while((itr1 = itr2) != cards.end())
	{
		itr2 += 3;
		temCards.clear();
		temCards.assign(itr1,itr2);
		if(_CheckHu_Three(temCards) == Pitch_Null)
			return HT_Null;
	}
	return HT_Normal;
}

uint16_t CGameLogic::FindCardCount( const CardVector cards,MJCARD card )
{
	uint16_t wCount = 0;
	for(const_CardIter itr = cards.begin();itr != cards.end();++itr)
		if(*itr == card) 
			++wCount;
	return wCount;
}

MJCARD CGameLogic::SearchOutCard(const CardVector cards)
{
	CardVector remainCardIndexs;
	uint16_t wCardCount = 0;
	for (const_CardIter itr=cards.begin();itr !=cards.end();++itr)
	{
		if(CARD_COLOR(*itr) < MJCARD_COLOR_WAN || CARD_COLOR(*itr) > MJCARD_COLOR_ZI)
			return false;

		if(m_cardBaida != 0 && *itr == m_cardBaida)
			continue;
		if(*itr >= remainCardIndexs.size())
			remainCardIndexs.resize(*itr+1);
		++remainCardIndexs[*itr];
		++wCardCount;
	};
	uint8_t cbReserveCard1 = MJCARD_NULL, cbReserveCard2 = MJCARD_NULL,reserveCardEx = MJCARD_NULL;

	for (size_t i=0;i<remainCardIndexs.size();++i)
	{
		if(remainCardIndexs[i] > 3)
			reserveCardEx = i;
		if(remainCardIndexs[i] >= 3)
		{
			wCardCount -= remainCardIndexs[i];
			remainCardIndexs[i] = 0;
		}
		else if(i < remainCardIndexs.size() -2 && i < MJCARD_COLOR_ZI && remainCardIndexs[i] > 0 
			&& remainCardIndexs[i+1] > 0 && remainCardIndexs[i+2] > 0)
		{
			--remainCardIndexs[i];
			--remainCardIndexs[i+1];
			--remainCardIndexs[i+2];
			wCardCount -= 3;

		}
	}

	//提取出风字单张出牌
	for(size_t i=MJCARD_COLOR_ZI;i<remainCardIndexs.size();++i)
	{
		if(i >= remainCardIndexs.size())
			break;
		if(remainCardIndexs[i] == 1)
			return i;
		else if(remainCardIndexs[i] == 2)
			reserveCardEx = i;
	}
	//提取出靠边的单张出牌,如果有连,则预备
	for(int i=MJCARD_COLOR_WAN; i<MJCARD_COLOR_ZI; ++i)
	{
		if(i >= remainCardIndexs.size())
			break;
		if(remainCardIndexs[i] == 0 )
			continue;
		uint8_t cbValue = CARD_VALUE(i);
		uint8_t cbNearBaidaValue1 = 0,cbNearBaidaValue2 = 0;
		if(m_cardBaida != 0 && CARD_COLOR(m_cardBaida) < MJCARD_COLOR_ZI)
		{
			cbNearBaidaValue1 = CARD_VALUE(m_cardBaida)-1;
			cbNearBaidaValue2 = CARD_VALUE(m_cardBaida)+1;
		}
		if(cbValue == MJCARD_VALUE_1 || cbValue == MJCARD_VALUE_9 
			|| cbValue == cbNearBaidaValue1 || cbValue == cbNearBaidaValue2)
		{
			 if(i < remainCardIndexs.size()-1 && remainCardIndexs[i] == 1 && (remainCardIndexs[i-1] > 0 || remainCardIndexs[i+1] > 0))
				 cbReserveCard1 = i;
			 else if(i<remainCardIndexs.size() - 2 && remainCardIndexs[i] == 1 && (remainCardIndexs[i-2] > 0 || remainCardIndexs[i+2] > 0))
				 cbReserveCard2 = i;
			 else if(remainCardIndexs[i] == 1)
				 return i;
			remainCardIndexs[i] = 0;
		}
	}
	//提取出普通单张出牌,如果有连,则预备
	for(int i=MJCARD_COLOR_WAN; i<MJCARD_COLOR_ZI; ++i)
	{
		if(i >= remainCardIndexs.size())
			break;

		if(remainCardIndexs[i] == 0 )
			continue;

		if(i<remainCardIndexs.size()-1 && remainCardIndexs[i] == 1 && (remainCardIndexs[i-1] > 0 || remainCardIndexs[i+1] > 0))
		{
			if(cbReserveCard1 == MJCARD_NULL)
				cbReserveCard1 = i;
		}
		else if(i<remainCardIndexs.size()-2 && remainCardIndexs[i] == 1 && (remainCardIndexs[i-2] > 0 || remainCardIndexs[i+2] > 0))
		{
			if(cbReserveCard2 == MJCARD_NULL)
				cbReserveCard2 = i;
		}
		else if(remainCardIndexs[i] == 1)
			return i;
		remainCardIndexs[i] = 0;		
	}
	//出预备牌
	if(cbReserveCard2 != MJCARD_NULL)
		return cbReserveCard2;
	if(cbReserveCard1 != MJCARD_NULL)
		return cbReserveCard1;
	if(reserveCardEx != MJCARD_NULL)
		return reserveCardEx;
	//出最后一张牌
	return cards.back();
}


//深度优先
//未考虑爆头优先
bool CGameLogic::_Hu( tagHuData &huData )
{
	//遍历完毕
	if(huData.cbRemainCount == 0)
	{
		if(huData.headCards.size() == 0 && huData.cbBaidaCount == 2)
		{
			huData.headCards.assign(2,m_cardBaida);
			huData.cbBaidaCount = 0;
		}
		else if(huData.headCards.size() == 3 && huData.cbBaidaCount == 3)
		{
			huData.pitchCardDatas.insert(huData.pitchCardDatas.end(),3,m_cardBaida);
			huData.cbBaidaCount = 0;
		}
		return huData.headCards.size() == 2;
	}
	// 找出一张,递归
	for(uint8_t i = huData.remainCardIndexs.size()-1; i > 0; --i)
	{
		if(huData.remainCardIndexs[i] == 0)
			continue;
		//做头
		if(huData.headCards.size()==0)
		{
			if(huData.remainCardIndexs[i]>=2)
			{
				huData.headCards.assign(2,i);
				huData.remainCardIndexs[i] -= 2;
				huData.cbRemainCount -= 2;
				if(_Hu(huData)) return true;
				huData.cbRemainCount += 2;
				huData.remainCardIndexs[i] += 2;
				huData.headCards.clear();
			}
			else if(huData.cbBaidaCount > 0 && huData.remainCardIndexs[i]>=1)
			{
				huData.headCards.push_back(i);
				huData.headCards.push_back(m_cardBaida);
				--huData.remainCardIndexs[i];
				--huData.cbRemainCount;
				--huData.cbBaidaCount;
				if(_Hu(huData)) return true;
				++huData.cbBaidaCount;
				++huData.cbRemainCount;
				++huData.remainCardIndexs[i];
				huData.headCards.clear();
			}
		}
		//做刻
		if(huData.remainCardIndexs[i]>=3)
		{
			huData.pitchCardDatas.insert(huData.pitchCardDatas.end(),3,i);
			huData.remainCardIndexs[i] -= 3;
			huData.cbRemainCount -= 3;
			if(_Hu(huData)) return true;
			huData.cbRemainCount += 3;
			huData.remainCardIndexs[i] += 3;
			huData.pitchCardDatas.resize(huData.pitchCardDatas.size()-3);
		}
		else if(huData.remainCardIndexs[i] > 0 && huData.remainCardIndexs[i]+huData.cbBaidaCount >= 3)
		{
			uint8_t cbCount = huData.remainCardIndexs[i];
			huData.pitchCardDatas.insert(huData.pitchCardDatas.end(),cbCount,i);
			huData.pitchCardDatas.insert(huData.pitchCardDatas.end(),3-cbCount,m_cardBaida);

			huData.remainCardIndexs[i] -= cbCount;
			huData.cbRemainCount -= cbCount;
			huData.cbBaidaCount -= (3-cbCount);
			if(_Hu(huData)) return true;
			huData.cbBaidaCount += (3-cbCount);
			huData.cbRemainCount += cbCount;
			huData.remainCardIndexs[i] += cbCount;
			huData.pitchCardDatas.resize(huData.pitchCardDatas.size()-3);

		}
		//风字不能做顺
		if(CARD_COLOR(i) >= MJCARD_COLOR_ZI)
			return false;
		
		//做顺
		if(i > 2 && huData.remainCardIndexs[i-1] > 0 && huData.remainCardIndexs[i-2] > 0)
		{
			huData.pitchCardDatas.push_back(i);
			huData.pitchCardDatas.push_back(i-1);
			huData.pitchCardDatas.push_back(i-2);
			--huData.remainCardIndexs[i];
			--huData.remainCardIndexs[i-1];
			--huData.remainCardIndexs[i-2];
			huData.cbRemainCount -= 3;
			if(_Hu(huData)) return true;
			huData.cbRemainCount += 3;
			++huData.remainCardIndexs[i];
			++huData.remainCardIndexs[i-1];
			++huData.remainCardIndexs[i-2];
			huData.pitchCardDatas.resize(huData.pitchCardDatas.size()-3);
		}
		if(huData.cbBaidaCount > 0)
		{
			//财神卡张
			if(i > 2 && huData.remainCardIndexs[i-2] > 0)
			{
				huData.pitchCardDatas.push_back(i);
				huData.pitchCardDatas.push_back(m_cardBaida);
				huData.pitchCardDatas.push_back(i-2);
				--huData.cbBaidaCount;
				--huData.remainCardIndexs[i];
				--huData.remainCardIndexs[i-2];
				huData.cbRemainCount -= 2;
				if(_Hu(huData)) return true;
				huData.cbRemainCount += 2;
				++huData.remainCardIndexs[i];
				++huData.remainCardIndexs[i-2];
				++huData.cbBaidaCount;
				huData.pitchCardDatas.resize(huData.pitchCardDatas.size()-3);
			}
			//财神边张
			if(i > 1 && huData.remainCardIndexs[i-1] > 0)
			{
				huData.pitchCardDatas.push_back(i);
				huData.pitchCardDatas.push_back(i-1);
				huData.pitchCardDatas.push_back(m_cardBaida);
				--huData.cbBaidaCount;
				--huData.remainCardIndexs[i];
				--huData.remainCardIndexs[i-1];
				huData.cbRemainCount -= 2;
				if(_Hu(huData)) return true;
				huData.cbRemainCount += 2;
				++huData.remainCardIndexs[i];
				++huData.remainCardIndexs[i-1];
				++huData.cbBaidaCount;
				huData.pitchCardDatas.resize(huData.pitchCardDatas.size()-3);
			}
		}
		return false;
	}
	return false;
}


bool CGameLogic::_CheckHu_Duizi( const CardVector cards )
{
	if(cards.size() != 2)
		return false;
	if(m_cardBaida != 0 && (cards[0] == m_cardBaida || cards[1] == m_cardBaida))
		return true;
	return cards[0] == cards[1];
}

emMjPitchType CGameLogic::_CheckHu_Three( const CardVector cards )
{
	if(cards.size() != 3)
		return Pitch_Null;

	CardVector tmpCards(cards.begin(),cards.end());
	if(m_cardBaida != 0 && std::find(tmpCards.begin(),tmpCards.end(),m_cardBaida) != tmpCards.end())
	{
		CardIter itrRmv = std::remove(tmpCards.begin(),tmpCards.end(),m_cardBaida);
		tmpCards.erase(itrRmv,tmpCards.end());
		SortCards(tmpCards);
		if(tmpCards.size() <= 1)
			return Pitch_Peng;
		if(tmpCards[0] == tmpCards[1])
			return Pitch_Peng;
		if(CARD_COLOR(tmpCards[0]) < MJCARD_COLOR_ZI && (tmpCards[1] - tmpCards[0]) <= 2)
			return Pitch_Shun;
	}
	else
	{
		SortCards(tmpCards);
		if(tmpCards[0] == tmpCards[1] && tmpCards[0] == tmpCards[2])
			return Pitch_Peng;
		else if(tmpCards[0] + 1 == tmpCards[1] && tmpCards[1] + 1 == tmpCards[2])
			return Pitch_Shun;
	}
	return Pitch_Null;
}

uint16_t CGameLogic::GetRandUnsocialCard( const CardVector cards )
{
	int nStartIndex = rand()%cards.size();
	int nMaxDistance = -1;
	uint16_t nIndexRet = 0xFF;
	for (int i = nStartIndex; i >= 0; --i)
	{
		if (cards[i] == m_cardBaida)
			continue;
		int nDistance = GetCardsDistance(cards, i);
		if (nDistance > nMaxDistance)
		{
			nMaxDistance = nDistance;
			nIndexRet = i;
			if (nDistance > 2)
				return nIndexRet;
		}
	}
	for (size_t i = nStartIndex+1; i < cards.size(); ++i)
	{
		if (cards[i] == m_cardBaida)
			continue;
		int nDistance = GetCardsDistance(cards, i);
		if (nDistance > nMaxDistance)
		{
			nMaxDistance = nDistance;
			nIndexRet = i;
			if (nDistance > 2)
				return nIndexRet;
		}
	}

	return nIndexRet;
}

/*
确保cards已经排序过
返回值定义:
	0: 有同张.如22		
	1: 有粘连张.如23		
	2: 有隔张.如24
	0xFF: 孤立
*/
uint8_t CGameLogic::GetCardsDistance( const CardVector cards, int nIndex )
{
	if (cards.size() <= 1)
		return 0xFF;
	if (nIndex == 0)
		return GetCardsDistance(cards[0], cards[1]);
	else if (nIndex == cards.size()-1)
		return GetCardsDistance(cards[nIndex], cards[nIndex-1]);
	if(cards.size() <= 2)
		return 0xFF;
	return std::min<uint8_t>(GetCardsDistance(cards[nIndex], cards[nIndex-1]),GetCardsDistance(cards[nIndex], cards[nIndex+1]));	
}
/*
返回值定义:
0: 同张.如22		
1: 粘连张.如23		
2: 隔张.如24
0xFF: 孤立
*/
uint8_t CGameLogic::GetCardsDistance( MJCARD card1, MJCARD card2 )
{
	if (card1 == card2)
		return 0;
	//有一个财神,则必定处于孤立状态
	else if (card1 == m_cardBaida || card2 == m_cardBaida)
		return 0xFF;
	//有风字,则必定处于孤立状态
	else if (CARD_COLOR(card1) == MJCARD_COLOR_ZI || CARD_COLOR(card2) == MJCARD_COLOR_ZI)
		return 0xFF;

	if (card2 == card1+1 || card2 == card1-1)
		return 1;
	else if (card2 == card1+2 || card2 == card1-2)
		return 2;
	return 0xFF;
}

bool CGameLogic::CheckCanFraudHu( const CardVector cards, MJCARD newCatchCard )
{
	CardVector tmpCards;
	tmpCards.assign(cards.begin(), cards.end());
	SortCards(tmpCards);
	int baidaCardCount = 0;
	int aloneCardCount = 0;
	if (m_cardBaida != 0)
		baidaCardCount = FindCardCount(cards,m_cardBaida);
	for (int i = 0; i < baidaCardCount; ++i)
		RemoveCard(tmpCards, m_cardBaida);
	for (int i = 0; i < tmpCards.size(); ++i)
	{
		if (GetCardsDistance(tmpCards,i) == 0xFF)
			++ aloneCardCount;
	}
	return (baidaCardCount >= aloneCardCount) && !CheckHu(cards);
}
