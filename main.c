#include<stdio.h>
#include<stdlib.h>
#include<string.h>
//#include<stdint.h>  // ���ڲ�ͬƽ̨int��С������

typedef struct _MsgInt {
	unsigned int* msgInt;//ԭʼ��Ϣ�ַ�ת��Ϊintָ�����飬ÿ4���ַ�ת��Ϊһ��int����
	int intCount;//int������
}MsgInt;

typedef struct _ExtendMsgInt {
	unsigned int W[68];
	unsigned int W1[64];
}ExtendMsgInt;

// =====================================================================
unsigned int chars_unit32(unsigned int n, char b[], int i)
{
	(n) = ((unsigned int)(b)[(i)] << 24)
		| ((unsigned int)(b)[(i)+1] << 16)
		| ((unsigned int)(b)[(i)+2] << 8)
		| ((unsigned int)(b)[(i)+3]);
	return n;
}
//��1��32λ�������洢��չλ�ĸ��ַ�
void unit32_chars(unsigned int n, char b[], int i)
{
	b[i] = (char)((n) >> 24);
	b[i + 1] = (char)((n) >> 16);
	b[i + 2] = (char)((n) >> 8);
	b[i + 3] = (char)((n));

}

void bigEnd()
{
	char c;
	int a = 1;
	// ����intΪ4Byte���߼�˳��չ����16����1=0x0000_0001��
	// ����Ǵ�������ڴ���ͬ�߼�˳�� 00 00 00 01��
	// ��С��������ڴ����� 01 00 00 00 �����ֽڣ���8bitΪ�飬�ߵ͵�ַ�ߵ���
	int* p = &a; // ��pָ��ָ��a���׵�ַ
	c = *p; // c��char���ͣ�1Byte����ȡ�׵�ַ��ʼ1���ֽ�
	if (c == a) // ��� c��ֵ�պ���1=a�Ļ���˵���ߵ��ˣ�����С��
	{
		printf("����ΪС�˴洢\n");
	}
	else
	{
		printf("����Ϊ��˴洢\n");
	}
}

//void test()
//{
//	unsigned char a[2] = { 0b01100001, 0b01100010 };
//	unsigned char b[2] = { 0b01100011, 0b01100010 };
//	//						   0b01100001, 0b01100010
//	unsigned char* c = (unsigned char*)malloc(2);
//
//	//*c = *a & *b;
//}

void test2()
{
	unsigned char* str = "abcdefg";
	unsigned int* a = (unsigned int*)malloc(sizeof(*str));
	unsigned int n = 0;
	char* s = "abcd";

	a = str;

	printf("%x\n", *a);
	printf("%s\n", a);

	unsigned int b = chars_unit32(n, s, 0);

	printf("%d\n", b);
}

void test3()
{
	unsigned char a = 'a';
	unsigned char b = 'b';
	unsigned char c = 'c';
	unsigned char d = 'd';

	unsigned int m = (unsigned int)"abcd";
	unsigned int aa = (unsigned int)a;
	unsigned int aaa = aa << 24;
	unsigned int bb = (unsigned int)b;
	unsigned int bbb = bb << 16;
	unsigned int dd = (unsigned int)d;

	unsigned int n = ((unsigned int)a << 24)
		| ((unsigned int)b << 16)
		| ((unsigned int)c << 8)
		| ((unsigned int)d);

	printf("%d\n", m);
	printf("%x\n", n);

	printf("%d\n", sizeof(unsigned long));
	printf("%d\n", sizeof(unsigned int));
	printf("%d\n", sizeof(unsigned __int32)); // ΢���׼
	printf("%d\n", sizeof(unsigned __int64));
	//printf("%d\n", sizeof(int32_t));

}

void test4()
{
	unsigned long long a = 0x7;
	printf("%d\n", sizeof(unsigned long));
}

//---------------------------------------------------------------------------------------------------


// ��ʼ����
const unsigned int IV[8] = {
	0x7380166F,0x4914B2B9,0x172442D7,0xDA8A0600,
	0xA96F30BC,0x163138AA,0xE38DEE4D,0xB0FB0E4E
};

/*
 * �꺯��NOT_BIG_ENDIAN()
 * ���ڲ������л����Ƿ�Ϊ��ˣ�С�˷���true
 */
static const int endianTestNum = 1;
#define NOT_BIG_ENDIAN() ( *(char *)&endianTestNum == 1 )

