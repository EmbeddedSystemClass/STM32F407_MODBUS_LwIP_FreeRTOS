#ifndef __DW_MAIN_H
#define __DW_MAIN_H
#include "deca_device_api.h"
#include "deca_regs.h"
#include "deca_sleep.h"
#include "port.h"
/* Example application name and version to display on LCD screen. ��ҵҺ��������ʾ��ʾ��Ӧ�ó������ƺͰ汾*/
#define APP_NAME "DS TWR INIT v1.1"

/* Inter-ranging delay period, in milliseconds. �����ʱ�䣬�Ժ���Ϊ��λ��*/
#define RNG_DELAY_MS 5
#define firmware_version 10 //�̼��汾�� 
/* Default communication configuration. We use here EVK1000's default mode (mode 3).Ĭ��ͨ�����á�����������ʹ��EVK1000��ģʽ3��*/
static dwt_config_t config = {
    2,               /* Channel number.ͨ���š�*/
    DWT_PRF_64M,     /* Pulse repetition frequency.�����ظ�Ƶ��*/
    DWT_PLEN_1024,   /* Preamble length. ǰ�����ȡ� */
    DWT_PAC32,       /* Preamble acquisition chunk size. Used in RX only. ǰ����ȡ���С��������RX�� */
    9,               /* TX preamble code. Used in TX only. TXǰ���롣ֻ��TXʹ�á� */
    9,               /* RX preamble code. Used in RX only. RXǰ���롣������RX��*/
    1,               /* Use non-standard SFD (Boolean)  ʹ�÷Ǳ�׼��SFD��������*/
    DWT_BR_110K,     /* Data rate. �������ʡ� */
    DWT_PHRMODE_STD, /* PHY header mode. PHYͷģʽ�� */
    (1025 + 64 - 32) /* SFD timeout (preamble length + 1 + SFD length - PAC size). Used in RX only.
												SFD��ʱ��ǰ������+ 1 +SFD����-PAC��С����������RX��*/
};
/* Default antenna delay values for 64 MHz PRF. See NOTE 1 below. 64 MHz PRF��Ĭ�������ӳ�ֵ���������ע��1��*/
#define TX_ANT_DLY 0
#define RX_ANT_DLY 32899
#define FINAL_MSG_TS_LEN 4  //ʱ�����ݳ���
extern uint32 MODBUS_BaudRate[10];
extern int16_t SYS_dis_buf_t[7]; //����վ��ȡ���ľ������ݻ�����

extern uint16 Time_time4_Cuo;  					//��¼ʱ���-������
extern uint16 Time_time4_Cuo_buf[10]; 	//��¼ʱ���-������
extern uint16 ERROR_FLAG;  //��������������־λ���ﵽһ����������

extern  uint16   Flash_Usart_BaudRate;   //�豸����ͨѶ������ 0��4800  1��9600 2��14400 3��19200 4��38400 5��56000 6��57600 7��115200  8��128000 9��256000
extern  uint16   Flash_Modbus_ADDR;        //Modbus ID�� 
extern  uint16   Flash_structure_Mode;     //0:Ϊ��άƽ��3��վģʽ
extern  uint16   Flash_Device_Mode;       //�豸ģʽ 0����ǩ 1���λ�վ 2������վ
extern  uint16   Flash_Device_ID;         //��8λΪ�λ�վID����Χ0~6  ��8λΪ��ǩID 0~247    �������ڲ� ��ǩIDΪ0~247  �λ�վIDΪ248~245  ����վIDΪ255��
extern  uint16   Flash_MAJOR_BS_X_Y_Z[3];   //6�λ�վ��λ�ã�      X,Y,Z�����ֽ�
extern  uint16   Flash_BS_EN_X_Y_Z[6][4];   //6�λ�վ��λ�ã�    ʹ�ܱ�־��0Ϊ��  1Ϊ��   X,Y,Z�����ֽ�
extern  uint16   Flash_FLAG;	 //��־λ


extern  uint16   Calculate_EN;       //����ʹ��
extern  uint16   Calculate_TAG_ID;		//������ǩID
extern  uint16		Calculate_FLAG;			//����״̬��־
extern  uint16		Calculate_TAG_X;		//������ǩ��X��
extern  uint16		Calculate_TAG_Y;		//������ǩ��Y��
extern  uint16		Calculate_TAG_Z;     //������ǩ��Z��
extern  uint16		Calculate_Station_TAG[7]; //������ǩ��Z��


void GPIO_Toggle(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
int dw_main(void);
void dw_init(void);
#endif


