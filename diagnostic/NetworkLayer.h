#ifndef _NETWORKLAYER_H
#define _NETWORKLAYER_H

#include "Diagnostic.h"
#include "MemManager.h"

typedef enum{
	N_OK,//sender and receiver
	N_TIMEOUT_A,//sender and receiver
	N_TIMEOUT_Bs,//sender only
	N_TIMEOUT_Cr,//receiver only
	N_WRONG_SN,//receiver only
	N_INVALID_FS,//sneder only
	N_UNEXP_PDU,//receiver only
	N_WFT_OVRN,//
	N_BUFFER_OVFLW,//sender only
	N_ERROR,//sender and receiver
}N_Result;

typedef enum{
	DIAGNOSTIC,
	REMOTE_DIAGNOSTICS,
}MType;

typedef enum{
	PHYSICAL,
	FUNCTIONAL,
}N_TAtype;

typedef enum{
	STmin,
	BS,
}Parameter;

typedef enum{
	N_CHANGE_OK,			//sender and receiver
	N_RX_ON,				//receiver only
	N_WRONG_PARAMETER,	//sender and receiver
	N_WRONG_VALUE,		//sender and receiver
}Result_ChangeParameter;

typedef enum{
	CTS,	 //continue to send
	WT,		//wait
	OVFLW,	//overflow
}FS_Type;

typedef enum
{
	NWL_IDLE,
	NWL_TRANSMITTING,
	NWL_RECIVING,
	NWL_WAIT,
}NWL_Status;

typedef enum{
	SF = 0,	//single frame
	FF = 1,	//first frame
	CF = 2,	//consecutive frame
	FC = 3,	//flow control
}N_PCIType;

typedef enum{
	TX_IDLE,
	TX_WAIT_FF_CONF,
	TX_WAIT_FC,
	TX_WAIT_CF_REQ,
	TX_WAIT_CF_CONF,
}TransmissionStep;

typedef enum{
	RX_IDLE,
	RX_WAIT_FC_REQ,
	RX_WAIT_FC_CONF,
	RX_WAIT_CF,
}RecivingStep;

typedef enum{
	HALF_DUPLEX,
	FULL_DUPLEX,
}DuplexMode;

typedef struct{
	uint16_t N_As;
	uint16_t N_Ar;
	uint16_t N_Bs;
	uint16_t N_Br;
	uint16_t N_Cs;
	uint16_t N_Cr;
	bool FF_ConfirmedOnSender;		//when transmission
	bool FC_RecivedOnSender;		//when transmission
	bool CF_RequestedOnSender;	//when transmission
	bool CF_ConfirmedOnSender;		//when transmission
	bool FC_RxBeforeFFOnSender;	//when transmission
	bool FC_RequestedOnReciver;	//when reciving
	bool FC_ConfirmedOnReciver;	//when reciving
	bool CF_RecivedOnReciver;		//when reciving
}TimePeriodParam;

typedef struct{
	MType Mtype;
	uint8_t N_SA;
	uint8_t N_TA;
	N_TAtype N_TAtype;
	uint8_t N_AE;
}AddressFormat;

typedef union{
	struct{
		MType Mtype;
		N_TAtype N_TAtype;
		uint8_t N_SA;
		uint8_t N_TA;
		uint8_t N_AE;
		uint32_t ID;
		uint8_t DLC;
		uint8_t RTR;
		uint8_t IDE;
		bool valid;
		uint8_t data7;
		uint8_t data6;
		uint8_t data5;
		uint8_t data4;
		uint8_t data3;
		uint8_t STmin;		//STmin ,data2
		uint8_t FF_DL_LOW;	//BS ,FF_DL_LOW ,data1
		uint8_t SF_DL:4;		//SF_DL ,FF_DL_HIGH ,TxParam.SN ,FS
		N_PCIType N_PciType:4;
	}N_PDU;
	
	struct{
		MType Mtype;
		N_TAtype N_TAtype;
		uint8_t N_SA;/* ����ID�Ÿ�λ */
		uint8_t N_TA;/* ����ID�ŵ�λ */
		uint8_t N_AE;
		uint32_t ID;
		uint8_t DLC;
		uint8_t RTR;
		uint8_t IDE;
		bool valid;
		uint8_t data7;
		uint8_t data6;
		uint8_t data5;
		uint8_t data4;
		uint8_t data3;
		uint8_t data2;
		uint8_t data1;
		uint8_t data0;
	}CanData;
}NetworkFrame;

typedef struct{
	FS_Type FS_Type;
	uint8_t BlockSize;
	uint8_t CompletedNumberInBlock;
	uint8_t STmin;
	uint8_t SN;
	uint16_t TotalDataNumber;
	uint16_t CompletedDataNumber;
	uint16_t BuffSize;
}CommuParam;

typedef enum{
	CONFIRM,
	FF_INDICATION,
	INDICATION,
}NetWorkNotificationType;

typedef struct{
	NetWorkNotificationType NotificationType;
	MType Mtype;
	uint8_t N_SA;
	uint8_t N_TA;
	N_TAtype N_TAtype;
	uint8_t N_AE;
	uint8_t *MessageData;
	uint16_t length;
	N_Result N_Resut;
	bool valid;
}NetworkNotification;

#define MAX_BUFF_NUMBER			3
#define MAX_INDICATION_NUMBER		3
#define MAX_REQUEST_SIZE			1026

#define CAN_ID_DIAGNOSIS_FUNCTION 		0x7DF

void NetworkLayer_InitParam(uint32_t PyhReqID,uint32_t FunReqID, uint32_t ResponseID,SendCANFun sendFun);
void NetworkLayer_Proc(void);
void N_USData_request(MType Mtype, uint8_t N_SA, uint8_t N_TA, N_TAtype N_TAtype, uint8_t N_AE, uint8_t* MessageData, uint16_t Length);

void N_ChangeParameter_request(MType Mtype, uint8_t N_SA, uint8_t N_TA, N_TAtype N_TAtype, uint8_t N_AE, Parameter Parameter, uint8_t Parameter_Value);
void N_ChangeParameter_confirm(MType Mtype, uint8_t N_SA, uint8_t N_TA, N_TAtype N_TAtype, uint8_t N_AE, Parameter Parameter, Result_ChangeParameter Result_ChangeParameter);
void NetworkLayer_RxFrame(uint32_t ID,uint8_t* data,uint8_t IDE,uint8_t DLC,uint8_t RTR);
void NetworkLayer_SetParam(uint8_t TimeAs, uint8_t TimeBs, uint8_t TimeCr, uint8_t TimeAr, uint8_t TimeBr, uint8_t TimeCs, 	uint8_t Bs, uint8_t m_STmin, DuplexMode nDuplex ,  MType Mtype , uint8_t N_SA , uint8_t N_TA , N_TAtype N_TAtype , uint8_t N_AE , uint8_t FillData);
bool IsIndicationListEmpty(void);
NetworkNotification PullIndication(void);
#endif