#define FF_LOW(x,y,z) ( (x) ^ (y) ^ (z))
#define FF_HIGH(x,y,z) (((x) & (y)) | ( (x) & (z)) | ( (y) & (z)))

//#define FF(x,y,z,j)									\
//{													\
//	if(j<16){										\
//		((x) ^ (y) ^ (z));							\
//	}												\
//	else{											\
//		(((x)& (y)) | ((x) & (z)) | ((y) & (z)));	\
//	}												\
//}




#define GG_LOW(x,y,z) ( (x) ^ (y) ^ (z))
#define GG_HIGH(x,y,z) (((x) & (y)) | ( (~(x)) & (z)) )


//unsigned int ROTATE_LEFT(unsigned int word, unsigned int bits)
//{
//	word = ((word) << (bits) | (word) >> (32 - (bits)));
//	return word;
//}

//#define ROTATE_LEFT(uint32,shift) ( (uint32) = ( ( (uint32) << (shift) ) | ( (uint32) >> (32 - (shift)) ) ) )
// ����д�Ļ�ı䴫��Ĳ�����ֵ��Ӱ��MsgExtend������W ������
#define ROTATE_LEFT(uint32,shift) ( ( ( (uint32) << (shift) ) | ( (uint32) >> (32 - (shift)) ) ) )

//#define  SHL(x,n) (((x) & 0xFFFFFFFF) << n)
//#define ROTL(x,n) (SHL((x),n) | ((x) >> (32 - n)))

#define P0(x) ((x) ^  ROTATE_LEFT((x),9) ^ ROTATE_LEFT((x),17))
#define P1(x) ((x) ^  ROTATE_LEFT((x),15) ^ ROTATE_LEFT((x),23))

/*
 * �꺯��UCHAR_2_UINT(uchr8,uint32,i,notBigendian)
 * uchr8        -- unsigned char - 8bit
 * uint32       -- unsigned int  - 32bit
 * i            -- int
 * notBigendian -- int/bool
 * ��uchr8���յ��ַ�����,ת���ɴ�˱�ʾ��uint32���ӵײ㿴�����ư����λ�ҵ�λ���У�
 * NOT_BIG_ENDIAN()���黷�����Ǵ��ʱnotBigendianΪ�棬���ô˺꺯��
 */
#define UCHAR_2_UINT(uchr8,uint32,i,notBigendian)				\
{																\
	if(notBigendian){                                           \
		(uint32) = ((unsigned int) (uchr8)[(i)    ] << 24 )		\
				 | ((unsigned int) (uchr8)[(i) + 1] << 16 )		\
				 | ((unsigned int) (uchr8)[(i) + 2] << 8  )		\
				 | ((unsigned int) (uchr8)[(i) + 3]       );	\
	}															\
}

 /*
  * �꺯��UINT_2_UCHAR(uint32,uchr8,i,notBigendian)
  * uchr8        -- unsigned char - 8bit
  * uint32       -- unsigned int  - 32bit
  * i            -- int
  * notBigendian -- int/bool
  * ����˱�ʾ��uint32,ת����uchr8���ַ�����
  * NOT_BIG_ENDIAN()���黷�����Ǵ��ʱnotBigendianΪ�棬���ô˺꺯��
  */
#define UINT_2_UCHAR(uint32,uchr8,i,notBigendian)				\
{																\
	if(notBigendian){                                           \
		(uchr8)[(i)    ] = (unsigned char)((uint32) >> 24);		\
		(uchr8)[(i) + 1] = (unsigned char)((uint32) >> 16);		\
		(uchr8)[(i) + 2] = (unsigned char)((uint32) >> 8 );		\
		(uchr8)[(i) + 3] = (unsigned char)((uint32)      );		\
	}															\
}

