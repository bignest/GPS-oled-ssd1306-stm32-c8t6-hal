/**
  ******************************************************************************
  * 文件名程: nmea_decode_test.c 
  * 作    者: 硬石嵌入式开发团队
  * 版    本: V1.0
  * 编写日期: 2015-10-04
  * 功    能: GPS模块解码
  ******************************************************************************
  * 说明：
  * 本例程配套硬石YS-F1Mini板使用。
  * 
  * 淘宝：
  * 论坛：http://www.ing10bbs.com
  * 版权归硬石嵌入式开发团队所有，请勿商用。
  ******************************************************************************
  */
/* 包含头文件 ----------------------------------------------------------------*/
#include "stm32f1xx_hal.h"
#include "usart/bsp_debug_usart.h"
#include "nmea/nmea.h"
#include "gps/bsp_gps_usartx.h"
#include "fonts.h"
#include "ssd1306.h"

/* 私有类型定义 --------------------------------------------------------------*/
/* 私有宏定义 ----------------------------------------------------------------*/
/* 私有变量 ------------------------------------------------------------------*/
uint8_t gps_rbuff[GPS_RBUFF_SIZE];
__IO uint8_t GPS_TransferEnd = 0, GPS_HalfTransferEnd = 0;


/* 扩展变量 ------------------------------------------------------------------*/
/* 私有函数原形 --------------------------------------------------------------*/
/* 函数体 --------------------------------------------------------------------*/
/**
  * 函数功能: trace 在解码时输出捕获的GPS语句
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：str: 要输出的字符串，str_size:数据长度
  */
void trace(const char *str, int str_size)
{
#ifdef __GPS_DEBUG    //配置这个宏，是否输出调试信息
    uint16_t i;
    printf("\r\nTrace: ");
    for(i=0;i<str_size;i++)
      printf("%c",*(str+i));
  
    printf("\n");
#endif
}

/**
  * 函数功能: error 在解码出错时输出提示消息
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：str: 要输出的字符串，str_size:数据长度
  */
void error(const char *str, int str_size)
{
#ifdef __GPS_DEBUG   //配置这个宏，是否输出调试信息

    uint16_t i;
    printf("\r\nError: ");
    for(i=0;i<str_size;i++)
      printf("%c",*(str+i));
    printf("\n");
#endif
}

/**
  * 函数功能: 判断闰年(仅针对于2000以后的年份) 
  * 输入参数: iYear    两位年数
  * 返 回 值: uint8_t        1:为闰年    0:为平年 
  * 说    明：无
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
  * 函数功能: 格林尼治时间换算世界各时区时间 
  * 输入参数: *DT:表示日期时间的数组 格式 YY,MM,DD,HH,MM,SS
  * 返 回 值: 无
  * 说    明：AREA:1(+)东区 W0(-)西区   GMT:时区数 
  */
