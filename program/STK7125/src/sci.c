/****************************************************************************/
/*																			*/
/*		STK-7125 サンプルプログラム											*/
/*			シリアル処理													*/
/*																			*/
/*			Copyright   :: ㈱アルファプロジェクト							*/
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
/* 送受信バッファ */
static char rxb[bs];
static char txb[bs];


// 送受信バッファパラメータ
static const sciBuf rx_prm={ rxb, sizeof(rxb), 0, 0 };
static const sciBuf tx_prm={ txb, sizeof(txb), 0, 0 };

// 初期化パラメータ
const sciPara sci_para = { (sciBuf *)&rx_prm, (sciBuf *)&tx_prm, 38400, 8, 1, sciPrNone, 7 };


/* ワークエリア */
static sciBuf rec_buf;
static sciBuf send_buf;

static char sci0_rec_buf[3];
static char buf_counter=0;

void open_sci0();                    //  Open local comm. channel
extern int receiveData;
extern char Transbuf[2];

/************************************/
/*	SCI初期化						*/
/************************************/
void sci_init(void)
{
	/* 38400,8,1,Nで初期化 */
	open_sci((sciPara *)&sci_para);
	open_sci0();                  
	
}

/************************************/
/*	SCIオープン						*/
/************************************/
void open_sci(sciPara * para)
{
	unsigned char tmp = 0;

	/* ワークエリア初期化 */
	rec_buf.buf   = para->rec->buf;
	rec_buf.size  = para->rec->size;
	rec_buf.wp    = rec_buf.rp  = 2;
	send_buf.buf  = para->send->buf;
	send_buf.size = para->send->size;
	send_buf.wp   = send_buf.rp = 2;

	/* 送受信割込禁止、送受信禁止 */
	SCI.SCSCR.BYTE = 0x00;	

	/* 割込みレベルの設定 */
	INTC.IPRL.WORD &= ~SCI_IPRMSK;
	INTC.IPRL.WORD |= (para->int_lv << SCI_IPRSFT) & SCI_IPRMSK;

	/* SMR設定 */
	// ビット長
	if (para->bit_len == 7) {
		tmp |= 0x40;			// 7bit
	}
	else {
		tmp |= 0x00;			// 8bit
	}
	// パリティ
	switch(para->parity) {
		case sciPrEven:			// 偶数パリティ
			tmp |= 0x20;
			break;
		case sciPrOdd:			// 奇数パリティ
			tmp |= 0x30;
			break;
		default:				// パリティ無し
			tmp |= 0x00;
			break;
	}
	// ストップビット
	if (para->stop_bit == 1) {
		tmp |= 0x00;			// STOP 1
	}
	else {
		tmp |= 0x08;			// STOP 2
	}
	SCI.SCSMR.BYTE = tmp;

	/* BBR設定 */
	tmp = ((PCLK / 32) / para->bps) - 1;
	SCI.SCBRR = tmp;

	/* SSR設定 */
	SCI.SCSSR.BYTE &= ~0x78;		/* ステータス(受信,エラー)クリア */
	SCI.SCSCR.BYTE |= 0x70;			/* 受信割込許可、送受信許可 */
}


/************************************/
/*	SCIクローズ						*/
/************************************/
void close_sci(void)
{
	SCI.SCSCR.BYTE = 0x00;			/* 送受信割込み禁止、送受信禁止 */
	INTC.IPRL.WORD &= ~SCI_IPRMSK;	/* 割り込みレベルクリア */
	
}


/************************************/
/*	１文字格納						*/
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
		SCI.SCSCR.BYTE |= 0x80;		/* 送信割込み許可 */
		ans = 0;
	}
	else {
		ans = -1;
	}

	return ans;
}



/************************************/
/*	１文字取得						*/
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
/*	シリアル文字列出力関数			*/
/************************************/
void sputs(char *p)
{
	while(1) {
		if(*p==0x00) break;			// Null文字まで出力
		while(sci_putc(*p)) ;		// 送信バッファが一杯なら再送
		++p;
	}
}



/* 10進数出力*/
void printDecimal(int d)
{
	char buf1[12]={0,};
	char buf2[12]={0,};         //data格納用配列
	int i=0,j=0;
	int sign=0;                     //符号判定用

	if(d < 0){                      // data < 0の場合、正に変換後、sign=1;
		d = -d ;
		sign=1;
	}	
	if(d == 0){                     // data =0の場合、buf1の0番目に文字'0'を格納
		buf1[0]='0';
		i++;
	}
	while(d>0){                     // buf1にdataを10で割った余りを'0'と一緒に格納
	 buf1[i] = '0'+d%10;            // dataは10で割って再格納(余りは無くなる)  
	 d /= 10;                     
	 i++;	
	}	
	if(sign){buf1[i]='-';i++;}           // sign=1なら、最後のbuf1に'-'を格納

	for(j=(i-1);j>=0;j--){              // buf1を逆から順番に1文字出力
		
	  sci_putc(buf1[j]);
		
	}		
}





/************************************/
/*	受信エラー割り込み				*/
/************************************/
#pragma interrupt (eri_int)
void eri_int(void)
{
	SCI.SCSSR.BYTE &= ~0x78;		/* エラーフラグクリア */
}


/************************************/
/*	受信割り込み					*/
/************************************/
#pragma interrupt (rxi_int)
void rxi_int(void)
{
	unsigned long wp;
	unsigned char data;

	data = SCI.SCRDR;				/* 受信 */
	SCI.SCSSR.BYTE &= ~0x40;		/* RDRFクリア */
	
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
/*	送信割り込み					*/
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

		SCI.SCTDR = data;			/* 1バイト送信 */
		SCI.SCSSR.BYTE &= ~0x80;	/* TDRE クリア */
	}
	else {
		SCI.SCSCR.BYTE &= ~0x80;	/* 送信割込み禁止 */
	}
}


