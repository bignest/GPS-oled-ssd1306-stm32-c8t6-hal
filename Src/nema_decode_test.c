/**
  ******************************************************************************
  * �ļ�����: nmea_decode_test.c 
  * ��    ��: ӲʯǶ��ʽ�����Ŷ�
  * ��    ��: V1.0
  * ��д����: 2015-10-04
  * ��    ��: GPSģ�����
  ******************************************************************************
  * ˵����
  * ����������ӲʯYS-F1Mini��ʹ�á�
  * 
  * �Ա���
  * ��̳��http://www.ing10bbs.com
  * ��Ȩ��ӲʯǶ��ʽ�����Ŷ����У��������á�
  ******************************************************************************
  */
/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include "stm32f1xx_hal.h"
#include "usart/bsp_debug_usart.h"
#include "nmea/nmea.h"
#include "gps/bsp_gps_usartx.h"
#include "fonts.h"
#include "ssd1306.h"

/* ˽�����Ͷ��� --------------------------------------------------------------*/
/* ˽�к궨�� ----------------------------------------------------------------*/
/* ˽�б��� ------------------------------------------------------------------*/
uint8_t gps_rbuff[GPS_RBUFF_SIZE];
__IO uint8_t GPS_TransferEnd = 0, GPS_HalfTransferEnd = 0;


/* ��չ���� ------------------------------------------------------------------*/
/* ˽�к���ԭ�� --------------------------------------------------------------*/
/* ������ --------------------------------------------------------------------*/
/**
  * ��������: trace �ڽ���ʱ��������GPS���
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ����str: Ҫ������ַ�����str_size:���ݳ���
  */
void trace(const char *str, int str_size)
{
#ifdef __GPS_DEBUG    //��������꣬�Ƿ����������Ϣ
    uint16_t i;
    printf("\r\nTrace: ");
    for(i=0;i<str_size;i++)
      printf("%c",*(str+i));
  
    printf("\n");
#endif
}

/**
  * ��������: error �ڽ������ʱ�����ʾ��Ϣ
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ����str: Ҫ������ַ�����str_size:���ݳ���
  */
void error(const char *str, int str_size)
{
#ifdef __GPS_DEBUG   //��������꣬�Ƿ����������Ϣ

    uint16_t i;
    printf("\r\nError: ");
    for(i=0;i<str_size;i++)
      printf("%c",*(str+i));
    printf("\n");
#endif
}

/**
  * ��������: �ж�����(�������2000�Ժ�����) 
  * �������: iYear    ��λ����
  * �� �� ֵ: uint8_t        1:Ϊ����    0:Ϊƽ�� 
  * ˵    ������
  */
static uint8_t IsLeapYear(uint8_t iYear) 
{ 
    uint16_t    Year; 
    Year    =    2000+iYear; 
    if((Year&3)==0) 
    { 
        return ((Year%400==0) || (Year%100!=0)); 
    } 
     return 0; 
} 

/**
  * ��������: ��������ʱ�任�������ʱ��ʱ�� 
  * �������: *DT:��ʾ����ʱ������� ��ʽ YY,MM,DD,HH,MM,SS
  * �� �� ֵ: ��
  * ˵    ����AREA:1(+)���� W0(-)����   GMT:ʱ���� 
  */