MsgInt MsgFill512(unsigned char* msg, int notBigendian)
{
	// int msgLength = sizeof(*msg) * 8; ָ�����鳤�Ȳ�����sizeof��
	//����ָ��������ԣ���Զ��¼���׵�ַ��������¼��ָ������ݵ��ڴ��С�����ָ�������޷��������
	//unsigned char msgArry[] = *msg; // ��˽�ֵ�������飬������Լ��㳤��...�������Ϸ�
	//int msgLength = sizeof(msgArry) / sizeof(msgArry[0]) * 8; //�������Һ�������strlen�ˡ�����
	MsgInt filledMsgInt;
	unsigned long long msgLength = strlen((char*)msg);// ��������и�ǿ��ת����strlen��֧��unsigned char*
	unsigned long long msgbitLength = msgLength * 8; // ��ԭʼ��Ϣ�ı��س���
	int zeroFill = 448 - (msgbitLength + 8) % 512; // +8�ǲ���0x80=0b1000_0000
	unsigned char* zeroChar = (unsigned char*)malloc(zeroFill / 8);

	memset(zeroChar, 0, zeroFill / 8);
	// ������strlen((char*)zeroChar),zeroCharȫ��0�����ַ���������־����0������strlen((char*)zeroChar)=0
	// ֱ����memset���ڴ��п���ֵ
	//zeroChar[zeroFill / 8] = '\0'; // �������Ҫ�ӣ�ֱ�Ӵ��ڴ濽��������ȷ���ǲ����ַ�����Ҳ����ӽ�����
	// ʵ���ϣ�zeroChar��������ȫ��0�����ַ���ʶ���������ǽ�����/0

	int totalChrLength = msgLength + 1 + zeroFill / 8 + 8;

	filledMsgInt.msgInt = (unsigned int*)malloc(totalChrLength / 4);
	filledMsgInt.intCount = totalChrLength / 4;//totalChrLength���ַ�����8bit/����msgIntΪ32bit/��

	unsigned char* msgFill = (unsigned char*)malloc(totalChrLength);// 1��ʾ0x80�ĳ��ȣ�һ���ֽ�
	memcpy(msgFill, msg, msgLength);
	unsigned char one = 0x80;
	memcpy(msgFill + msgLength, &one, 1);
	memcpy(msgFill + msgLength + 1, zeroChar, zeroFill / 8);
	//unsigned char* msgLenChr = (unsigned char*)msgLength;
	//memcpy(msgFill + msgLength + 1  + zeroFill / 8, msgLenChr, 8); // �������������

	unsigned char msgLenChr[8];
	if (notBigendian) { // С��ϵͳ��long long �������ڴ��еߵ��洢�ģ�������Ҫת��
		for (int i = 0; i < 8; i++) {
			msgLenChr[i] = msgbitLength >> (56 - 8 * i);
		}
		memcpy(msgFill + msgLength + 1 + zeroFill / 8, msgLenChr, 8);
	}
	else { // ����Ǵ��ϵͳ��ֱ�ӿ���msgbitLength�ڴ����ݼ���
		memcpy(msgFill + msgLength + 1 + zeroFill / 8, &msgbitLength, 8);
	}

	/*printf("%d\n", msgbitLength);
	printf("%d\n", zeroFill);
	printf("%d\n", zeroFill / 8);
	printf("%s\n", zeroChar);
	printf("%d\n", strlen(zeroChar));*/

	for (int i = 0; i < filledMsgInt.intCount; i++) {
		unsigned char msgSlice[4] = { *(msgFill + i * 4),*(msgFill + i * 4 + 1),*(msgFill + i * 4 + 2),*(msgFill + i * 4 + 3) };
		unsigned int a = (unsigned int*)msgSlice;
		UCHAR_2_UINT(msgSlice, filledMsgInt.msgInt[i], 0, notBigendian);
	}

	return filledMsgInt;
}

ExtendMsgInt MsgExtend(unsigned int msgInt16[])
{
	ExtendMsgInt etdMsgInt;

	for (int i = 0; i < 16; i++) {
		etdMsgInt.W[i] = msgInt16[i];
	}
	for (int j = 16; j < 68; j++) {
		unsigned int tmp;
		tmp = etdMsgInt.W[j - 16] ^ etdMsgInt.W[j - 9] ^ ROTATE_LEFT(etdMsgInt.W[j - 3], 15);
		// �����ҵ�W�����Ī�����޸ĵ�ԭ���ˣ�������ROTATE_LEFT��ı䴫��Ĳ���
		etdMsgInt.W[j] = P1(tmp) ^ ROTATE_LEFT(etdMsgInt.W[j - 13], 7) ^ etdMsgInt.W[j - 6];
	}
	for (int j = 0; j < 64; j++) {
		etdMsgInt.W1[j] = etdMsgInt.W[j] ^ etdMsgInt.W[j + 4];
	}
	return etdMsgInt;
}

