/****************************************************************************/
/*																			*/
/*		STK-7125 �T���v���v���O����											*/
/*			�V���A������													*/
/*																			*/
/*			Copyright   :: ���A���t�@�v���W�F�N�g							*/
/*			Cpu         :: SH7125											*/
/*			Language    :: SH-C												*/
/*			File Name   :: sci.c											*/
/*																			*/
/****************************************************************************/
#include "global.h"
#include "7125s.h"
#include "sci.h"

// prototype
void open_sci(sciPara * );

/* ����M�o�b�t�@ */
static char rxb[16];
static char txb[16];
// ����M�o�b�t�@�p�����[�^
static const sciBuf rx_prm={ rxb, sizeof(rxb), 0, 0 };
static const sciBuf tx_prm={ txb, sizeof(txb), 0, 0 };
// �������p�����[�^
const sciPara sci_para = { (sciBuf *)&rx_prm, (sciBuf *)&tx_prm, 38400, 8, 1, sciPrNone, 7 };

/* ���[�N�G���A */
static sciBuf rec_buf;
static sciBuf send_buf;

/************************************/
/*	SCI������						*/
/************************************/
void sci_init(void)
{
	/* 38400,8,1,N�ŏ����� */
	open_sci((sciPara *)&sci_para);
}

/************************************/
/*	SCI�I�[�v��						*/
/************************************/
void open_sci(sciPara * para)
{
	unsigned char tmp = 0;

	/* ���[�N�G���A������ */
	rec_buf.buf   = para->rec->buf;
	rec_buf.size  = para->rec->size;
	rec_buf.wp    = rec_buf.rp  = 0;
	send_buf.buf  = para->send->buf;
	send_buf.size = para->send->size;
	send_buf.wp   = send_buf.rp = 0;

	/* ����M�����֎~�A����M�֎~ */
	SCI.SCSCR.BYTE = 0x00;	

	/* �����݃��x���̐ݒ� */
	INTC.IPRL.WORD &= ~SCI_IPRMSK;
	INTC.IPRL.WORD |= (para->int_lv << SCI_IPRSFT) & SCI_IPRMSK;

	/* SMR�ݒ� */
	// �r�b�g��
	if (para->bit_len == 7) {
		tmp |= 0x40;			// 7bit
	}
	else {
		tmp |= 0x00;			// 8bit
	}
	// �p���e�B
	switch(para->parity) {
		case sciPrEven:			// �����p���e�B
			tmp |= 0x20;
			break;
		case sciPrOdd:			// ��p���e�B
			tmp |= 0x30;
			break;
		default:				// �p���e�B����
			tmp |= 0x00;
			break;
	}
	// �X�g�b�v�r�b�g
	if (para->stop_bit == 1) {
		tmp |= 0x00;			// STOP 1
	}
	else {
		tmp |= 0x08;			// STOP 2
	}
	SCI.SCSMR.BYTE = tmp;

	/* BBR�ݒ� */
	tmp = ((PCLK / 32) / para->bps) - 1;
	SCI.SCBRR = tmp;

	/* SSR�ݒ� */
	SCI.SCSSR.BYTE &= ~0x78;		/* �X�e�[�^�X(��M,�G���[)�N���A */
	SCI.SCSCR.BYTE |= 0x70;			/* ��M�������A����M���� */
}


/************************************/
/*	SCI�N���[�Y						*/
/************************************/
void close_sci(void)
{
	SCI.SCSCR.BYTE = 0x00;			/* ����M�����݋֎~�A����M�֎~ */
	INTC.IPRL.WORD &= ~SCI_IPRMSK;	/* ���荞�݃��x���N���A */
}


/************************************/
/*	�P�����i�[						*/
/************************************/
short sci_putc(char data)
{
	unsigned long wp;
	short ans;

	wp = send_buf.wp + 1;
	if (wp >= send_buf.size) {
		wp = 0;
	}

	if (wp != send_buf.rp) {
		send_buf.buf[send_buf.wp] = data;
		send_buf.wp = wp;
		SCI.SCSCR.BYTE |= 0x80;		/* ���M�����݋��� */
		ans = 0;
	}
	else {
		ans = -1;
	}

	return ans;
}


