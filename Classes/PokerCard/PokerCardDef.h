#ifndef CARDDEF_H
#define CARDDEF_H
#pragma once
#include <vector>


#define PACK_CELL_COUNT				54									// ÿ��������

typedef unsigned char CARD;
typedef std::vector<CARD> CardVector;


// �˿�������
#define	CARD_COLOR_MASK				0xF0								// ��ɫ����
#define	CARD_VALUE_MASK				0x0F								// ��ֵ����
#define MAKE_CARD(c, v)				(c | v)

// ��ɫ����
#define CARD_COLOR_NULL				0x00								// ��Ч��ɫ
#define CARD_COLOR_FANG_KUAI		0x10								// ���黨ɫ
#define CARD_COLOR_MEI_HUA			0x20								// ÷����ɫ
#define CARD_COLOR_HONG_TAO			0x30								// ���һ�ɫ
#define CARD_COLOR_HEI_TAO			0x40								// ���һ�ɫ
#define CARD_COLOR_JOKER			0x50								// ���ƻ�ɫ
#define CARD_COLOR(c)				(c & CARD_COLOR_MASK)

// ��ֵ����
#define CARD_VALUE_NULL				0x00
#define CARD_VALUE_A				0x01
#define CARD_VALUE_2				0x02
#define CARD_VALUE_3				0x03
#define CARD_VALUE_4				0x04
#define CARD_VALUE_5				0x05
#define CARD_VALUE_6				0x06
#define CARD_VALUE_7				0x07
#define CARD_VALUE_8				0x08
#define CARD_VALUE_9				0x09
#define CARD_VALUE_10				0x0A
#define CARD_VALUE_J				0x0B
#define CARD_VALUE_Q				0x0C
#define CARD_VALUE_K				0x0D
#define CARD_VALUE_BJOKER			0x0E
#define CARD_VALUE_RJOKER			0x0F
#define CARD_VALUE(c)				(c & CARD_VALUE_MASK)

#define CARD_NULL					0x00								// ��Ч��
#define CARD_BJOKER					0x5E								// С��
#define CARD_RJOKER					0x5F								// ����
#define CARD_BACK					0x00


//////////////////////////////////////////////////////////////////////////
typedef CARD MJCARD;	
typedef CardVector MjCardVector;

// �˿�������
#define	MJCARD_NULL						0x00								// �Ʊ�
#define	MJCARD_COLOR_MASK				0xF0								// ��ɫ����
#define	MJCARD_VALUE_MASK				0x0F								// ��ֵ����
#define MAKE_MJCARD(c, v)				(c | v)

//��ɫ����
#define MJCARD_COLOR_NULL		0X00		//NULL
#define MJCARD_COLOR_WAN		0X10		//��
#define MJCARD_COLOR_TONG		0X20		//Ͳ
#define MJCARD_COLOR_TIAO		0X30		//��
#define MJCARD_COLOR_ZI			0X40		//����
#define MJCARD_COLOR_HUA		0X50		//��
#define MJCARD_COLOR(c)			(c & MJCARD_COLOR_MASK)

//ֵ����
#define MJCARD_VALUE_NULL			0x00
#define MJCARD_VALUE_1				0x01
#define MJCARD_VALUE_2				0x02
#define MJCARD_VALUE_3				0x03
#define MJCARD_VALUE_4				0x04
#define MJCARD_VALUE_5				0x05
#define MJCARD_VALUE_6				0x06
#define MJCARD_VALUE_7				0x07
#define MJCARD_VALUE_8				0x08
#define MJCARD_VALUE_9				0x09

#define MJCARD_VALUE_DONG			0x01
#define MJCARD_VALUE_NAN			0x02
#define MJCARD_VALUE_XI				0x03
#define MJCARD_VALUE_BEI			0x04
#define MJCARD_VALUE_ZHONG		0x05
#define MJCARD_VALUE_FA				0x06
#define MJCARD_VALUE_BAI			0x07

#define MJCARD_VALUE_FLOWER1		0x01
#define MJCARD_VALUE_FLOWER2		0x02
#define MJCARD_VALUE_FLOWER3		0x03
#define MJCARD_VALUE_FLOWER4		0x04
#define MJCARD_VALUE_FLOWER5		0x05
#define MJCARD_VALUE_FLOWER6		0x06
#define MJCARD_VALUE_FLOWER7		0x07
#define MJCARD_VALUE_FLOWER8		0x08

#define MJCARD_VALUE(c)				(c & MJCARD_VALUE_MASK)

#define MJCARD_BACK					0
#define MJCARD_INVALID				0xFF

#endif