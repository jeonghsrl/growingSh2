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

#define  bs 128
/* ����M�o�b�t�@ */
static char rxb[bs];
static char txb[bs];


// ����M�o�b�t�@�p�����[�^
static const sciBuf rx_prm={ rxb, sizeof(rxb), 0, 0 };
static const sciBuf tx_prm={ txb, sizeof(txb), 0, 0 };

// �������p�����[�^
const sciPara sci_para = { (sciBuf *)&rx_prm, (sciBuf *)&tx_prm, 38400, 8, 1, sciPrNone, 7 };


/* ���[�N�G���A */
static sciBuf rec_buf;
static sciBuf send_buf;

static char sci0_rec_buf[3];
static char buf_counter=0;

void open_sci0();                    //  Open local comm. channel
extern int receiveData;
extern char Transbuf[2];

/************************************/
/*	SCI������						*/
/************************************/
void sci_init(void)
{
	/* 38400,8,1,N�ŏ����� */
	open_sci((sciPara *)&sci_para);
	open_sci0();                  
	
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
	rec_buf.wp    = rec_buf.rp  = 2;
	send_buf.buf  = para->send->buf;
	send_buf.size = para->send->size;
	send_buf.wp   = send_buf.rp = 2;

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
		wp = 2;
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
			rp = 2;
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
	char buf1[12]={0,};
	char buf2[12]={0,};         //data�i�[�p�z��
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
		wp = 2;
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


/*      Local Communication Setting   */


void open_sci0()
{
	

	// SCI�����~
	SCI0.SCSCR.BIT.TIE = 0;      // ���M������:����(1)/�֎~(0)
	SCI0.SCSCR.BIT.RIE = 0;      // ��M������:����(1)/�֎~(0)
	SCI0.SCSCR.BIT.TE  = 0;      // ���M����:����(1)/�֎~(0)
	SCI0.SCSCR.BIT.RE  = 0;      // ��M����:����(1)/�֎~(0)
	SCI0.SCSCR.BIT.MPIE = 0;     // MP�����݃C�l�[�u��
	SCI0.SCSCR.BIT.TEIE = 0;     // ���M�I��������:����(1)/�֎~(0)
	SCI0.SCSCR.BIT.CKE  = 0;     // �N���b�N�\�[�X�I��

    /* �����݃��x���̐ݒ� */
	INTC.IPRL.WORD &= ~SCI0_IPRMSK;
	INTC.IPRL.WORD |= (7 << SCI0_IPRSFT) & SCI0_IPRMSK;
	
    //INTC.IPRL.BIT._SCI0 = 0xF;    // ���荞�݃��x���ݒ�(SCI0)

	// SCI�ݒ�
	SCI0.SCSMR.BIT.CA   = 0;      // 0:���������A1:�N���b�N����
	SCI0.SCSMR.BIT.CHR  = 0;      // 0:8�r�b�g�f�[�^�A1:7�r�b�g�f�[�^
	SCI0.SCSMR.BIT._PE  = 0;      // 0:�p���e�B�֎~�A1:�p���e�B����
	SCI0.SCSMR.BIT.OE   = 0;      // 0:�����p���e�B�A1:��p���e�B
	SCI0.SCSMR.BIT.STOP = 0;      // 0:1�X�g�b�v�r�b�g�A1:2�X�g�b�v�r�b�g
	SCI0.SCSMR.BIT.MP   = 0;      // �}���`�v���Z�b�T���[�h 0:�֎~,1:����
	SCI0.SCSMR.BIT.CKS  = 0;      // �N���b�N�Z���N�g 00:1/1 , 01:1/4 , 10:1/16 , 11:1/64
	
	// �{�[���[�g�ݒ�
	SCI0.SCBRR= ((PCLK / 32) / 38400) - 1;       //38400 bps


	// SSR�����ݒ�
	SCI0.SCSSR.BIT.TDRE = 0;     // TDR�G���v�e�B
	SCI0.SCSSR.BIT.RDRF = 0;     // RDR�t��
	SCI0.SCSSR.BIT.ORER = 0;     // �I�[�o�[�����G���[
	SCI0.SCSSR.BIT.FER  = 0;     // �t���[�~���O�G���[
	SCI0.SCSSR.BIT.PER  = 0;     // �p���e�B�G���[

    
	SCI0.SCSCR.BIT.RIE = 1;      // ��M������:����(1)/�֎~(0)
	SCI0.SCSCR.BIT.TE  = 1;      // ���M����:����(1)/�֎~(0)
	SCI0.SCSCR.BIT.RE  = 1;      // ��M����:����(1)/�֎~(0)
		
	
}


/************************************/
/*	SCI0 �P���� Transmmit			*/
/************************************/
short sci0_putc(char data)
{
	unsigned long i=0;
    unsigned short flag = 0;
	
	for(i=0;i<0x00200000;i++){        //���M�o�t�@(TE)���J�������܂őҋ@
		if(SCI0.SCSSR.BIT.TDRE){      //TE���W�X�^���󂢂Ă��邩�`�F�b�N
			flag = 1;
			break;
		}
	}
	
	if(flag){	        
		SCI0.SCTDR = data;			    // 1�o�C�g���M 
		SCI0.SCSSR.BIT.TDRE=0;  	    // TDRE �N���A 
		return 1;
	}

	return 0;
}

#pragma interrupt(rxi0_int)
void rxi0_int(void){

	
	unsigned char c;
    int temp=0;
	
    SCI0.SCSSR.BIT.ORER = 0;     // �I�[�o�[�����G���[
	SCI0.SCSSR.BIT.FER  = 0;     // �t���[�~���O�G���[
	SCI0.SCSSR.BIT.PER  = 0;     // �p���e�B�G���[

	// ��M�o�b�t�@�Ƀf�[�^�i�[
	if(SCI0.SCSSR.BIT.RDRF){                      // ��M�t���O�`�F�b�N
		SCI0.SCSSR.BIT.RDRF = 0;                  // ��M�t���O�N���A
		c = SCI0.SCRDR;    			              // ��M�f�[�^�擾		

		if(buf_counter<3){                        // 2byte �擾 
		sci0_rec_buf[buf_counter]=c; 
		buf_counter++;
		}
		if(buf_counter==3){                       //int�^�ɕϊ� 
			buf_counter=0;
			temp = (int)(sci0_rec_buf[1]&0x00ff);
			temp|= (int)((sci0_rec_buf[2] << 8)&0xff00);
			if(temp > 32768) temp = temp-65536;   //�����t�ɕϊ�
			receiveData=(int)temp;                //�f�[�^�i�[
			
			//sci0_putc(Transbuf[0]);
        	//sci0_putc(Transbuf[1]);
	
		}
	}
	
}


#pragma interrupt (eri0_int)
void eri0_int(void)
{
	SCI0.SCSSR.BYTE &= ~0x78;		 //�G���[�t���O�N���A 
}