/************************************/
/*	�P�����擾						*/
/************************************/
short sci_getc(void)
{
	unsigned long rp;
	short ans;

	if (rec_buf.rp != rec_buf.wp) {
		ans = (short)rec_buf.buf[rec_buf.rp] & 0x00ff;
		rp = rec_buf.rp + 1;
		if (rp >= rec_buf.size) {
			rp = 0;
		}
		rec_buf.rp = rp;
	}
	else {
		ans = sciEmpty;
	}

	return ans;
}

/************************************/
/*	�V���A��������o�͊֐�			*/
/************************************/
void sputs(char *p)
{
	while(1) {
		if(*p==0x00) break;			// Null�����܂ŏo��
		while(sci_putc(*p)) ;		// ���M�o�b�t�@����t�Ȃ�đ�
		++p;
	}
}


/* 10�i���o��*/
void printDecimal(int d)
{
	char buf1[12],buf2[12];         //data�i�[�p�z��
	int i=0,j=0;
	int sign=0;                     //��������p

	if(d < 0){                      // data < 0�̏ꍇ�A���ɕϊ���Asign=1;
		d = -d ;
		sign=1;
	}	
	if(d == 0){                     // data =0�̏ꍇ�Abuf1��0�Ԗڂɕ���'0'���i�[
		buf1[0]='0';
		i++;
	}
	while(d>0){                     // buf1��data��10�Ŋ������]���'0'�ƈꏏ�Ɋi�[
	 buf1[i] = '0'+d%10;            // data��10�Ŋ����čĊi�[(�]��͖����Ȃ�)  
	 d /= 10;                     
	 i++;	
	}	
	if(sign){buf1[i]='-';i++;}           // sign=1�Ȃ�A�Ō��buf1��'-'���i�[

	for(j=(i-1);j>=0;j--){              // buf1���t���珇�Ԃ�1�����o��
		
	  sci_putc(buf1[j]);
		
	}		
}


/************************************/
/*	��M�G���[���荞��				*/
/************************************/
#pragma interrupt (eri_int)
void eri_int(void)
{
	SCI.SCSSR.BYTE &= ~0x78;		/* �G���[�t���O�N���A */
}


/************************************/
/*	��M���荞��					*/
/************************************/
#pragma interrupt (rxi_int)
void rxi_int(void)
{
	unsigned long wp;
	unsigned char data;

	data = SCI.SCRDR;				/* ��M */
	SCI.SCSSR.BYTE &= ~0x40;		/* RDRF�N���A */
	
	wp = rec_buf.wp + 1;
	if (wp >= rec_buf.size) {
		wp = 0;
	}
	
	if (wp != rec_buf.rp) {
		rec_buf.buf[rec_buf.wp] = data;
		rec_buf.wp = wp;
	}
}


/************************************/
/*	���M���荞��					*/
/************************************/
#pragma interrupt (txi_int)
void txi_int(void)
{
	unsigned char data;

	if (send_buf.rp != send_buf.wp) {
		data = send_buf.buf[send_buf.rp];
		send_buf.rp++;
		if (send_buf.rp >= send_buf.size) {
			send_buf.rp = 0;
		}

		SCI.SCTDR = data;			/* 1�o�C�g���M */
		SCI.SCSSR.BYTE &= ~0x80;	/* TDRE �N���A */
	}
	else {
		SCI.SCSCR.BYTE &= ~0x80;	/* ���M�����݋֎~ */
	}
}


/************************************/
/*	���M�I��������					*/
/************************************/
#pragma interrupt (tei_int)
void tei_int(void)
{
	SCI.SCSCR.BYTE &= ~0x04;		/* ���M�I�������݋֎~ */
}




