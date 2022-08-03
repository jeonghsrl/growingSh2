/****************************************************************************/
/*																			*/
/*		STK-7125 SCI �T���v���v���O���� �w�b�_�t�@�C��			 			*/
/*																			*/
/*			Copyright   :: ���A���t�@�v���W�F�N�g							*/
/*			Language    :: SH-C												*/
/*			File Name   :: sci.h											*/
/*																			*/
/****************************************************************************/
#ifndef __SCI_H__
#define __SCI_H__

// �g�p����SCI�̃`���l���w��
#define	SCICH		1		// SCI�`���l���ԍ� 0 or 1

#if SCICH==0
#define	SCI			SCI0	// SCI�`���l��0
#define	SCI_IPRMSK	0xf000	// SCI�`���l��0 IPRL MASK
#define	SCI_IPRSFT	12		// SCI�`���l��0 IPRL SHIFT
#else
#define	SCI			SCI1	// SCI�`���l��1
#define	SCI_IPRMSK	0x0f00	// SCI�`���l��1 IPRL MASK
#define	SCI_IPRSFT	8		// SCI�`���l��1 IPRL SHIFT
#endif

#define	sciEmpty	-1		// ��M�o�b�t�@��

// SCI�p���e�B��`
typedef enum _sciPr {
	sciPrNone = 0,
	sciPrEven = 2,
	sciPrOdd  = 3,
} sciPr;

// SCI�o�b�t�@�\����
typedef struct _sciBuf {
	char * buf;				// �o�b�t�@�{�̂ւ̃|�C���^
	unsigned long size;		// �o�b�t�@�T�C�Y
	unsigned long wp;		// �����݃|�C���^
	unsigned long rp;		// �Ǐo���|�C���^
} sciBuf;

// SCI�������p�����[�^�\����
typedef struct _sciPara {
	sciBuf * rec;			// ��MSCI�o�b�t�@�\���̂ւ̃|�C���^
	sciBuf * send;			// ���MSCI�o�b�t�@�\���̂ւ̃|�C���^
	long bps;				// ���[�g   : 2400,4800,9600,19200,38400
	short bit_len;			// �r�b�g��	: 8 or 7
	short stop_bit;			// �X�g�b�v	: 1 or 2
	sciPr parity;			// �p���e�B	: sciPrNone,sciPrEven,sciPrOdd
	short int_lv;			// �������� : 0-15
} sciPara;

// �v���g�^�C�v�錾
void sci_init(void);
void open_sci(sciPara *);
void close_sci(void);
short sci_putc(char);
short sci_getc(void);
void printDecimal(int d);
void sputs(char *p);
double adhen(void);
double get_keisan(void);
double feed_back(void);

void eri_int(void);
void rxi_int(void);
void txi_int(void);
void tei_int(void);

#endif		// END _SCI_H_