void    GMTconvert(nmeaTIME *SourceTime, nmeaTIME *ConvertTime, uint8_t GMT,uint8_t AREA) 
{ 
    uint32_t    YY,MM,DD,hh,mm,ss;        //年月日时分秒暂存变量 
     
    if(GMT==0)    return;                //如果处于0时区直接返回 
    if(GMT>12)    return;                //时区最大为12 超过则返回         

    YY    =    SourceTime->year;                //获取年 
    MM    =    SourceTime->mon;                 //获取月 
    DD    =    SourceTime->day;                 //获取日 
    hh    =    SourceTime->hour;                //获取时 
    mm    =    SourceTime->min;                 //获取分 
    ss    =    SourceTime->sec;                 //获取秒 

    if(AREA)                        //东(+)时区处理 
    { 
        if(hh+GMT<24)    hh    +=    GMT;//如果与格林尼治时间处于同一天则仅加小时即可 
        else                        //如果已经晚于格林尼治时间1天则进行日期处理 
        { 
            hh    =    hh+GMT-24;        //先得出时间 
            if(MM==1 || MM==3 || MM==5 || MM==7 || MM==8 || MM==10)    //大月份(12月单独处理) 
            { 
                if(DD<31)    DD++; 
                else 
                { 
                    DD    =    1; 
                    MM    ++; 
                } 
            } 
            else if(MM==4 || MM==6 || MM==9 || MM==11)                //小月份2月单独处理) 
            { 
                if(DD<30)    DD++; 
                else 
                { 
                    DD    =    1; 
                    MM    ++; 
                } 
            } 
            else if(MM==2)    //处理2月份 
            { 
                if((DD==29) || (DD==28 && IsLeapYear(YY)==0))        //本来是闰年且是2月29日 或者不是闰年且是2月28日 
                { 
                    DD    =    1; 
                    MM    ++; 
                } 
                else    DD++; 
            } 
            else if(MM==12)    //处理12月份 
            { 
                if(DD<31)    DD++; 
                else        //跨年最后一天 
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
        if(hh>=GMT)    hh    -=    GMT;    //如果与格林尼治时间处于同一天则仅减小时即可 
        else                        //如果已经早于格林尼治时间1天则进行日期处理 
        { 
            hh    =    hh+24-GMT;        //先得出时间 
            if(MM==2 || MM==4 || MM==6 || MM==8 || MM==9 || MM==11)    //上月是大月份(1月单独处理) 
            { 
                if(DD>1)    DD--; 
                else 
                { 
                    DD    =    31; 
                    MM    --; 
                } 
            } 
            else if(MM==5 || MM==7 || MM==10 || MM==12)                //上月是小月份2月单独处理) 
            { 
                if(DD>1)    DD--; 
                else 
                { 
                    DD    =    30; 
                    MM    --; 
                } 
            } 
            else if(MM==3)    //处理上个月是2月份 
            { 
                if((DD==1) && IsLeapYear(YY)==0)                    //不是闰年 
                { 
                    DD    =    28; 
                    MM    --; 
                } 
                else    DD--; 
            } 
            else if(MM==1)    //处理1月份 
            { 
                if(DD>1)    DD--; 
                else        //新年第一天 
                {               
                    DD    =    31; 
                    MM    =    12; 
                    YY    --; 
                } 
            } 
        } 
    }         

    ConvertTime->year   =    YY;                //更新年 
    ConvertTime->mon    =    MM;                //更新月 
    ConvertTime->day    =    DD;                //更新日 
    ConvertTime->hour   =    hh;                //更新时 
    ConvertTime->min    =    mm;                //更新分 
    ConvertTime->sec    =    ss;                //更新秒 
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
  * 函数功能: 解码GPS模块信息
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无
  */
int nmea_decode_test(void)
{
  nmeaINFO info;          //GPS解码后得到的信息
  nmeaPARSER parser;      //解码时使用的数据结构  
  uint8_t new_parse=0;    //是否有新的解码数据标志

  nmeaTIME beiJingTime;    //北京时间 

  /* 设置用于输出调试信息的函数 */
  nmea_property()->trace_func = &trace;
  nmea_property()->error_func = &error;

  /* 初始化GPS数据结构 */
  nmea_zero_INFO(&info);
  nmea_parser_init(&parser);
  
  /* 使用DMA传输数据到电脑端 */
  HAL_UART_Receive_DMA(&husartx,gps_rbuff,GPS_RBUFF_SIZE);  
  
//  while(1)
//  {
//    if(GPS_HalfTransferEnd)     /* 接收到GPS_RBUFF_SIZE一半的数据 */
//    {
//      /* 进行nmea格式解码 */
//      nmea_parse(&parser, (const char*)&gps_rbuff[0], HALF_GPS_RBUFF_SIZE, &info);
//      GPS_HalfTransferEnd = 0;   //清空标志位
//      new_parse = 1;             //设置解码消息标志 
//    }
//    else if(GPS_TransferEnd)    /* 接收到另一半数据 */
//    {          
//      nmea_parse(&parser, (const char*)&gps_rbuff[HALF_GPS_RBUFF_SIZE], HALF_GPS_RBUFF_SIZE, &info);
//      GPS_TransferEnd = 0;
//      new_parse =1;
//    }
//    if(new_parse )                //有新的解码消息   
//    {             
//      /* 对解码后的时间进行转换，转换成北京时间 */
//      GMTconvert(&info.utc,&beiJingTime,8,1);      
//      /* 输出解码得到的信息 */
//      printf("\n时间%d,%d,%d,%d,%d,%d\n", beiJingTime.year+1900, beiJingTime.mon+1,beiJingTime.day,beiJingTime.hour,beiJingTime.min,beiJingTime.sec);
//      printf("纬度：%f,经度%f\n",info.lat/100,info.lon/100);
//      printf("正在使用的卫星：%d,可见卫星：%d\n",info.satinfo.inuse,info.satinfo.inview);
//      printf("海拔高度：%f 米 \n", info.elv);
//      printf("速度：%f km/h \n", info.speed);
//      printf("航向：%f 度\n", info.direction);      
//			
//      new_parse = 0;
//    }
//  }
  while(1)
  {
    if(GPS_HalfTransferEnd)     /* 接收到GPS_RBUFF_SIZE一半的数据 */
    {
      /* 进行nmea格式解码 */
      nmea_parse(&parser, (const char*)&gps_rbuff[0], HALF_GPS_RBUFF_SIZE, &info);
      GPS_HalfTransferEnd = 0;   //清空标志位
      new_parse = 1;             //设置解码消息标志 
    }
    else if(GPS_TransferEnd)    /* 接收到另一半数据 */
    {          
      nmea_parse(&parser, (const char*)&gps_rbuff[HALF_GPS_RBUFF_SIZE], HALF_GPS_RBUFF_SIZE, &info);
      GPS_TransferEnd = 0;
      new_parse =1;
    }
    if(new_parse )                //有新的解码消息   
    {             
      /* 对解码后的时间进行转换，转换成北京时间 */
      GMTconvert(&info.utc,&beiJingTime,8,1);      
      /* 输出解码得到的信息 */
			
 //printf("\n时间%d,%d,%d,%d,%d,%d\n", beiJingTime.year+1900, beiJingTime.mon+1,beiJingTime.day,beiJingTime.hour,beiJingTime.min,beiJingTime.sec);
//      printf("纬度：%f,经度%f\n",info.lat/100,info.lon/100);
	//printf("正在使用的卫星：%d,可见卫星：%d\n",info.satinfo.inuse,info.satinfo.inview);
//      printf("海拔高度：%f 米 \n", info.elv);
     // printf("速度：%f km/h \n", info.satinfo.inview);
		
char str[2]; 
int id=beiJingTime.sec;
SSD1306_Fill(0);
sprintf(str,"%d",id); 
SSD1306_GotoXY(10,52);
SSD1306_Puts(str, &Font_7x10, 1);
SSD1306_UpdateScreen(); //display
//      printf("航向：%f 度\n", info.direction);      
			
      new_parse = 0;
    }
  }
}



/******************* (C) COPYRIGHT 2015-2020 硬石嵌入式开发团队 *****END OF FILE****/