unsigned int* CF(unsigned int Vi[], unsigned int msgInt16[], unsigned int W[], unsigned int W1[])
{
	unsigned int regA2H[8]; // A~H 8���Ĵ���
	unsigned int SS1, SS2, TT1, TT2;

	for (int i = 0; i < 8; i++) {
		regA2H[i] = Vi[i];
	}
	for (int j = 0; j < 64; j++) {
		unsigned int T = 0x79cc4519; // �ĵ��еĳ���Tj���˴���T
		if (j >= 16) {
			T = 0x7a879d8a;
		}
		SS1 = ROTATE_LEFT(ROTATE_LEFT(regA2H[0], 12) + regA2H[4] + ROTATE_LEFT(T, j), 7);
		SS2 = SS1 ^ ROTATE_LEFT(regA2H[0], 12);
		if (j < 16) {
			TT1 = FF_LOW(regA2H[0], regA2H[1], regA2H[2]) + regA2H[3] + SS2 + W1[j];
			TT2 = GG_LOW(regA2H[4], regA2H[5], regA2H[6]) + regA2H[7] + SS1 + W[j];
		}
		else {
			TT1 = FF_HIGH(regA2H[0], regA2H[1], regA2H[2]) + regA2H[3] + SS2 + W1[j];
			TT2 = GG_HIGH(regA2H[4], regA2H[5], regA2H[6]) + regA2H[7] + SS1 + W[j];
		}
		regA2H[3] = regA2H[2];
		regA2H[2] = ROTATE_LEFT(regA2H[1], 9);
		regA2H[1] = regA2H[0];
		regA2H[0] = TT1;
		regA2H[7] = regA2H[6];
		regA2H[6] = ROTATE_LEFT(regA2H[5], 19);
		regA2H[5] = regA2H[4];
		regA2H[4] = P0(TT2);
	}
	for (int i = 0; i < 8; i++) { // ���� ABCDEFH ^ Vi
		regA2H[i] ^= Vi[i];
	}
	return regA2H;
}

unsigned int* SM3Hash(unsigned char* msgText, int notBigendian) {
	MsgInt filledMsgInt = MsgFill512(msgText, notBigendian);
	// �����õ���Ϣ��512bit���з��飬��ÿ16��intһ��
	int groupAmount = filledMsgInt.intCount / 16; 
	//unsigned int* V = IV;

	unsigned int V[8];
	for (int i = 0; i < 8; i++) {
		V[i] = IV[i];
	}
	for (int i = 0; i < groupAmount; i++) {
		unsigned int* bi = 16 * i + filledMsgInt.msgInt;
		ExtendMsgInt etdMsgInt = MsgExtend(bi);
		unsigned int* temp = CF(V, bi, etdMsgInt.W, etdMsgInt.W1); // ÿһ��ѹ������V
		for (int i = 0; i < 8; i++) {
			V[i] = temp[i];
		}
	}
	return V;
	char sm3HashValue[32];
	for (int i = 0; i < 8; i++) {
		UINT_2_UCHAR(V[i], sm3HashValue, 4 * i, notBigendian);
	}
}


void test()
{
	int bigendFlag = NOT_BIG_ENDIAN();

	unsigned char* chr = "abcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcd";

	MsgInt msgInt;
	ExtendMsgInt etdMsgInt;
	//unsigned char ch[] = *chr;


	msgInt = MsgFill512(chr, bigendFlag);

	//��Ϣ����������
	puts("------- ��Ϣ���������� -------");
	for (int i = 0; i < msgInt.intCount; i++) {
		printf("%08x ", msgInt.msgInt[i]);
	}

	etdMsgInt = MsgExtend(msgInt.msgInt);

	//��Ϣ��չ�������
	puts("\n------- ��Ϣ��չ������� -------");
	printf("W0---W67:\n");
	for (int i = 0; i < 68; i++) {
		printf("%08x ", etdMsgInt.W[i]);
	}
	printf("\n\nW1_0----W1_63:\n");
	for (int i = 0; i < 64; i++) {
		printf("%08x ", etdMsgInt.W1[i]);
	}
}


int main()
{
	int bigendFlag = NOT_BIG_ENDIAN();

	unsigned char* chr = "���ô";
	unsigned int* hashInt = SM3Hash(chr, bigendFlag);

	unsigned int result[8];

	for (int i = 0; i < 8; i++) {
		result[i] = hashInt[i];
	}

	//printf("%08x ", hashInt[0]);
	//printf("%08x ", hashInt[1]);
	//printf("%08x ", hashInt[2]);
	for (int i = 0; i < 8; i++) {
		printf("%08x ", result[i]);
	}
	printf("\n");

	//test();

	return 0;
}
