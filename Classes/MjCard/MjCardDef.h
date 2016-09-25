#ifndef MJTEMPLATE_CARDDEF_H
#define MJTEMPLATE_CARDDEF_H

#pragma once

#define GAME_PLAYER 4

#define MJCARD_COUNT_NOFLOWER		136			//�޻�����
#define MJCARD_COUNT_FLOWER			144			//��������
#define MJCARD_COUNT	MJCARD_COUNT_NOFLOWER

#define MJCARD_HAND_COUNT_START		13			//��������
#define MJCARD_MAX_HAND_COUNT		14			//��������


typedef unsigned char MJCARD;

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
// #define MJCARD_COLOR_FENG		0X40		//��
#define MJCARD_COLOR_ZI		0X40		//��
#define MJCARD_COLOR_HUA		0X50		//��
#define CARD_COLOR(c)			(c & MJCARD_COLOR_MASK)

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
#define MJCARD_VALUE_ZHONG			0x05
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

#define CARD_VALUE(c)				(c & MJCARD_VALUE_MASK)

#define MJCARD_BACK					0
#define MJCARD_INVALID				0xFF

#define EVENT_KEYID_COUNT 75
#define EVENT_KEYID_TYPE_COUNT 71

enum emDirection
{
	DIR_NULL	= -1,
	DIR_BOTTOM	= 0,
	DIR_RIGHT	= 1,
	DIR_TOP	= 2,
	DIR_LEFT	= 3,
	DIR_COUNT = 4,

};

enum emMjStatus
{
	MjStatus_Stand,
	MjStatus_Face,	//����
	MjStatus_Back,	//����
	MjStatus_Count
};

#endif //MJTEMPLATE_CARDDEF_H