/************************************/
/*	送信終了割込み					*/
/************************************/
#pragma interrupt (tei_int)
void tei_int(void)
{
	SCI.SCSCR.BYTE &= ~0x04;		/* 送信終了割込み禁止 */
}


/*      Local Communication Setting   */


void open_sci0()
{
	

	// SCI動作停止
	SCI0.SCSCR.BIT.TIE = 0;      // 送信割込み:許可(1)/禁止(0)
	SCI0.SCSCR.BIT.RIE = 0;      // 受信割込み:許可(1)/禁止(0)
	SCI0.SCSCR.BIT.TE  = 0;      // 送信動作:許可(1)/禁止(0)
	SCI0.SCSCR.BIT.RE  = 0;      // 受信動作:許可(1)/禁止(0)
	SCI0.SCSCR.BIT.MPIE = 0;     // MP割込みイネーブル
	SCI0.SCSCR.BIT.TEIE = 0;     // 送信終了割込み:許可(1)/禁止(0)
	SCI0.SCSCR.BIT.CKE  = 0;     // クロックソース選択

    /* 割込みレベルの設定 */
	INTC.IPRL.WORD &= ~SCI0_IPRMSK;
	INTC.IPRL.WORD |= (7 << SCI0_IPRSFT) & SCI0_IPRMSK;
	
    //INTC.IPRL.BIT._SCI0 = 0xF;    // 割り込みレベル設定(SCI0)

	// SCI設定
	SCI0.SCSMR.BIT.CA   = 0;      // 0:調歩同期、1:クロック同期
	SCI0.SCSMR.BIT.CHR  = 0;      // 0:8ビットデータ、1:7ビットデータ
	SCI0.SCSMR.BIT._PE  = 0;      // 0:パリティ禁止、1:パリティ許可
	SCI0.SCSMR.BIT.OE   = 0;      // 0:偶数パリティ、1:奇数パリティ
	SCI0.SCSMR.BIT.STOP = 0;      // 0:1ストップビット、1:2ストップビット
	SCI0.SCSMR.BIT.MP   = 0;      // マルチプロセッサモード 0:禁止,1:許可
	SCI0.SCSMR.BIT.CKS  = 0;      // クロックセレクト 00:1/1 , 01:1/4 , 10:1/16 , 11:1/64
	
	// ボーレート設定
	SCI0.SCBRR= ((PCLK / 32) / 38400) - 1;       //38400 bps


	// SSR初期設定
	SCI0.SCSSR.BIT.TDRE = 0;     // TDRエンプティ
	SCI0.SCSSR.BIT.RDRF = 0;     // RDRフル
	SCI0.SCSSR.BIT.ORER = 0;     // オーバーランエラー
	SCI0.SCSSR.BIT.FER  = 0;     // フレーミングエラー
	SCI0.SCSSR.BIT.PER  = 0;     // パリティエラー

    
	SCI0.SCSCR.BIT.RIE = 1;      // 受信割込み:許可(1)/禁止(0)
	SCI0.SCSCR.BIT.TE  = 1;      // 送信動作:許可(1)/禁止(0)
	SCI0.SCSCR.BIT.RE  = 1;      // 受信動作:許可(1)/禁止(0)
		
	
}


/************************************/
/*	SCI0 １文字 Transmmit			*/
/************************************/
short sci0_putc(char data)
{
	unsigned long i=0;
    unsigned short flag = 0;
	
	for(i=0;i<0x00200000;i++){        //送信バファ(TE)が開放されるまで待機
		if(SCI0.SCSSR.BIT.TDRE){      //TEレジスタが空いているかチェック
			flag = 1;
			break;
		}
	}
	
	if(flag){	        
		SCI0.SCTDR = data;			    // 1バイト送信 
		SCI0.SCSSR.BIT.TDRE=0;  	    // TDRE クリア 
		return 1;
	}

	return 0;
}

#pragma interrupt(rxi0_int)
void rxi0_int(void){

	
	unsigned char c;
    int temp=0;
	
    SCI0.SCSSR.BIT.ORER = 0;     // オーバーランエラー
	SCI0.SCSSR.BIT.FER  = 0;     // フレーミングエラー
	SCI0.SCSSR.BIT.PER  = 0;     // パリティエラー

	// 受信バッファにデータ格納
	if(SCI0.SCSSR.BIT.RDRF){                      // 受信フラグチェック
		SCI0.SCSSR.BIT.RDRF = 0;                  // 受信フラグクリア
		c = SCI0.SCRDR;    			              // 受信データ取得		

		if(buf_counter<3){                        // 2byte 取得 
		sci0_rec_buf[buf_counter]=c; 
		buf_counter++;
		}
		if(buf_counter==3){                       //int型に変換 
			buf_counter=0;
			temp = (int)(sci0_rec_buf[1]&0x00ff);
			temp|= (int)((sci0_rec_buf[2] << 8)&0xff00);
			if(temp > 32768) temp = temp-65536;   //符号付に変換
			receiveData=(int)temp;                //データ格納
			
			//sci0_putc(Transbuf[0]);
        	//sci0_putc(Transbuf[1]);
	
		}
	}
	
}


#pragma interrupt (eri0_int)
void eri0_int(void)
{
	SCI0.SCSSR.BYTE &= ~0x78;		 //エラーフラグクリア 
}