void    GMTconvert(nmeaTIME *SourceTime, nmeaTIME *ConvertTime, uint8_t GMT,uint8_t AREA) 
{ 
    uint32_t    YY,MM,DD,hh,mm,ss;        //������ʱ�����ݴ���� 
     
    if(GMT==0)    return;                //�������0ʱ��ֱ�ӷ��� 
    if(GMT>12)    return;                //ʱ�����Ϊ12 �����򷵻�         

    YY    =    SourceTime->year;                //��ȡ�� 
    MM    =    SourceTime->mon;                 //��ȡ�� 
    DD    =    SourceTime->day;                 //��ȡ�� 
    hh    =    SourceTime->hour;                //��ȡʱ 
    mm    =    SourceTime->min;                 //��ȡ�� 
    ss    =    SourceTime->sec;                 //��ȡ�� 

    if(AREA)                        //��(+)ʱ������ 
    { 
        if(hh+GMT<24)    hh    +=    GMT;//������������ʱ�䴦��ͬһ�������Сʱ���� 
        else                        //����Ѿ����ڸ�������ʱ��1����������ڴ��� 
        { 
            hh    =    hh+GMT-24;        //�ȵó�ʱ�� 
            if(MM==1 || MM==3 || MM==5 || MM==7 || MM==8 || MM==10)    //���·�(12�µ�������) 
            { 
                if(DD<31)    DD++; 
                else 
                { 
                    DD    =    1; 
                    MM    ++; 
                } 
            } 
            else if(MM==4 || MM==6 || MM==9 || MM==11)                //С�·�2�µ�������) 
            { 
                if(DD<30)    DD++; 
                else 
                { 
                    DD    =    1; 
                    MM    ++; 
                } 
            } 
            else if(MM==2)    //����2�·� 
            { 
                if((DD==29) || (DD==28 && IsLeapYear(YY)==0))        //��������������2��29�� ���߲�����������2��28�� 
                { 
                    DD    =    1; 
                    MM    ++; 
                } 
                else    DD++; 
            } 
            else if(MM==12)    //����12�·� 
            { 
                if(DD<31)    DD++; 
                else        //�������һ�� 
                {               
                    DD    =    1; 
                    MM    =    1; 
                    YY    ++; 
                } 
            } 
        } 
    } 
    else 
    {     
        if(hh>=GMT)    hh    -=    GMT;    //������������ʱ�䴦��ͬһ�������Сʱ���� 
        else                        //����Ѿ����ڸ�������ʱ��1����������ڴ��� 
        { 
            hh    =    hh+24-GMT;        //�ȵó�ʱ�� 
            if(MM==2 || MM==4 || MM==6 || MM==8 || MM==9 || MM==11)    //�����Ǵ��·�(1�µ�������) 
            { 
                if(DD>1)    DD--; 
                else 
                { 
                    DD    =    31; 
                    MM    --; 
                } 
            } 
            else if(MM==5 || MM==7 || MM==10 || MM==12)                //������С�·�2�µ�������) 
            { 
                if(DD>1)    DD--; 
                else 
                { 
                    DD    =    30; 
                    MM    --; 
                } 
            } 
            else if(MM==3)    //�����ϸ�����2�·� 
            { 
                if((DD==1) && IsLeapYear(YY)==0)                    //�������� 
                { 
                    DD    =    28; 
                    MM    --; 
                } 
                else    DD--; 
            } 
            else if(MM==1)    //����1�·� 
            { 
                if(DD>1)    DD--; 
                else        //�����һ�� 
                {               
                    DD    =    31; 
                    MM    =    12; 
                    YY    --; 
                } 
            } 
        } 
    }         

    ConvertTime->year   =    YY;                //������ 
    ConvertTime->mon    =    MM;                //������ 
    ConvertTime->day    =    DD;                //������ 
    ConvertTime->hour   =    hh;                //����ʱ 
    ConvertTime->min    =    mm;                //���·� 
    ConvertTime->sec    =    ss;                //������ 
}  

void HAL_UART_RxHalfCpltCallback(UART_HandleTypeDef *huart)
{
  GPS_HalfTransferEnd=1;

}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  GPS_TransferEnd=1;
}

/**
  * ��������: ����GPSģ����Ϣ
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  */
int nmea_decode_test(void)
{
  nmeaINFO info;          //GPS�����õ�����Ϣ
  nmeaPARSER parser;      //����ʱʹ�õ����ݽṹ  
  uint8_t new_parse=0;    //�Ƿ����µĽ������ݱ�־

  nmeaTIME beiJingTime;    //����ʱ�� 

  /* �����������������Ϣ�ĺ��� */
  nmea_property()->trace_func = &trace;
  nmea_property()->error_func = &error;

  /* ��ʼ��GPS���ݽṹ */
  nmea_zero_INFO(&info);
  nmea_parser_init(&parser);
  
  /* ʹ��DMA�������ݵ����Զ� */
  HAL_UART_Receive_DMA(&husartx,gps_rbuff,GPS_RBUFF_SIZE);  
  
//  while(1)
//  {
//    if(GPS_HalfTransferEnd)     /* ���յ�GPS_RBUFF_SIZEһ������� */
//    {
//      /* ����nmea��ʽ���� */
//      nmea_parse(&parser, (const char*)&gps_rbuff[0], HALF_GPS_RBUFF_SIZE, &info);
//      GPS_HalfTransferEnd = 0;   //��ձ�־λ
//      new_parse = 1;             //���ý�����Ϣ��־ 
//    }
//    else if(GPS_TransferEnd)    /* ���յ���һ������ */
//    {          
//      nmea_parse(&parser, (const char*)&gps_rbuff[HALF_GPS_RBUFF_SIZE], HALF_GPS_RBUFF_SIZE, &info);
//      GPS_TransferEnd = 0;
//      new_parse =1;
//    }
//    if(new_parse )                //���µĽ�����Ϣ   
//    {             
//      /* �Խ�����ʱ�����ת����ת���ɱ���ʱ�� */
//      GMTconvert(&info.utc,&beiJingTime,8,1);      
//      /* �������õ�����Ϣ */
//      printf("\nʱ��%d,%d,%d,%d,%d,%d\n", beiJingTime.year+1900, beiJingTime.mon+1,beiJingTime.day,beiJingTime.hour,beiJingTime.min,beiJingTime.sec);
//      printf("γ�ȣ�%f,����%f\n",info.lat/100,info.lon/100);
//      printf("����ʹ�õ����ǣ�%d,�ɼ����ǣ�%d\n",info.satinfo.inuse,info.satinfo.inview);
//      printf("���θ߶ȣ�%f �� \n", info.elv);
//      printf("�ٶȣ�%f km/h \n", info.speed);
//      printf("����%f ��\n", info.direction);      
//			
//      new_parse = 0;
//    }
//  }
  while(1)
  {
    if(GPS_HalfTransferEnd)     /* ���յ�GPS_RBUFF_SIZEһ������� */
    {
      /* ����nmea��ʽ���� */
      nmea_parse(&parser, (const char*)&gps_rbuff[0], HALF_GPS_RBUFF_SIZE, &info);
      GPS_HalfTransferEnd = 0;   //��ձ�־λ
      new_parse = 1;             //���ý�����Ϣ��־ 
    }
    else if(GPS_TransferEnd)    /* ���յ���һ������ */
    {          
      nmea_parse(&parser, (const char*)&gps_rbuff[HALF_GPS_RBUFF_SIZE], HALF_GPS_RBUFF_SIZE, &info);
      GPS_TransferEnd = 0;
      new_parse =1;
    }
    if(new_parse )                //���µĽ�����Ϣ   
    {             
      /* �Խ�����ʱ�����ת����ת���ɱ���ʱ�� */
      GMTconvert(&info.utc,&beiJingTime,8,1);      
      /* �������õ�����Ϣ */
			
 //printf("\nʱ��%d,%d,%d,%d,%d,%d\n", beiJingTime.year+1900, beiJingTime.mon+1,beiJingTime.day,beiJingTime.hour,beiJingTime.min,beiJingTime.sec);
//      printf("γ�ȣ�%f,����%f\n",info.lat/100,info.lon/100);
	//printf("����ʹ�õ����ǣ�%d,�ɼ����ǣ�%d\n",info.satinfo.inuse,info.satinfo.inview);
//      printf("���θ߶ȣ�%f �� \n", info.elv);
     // printf("�ٶȣ�%f km/h \n", info.satinfo.inview);
		
char str[2]; 
int id=beiJingTime.sec;
SSD1306_Fill(0);
sprintf(str,"%d",id); 
SSD1306_GotoXY(10,52);
SSD1306_Puts(str, &Font_7x10, 1);
SSD1306_UpdateScreen(); //display
//      printf("����%f ��\n", info.direction);      
			
      new_parse = 0;
    }
  }
}



/******************* (C) COPYRIGHT 2015-2020 ӲʯǶ��ʽ�����Ŷ� *****END OF FILE****/
