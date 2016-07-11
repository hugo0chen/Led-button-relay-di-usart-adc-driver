#include "LCD.h"
#include "stdlib.h"
#include "font.h"
//#include "RS485.h"
#include "Delay.h"
#include "spi1.h"

//LCD�Ļ�����ɫ�ͱ���ɫ
u16 POINT_COLOR=0x0000;	//������ɫ
u16 BACK_COLOR=0xFFFF;  //����ɫ

//����LCD��Ҫ����
_lcd_dev lcddev;


//д�Ĵ�������
//regval:�Ĵ���ֵ
void LCD_WR_REG(u16 regval)
{
    SPILCD_CS_RESET;  //LCD_CS=0
    SPILCD_RS_RESET;
    SPI_WriteByte(SPI1,regval&0x00FF);
    SPILCD_CS_SET;  //LCD_CS=1
}
//дLCD����
//data:Ҫд���ֵ
void LCD_WR_DATA(u16 data)
{
    SPILCD_CS_RESET;  //LCD_CS=0
    SPILCD_RS_SET;
    SPI_WriteByte(SPI1,data>>8);
    SPI_WriteByte(SPI1,data);
    SPILCD_CS_SET;  //LCD_CS=1
}
void LCD_WR_DATA8(u8 da)   //д8λ����
{
    SPILCD_CS_RESET;  //LCD_CS=0
    SPILCD_RS_SET;
    SPI_WriteByte(SPI1,da);
    SPILCD_CS_SET;  //LCD_CS=1
}
//д�Ĵ���
//LCD_Reg:�Ĵ�����ַ
//LCD_RegValue:Ҫд�������
void LCD_WR_REG_DATA(u8 LCD_Reg, u16 LCD_RegValue)
{
    LCD_WR_REG(LCD_Reg);
    LCD_WR_DATA(LCD_RegValue);
}
//��ʼдGRAM
void LCD_WriteRAM_Prepare(void)
{
    LCD_WR_REG(lcddev.wramcmd);
}
//��mdk -O1ʱ���Ż�ʱ��Ҫ����
//��ʱi
void opt_delay(u8 i)
{
    while (i--);
}
//LCD������ʾ
void LCD_DisplayOn(void)
{

}
//LCD�ر���ʾ
void LCD_DisplayOff(void)
{

}

//���ù��λ��
//Xpos:������
//Ypos:������
void LCD_SetCursor(u16 Xpos, u16 Ypos)
{
    LCD_WR_REG(lcddev.setxcmd);
    LCD_WR_DATA8(Xpos>>8);
    LCD_WR_DATA8(Xpos&0XFF);
    LCD_WR_REG(lcddev.setycmd);
    LCD_WR_DATA8(Ypos>>8);
    LCD_WR_DATA8(Ypos&0XFF);
}

//����
//x,y:����
//POINT_COLOR:�˵����ɫ
void LCD_DrawPoint(u16 x,u16 y)
{
    LCD_SetCursor(x,y);		//���ù��λ��
    LCD_WriteRAM_Prepare();	//��ʼд��GRAM
    LCD_WR_DATA(POINT_COLOR);
}
//��ʼ��lcd
void LCD_Init(void)
{
    SPILCD_RST_RESET ;	//LCD_RST=0	 //SPI�ӿڸ�λ
    Delay_nms(20); // delay 20 ms
    SPILCD_RST_SET ;	//LCD_RST=1
    Delay_nms(20);

    lcddev.width=320;
    lcddev.height=240;
    lcddev.wramcmd=0X2C;
    lcddev.setxcmd=0X2A;
    lcddev.setycmd=0X2B;

    LCD_WR_REG(0xCB);
    LCD_WR_DATA8(0x39);
    LCD_WR_DATA8(0x2C);
    LCD_WR_DATA8(0x00);
    LCD_WR_DATA8(0x34);
    LCD_WR_DATA8(0x02);

    LCD_WR_REG(0xCF);
    LCD_WR_DATA8(0x00);
    LCD_WR_DATA8(0XC1);
    LCD_WR_DATA8(0X30);

    LCD_WR_REG(0xE8);
    LCD_WR_DATA8(0x85);
    LCD_WR_DATA8(0x00);
    LCD_WR_DATA8(0x78);

    LCD_WR_REG(0xEA);
    LCD_WR_DATA8(0x00);
    LCD_WR_DATA8(0x00);

    LCD_WR_REG(0xED);
    LCD_WR_DATA8(0x64);
    LCD_WR_DATA8(0x03);
    LCD_WR_DATA8(0X12);
    LCD_WR_DATA8(0X81);

    LCD_WR_REG(0xF7);
    LCD_WR_DATA8(0x20);

    LCD_WR_REG(0xC0);    //Power control
    LCD_WR_DATA8(0x23);   //VRH[5:0]

    LCD_WR_REG(0xC1);    //Power control
    LCD_WR_DATA8(0x10);   //SAP[2:0];BT[3:0]

    LCD_WR_REG(0xC5);    //VCM control
    LCD_WR_DATA8(0x3e); //�Աȶȵ���
    LCD_WR_DATA8(0x28);

    LCD_WR_REG(0xC7);    //VCM control2
    LCD_WR_DATA8(0x86);  //--

    LCD_WR_REG(0x36);    // Memory Access Control
    LCD_WR_DATA8(0x28); //C8	   //48 68����//28 E8 ����

    LCD_WR_REG(0X2A);
    LCD_WR_DATA8(0x00);	//start
    LCD_WR_DATA8(0x00);
    LCD_WR_DATA8(0x01);	//end
    LCD_WR_DATA8(0x3F);

    LCD_WR_REG(0X2B);
    LCD_WR_DATA8(0x00);   //start
    LCD_WR_DATA8(0x00);
    LCD_WR_DATA8(0x00);   //end
    LCD_WR_DATA8(0xEF);

    LCD_WR_REG(0x3A);
    LCD_WR_DATA8(0x55);

    LCD_WR_REG(0xB1);
    LCD_WR_DATA8(0x00);
    LCD_WR_DATA8(0x18);

    LCD_WR_REG(0xB6);    // Display Function Control
    LCD_WR_DATA8(0x08);
    LCD_WR_DATA8(0x82);
    LCD_WR_DATA8(0x27);

    LCD_WR_REG(0xF2);    // 3Gamma Function Disable
    LCD_WR_DATA8(0x00);

    LCD_WR_REG(0x26);    //Gamma curve selected
    LCD_WR_DATA8(0x01);

    LCD_WR_REG(0xE0);    //Set Gamma
    LCD_WR_DATA8(0x0F);
    LCD_WR_DATA8(0x31);
    LCD_WR_DATA8(0x2B);
    LCD_WR_DATA8(0x0C);
    LCD_WR_DATA8(0x0E);
    LCD_WR_DATA8(0x08);
    LCD_WR_DATA8(0x4E);
    LCD_WR_DATA8(0xF1);
    LCD_WR_DATA8(0x37);
    LCD_WR_DATA8(0x07);
    LCD_WR_DATA8(0x10);
    LCD_WR_DATA8(0x03);
    LCD_WR_DATA8(0x0E);
    LCD_WR_DATA8(0x09);
    LCD_WR_DATA8(0x00);

    LCD_WR_REG(0XE1);    //Set Gamma
    LCD_WR_DATA8(0x00);
    LCD_WR_DATA8(0x0E);
    LCD_WR_DATA8(0x14);
    LCD_WR_DATA8(0x03);
    LCD_WR_DATA8(0x11);
    LCD_WR_DATA8(0x07);
    LCD_WR_DATA8(0x31);
    LCD_WR_DATA8(0xC1);
    LCD_WR_DATA8(0x48);
    LCD_WR_DATA8(0x08);
    LCD_WR_DATA8(0x0F);
    LCD_WR_DATA8(0x0C);
    LCD_WR_DATA8(0x31);
    LCD_WR_DATA8(0x36);
    LCD_WR_DATA8(0x0F);

    LCD_WR_REG(0x11);    //Exit Sleep
    Delay_nms(120);

    LCD_WR_REG(0x29);    //Display on
    LCD_WR_REG(0x2c);
    LCD_Clear(WHITE);

}
//��������
//color:Ҫ���������ɫ
void LCD_Clear(u16 color)
{
    u32 index=0;
    u32 totalpoint=lcddev.width;
    totalpoint*=lcddev.height; 	//�õ��ܵ���
    LCD_SetCursor(0x00,0x0000);	//���ù��λ��
    LCD_WriteRAM_Prepare();     //��ʼд��GRAM
    for (index=0;index<totalpoint;index++)
    {
        LCD_WR_DATA(color);
    }

}

//��ָ��λ����ʾһ������(16*16��С)
void showhanzi16(unsigned int x,unsigned int y,unsigned char index)
{
    unsigned char i,j,k;
    const unsigned char *temp=hanzi16;
    temp+=index*32;
    for (j=0;j<16;j++)
    {
        LCD_SetCursor(x,y+j);
        LCD_WriteRAM_Prepare();	//��ʼд��GRAM
        for (k=0;k<2;k++)
        {
            for (i=0;i<8;i++)
            {
                if ((*temp&(1<<i))!=0)
                {
                    LCD_WR_DATA(POINT_COLOR);
                }
                else
                {
                    LCD_WR_DATA(BACK_COLOR);
                }
            }
            temp++;
        }
    }
}
//��ָ��λ����ʾһ������(32*32��С)
void showhanzi32(unsigned int x,unsigned int y,unsigned char index)
{
    unsigned char i,j,k;
    const unsigned char *temp=hanzi32;
    temp+=index*128;
    for (j=0;j<32;j++)
    {
        LCD_SetCursor(x,y+j);
        LCD_WriteRAM_Prepare();	//��ʼд��GRAM
        for (k=0;k<4;k++)
        {
            for (i=0;i<8;i++)
            {
                if ((*temp&(1<<i))!=0)
                {
                    LCD_WR_DATA(POINT_COLOR);
                }
                else
                {
                    LCD_WR_DATA(BACK_COLOR);
                }
            }
            temp++;
        }
    }
}

//��ָ��λ����ʾһ������(16*16��С)
void DispHZ16(unsigned int x,unsigned int y,unsigned char index)
{
    unsigned char i,j,k,n;
    n=0;
    for (j=0;j<16;j++)
    {
        LCD_SetCursor(x,y+j);
        LCD_WriteRAM_Prepare();	//��ʼд��GRAM
        for (k=0;k<2;k++)
        {
            for (i=0;i<8;i++)
            {
                if ((asc1_1616[index][n]&(1<<i))!=0)
                {
                    LCD_WR_DATA(POINT_COLOR);
                }
                else
                {
                    LCD_WR_DATA(BACK_COLOR);
                }
            }
            n++;
        }
    }
}
//��ָ��λ����ʾһ���ַ�
//x,y:��ʼ����
//num:Ҫ��ʾ���ַ�:" "--->"~"
//size:�����С 12/16
//mode:���ӷ�ʽ(1)���Ƿǵ��ӷ�ʽ(0)
void LCD_ShowChar(u16 x,u16 y,u8 num,u8 size,u8 mode)
{
    u8 temp,t1,t;
    u16 y0=y;
    u16 colortemp=POINT_COLOR;
    //���ô���
    num=num-' ';//�õ�ƫ�ƺ��ֵ
    if (!mode) //�ǵ��ӷ�ʽ
    {
        for (t=0;t<size;t++)
        {
            if (size==12){
                //����1206����
                temp=asc2_1206[num][t]; 
            }else{ 
                //����1608����
                temp=asc2_1608[num][t];
            }
            for (t1=0;t1<8;t1++)
            {
                if (temp&0x80)POINT_COLOR=colortemp;
                else POINT_COLOR=BACK_COLOR;
                LCD_DrawPoint(x,y);
                temp<<=1;
                y++;
                if (y>=lcddev.height){
                    //��������
                    POINT_COLOR=colortemp;                        
                    return;                
                }
                if ((y-y0)==size)
                {
                    y=y0;
                    x++;
                    if (x>=lcddev.width){
                        //��������
                        POINT_COLOR=colortemp;                            
                        return;                    
                    }
                    break;
                }
            }
        }
    }else
    {
        //���ӷ�ʽ
        for (t=0;t<size;t++)
        {
            if (size==12){
                //����1206����
                temp=asc2_1206[num][t];
            }
            else{
                //����1608����
                temp=asc2_1608[num][t];		 
            }
            for (t1=0;t1<8;t1++)
            {
                if (temp&0x80)LCD_DrawPoint(x,y);
                temp<<=1;
                y++;
                if (y>=lcddev.height){ 
                    //��������  
                    POINT_COLOR=colortemp;        
                    return;               
                }
                if ((y-y0)==size)
                {
                    y=y0;
                    x++;
                    if (x>=lcddev.width){  
                        //��������     
                        POINT_COLOR=colortemp;    
                        return;                  
                    }
                    break;
                }
            }
        }
    }
    POINT_COLOR=colortemp;
}
//m^n����
//����ֵ:m^n�η�.
u32 LCD_Pow(u8 m,u8 n)
{
    u32 result=1;
    while (n--)result*=m;
    return result;
}
//��ʾ����,��λΪ0,����ʾ
//x,y :�������
//len :���ֵ�λ��
//size:�����С
//color:��ɫ
//num:��ֵ(0~4294967295);
void LCD_ShowNum(u16 x,u16 y,u32 num,u8 len,u8 size)
{
    u8 t,temp;
    u8 enshow=0;
    for (t=0;t<len;t++)
    {
        temp=(num/LCD_Pow(10,len-t-1))%10;
        if (enshow==0&&t<(len-1))
        {
            if (temp==0)
            {
                LCD_ShowChar(x+(size/2)*t,y,' ',size,0);
                continue;
            }            else enshow=1;

        }
        LCD_ShowChar(x+(size/2)*t,y,temp+'0',size,0);
    }
}
//��ʾ����,��λΪ0,������ʾ
//x,y:�������
//num:��ֵ(0~999999999);
//len:����(��Ҫ��ʾ��λ��)
//size:�����С
//mode:
//[7]:0,�����;1,���0.
//[6:1]:����
//[0]:0,�ǵ�����ʾ;1,������ʾ.
void LCD_ShowxNum(u16 x,u16 y,u32 num,u8 len,u8 size,u8 mode)
{
    u8 t,temp;
    u8 enshow=0;
    for (t=0;t<len;t++)
    {
        temp=(num/LCD_Pow(10,len-t-1))%10;
        if (enshow==0&&t<(len-1))
        {
            if (temp==0)
            {
                if (mode&0X80)LCD_ShowChar(x+(size/2)*t,y,'0',size,mode&0X01);
                else LCD_ShowChar(x+(size/2)*t,y,' ',size,mode&0X01);
                continue;
            }            else enshow=1;

        }
        LCD_ShowChar(x+(size/2)*t,y,temp+'0',size,mode&0X01);
    }
}
//��ʾ�ַ���
//x,y:�������
//width,height:�����С
//size:�����С
//*p:�ַ�����ʼ��ַ
void LCD_ShowString(u16 x,u16 y,u16 width,u16 height,u8 size,const char *p)
{
    u8 x0=x;
    width+=x;
    height+=y;
    while ((*p<='~')&&(*p>=' '))//�ж��ǲ��ǷǷ��ַ�!
    {
        if (x>=width)        {            x=x0;            y+=size;        }
        if (y>=height)break;//�˳�
        LCD_ShowChar(x,y,*p,size,0);
        x+=size/2;
        p++;
    }
}

void showhz(u16 x,u16 y) //��ʾ40*40ͼƬ
{
    u16 i,j,k;
    u16 da;
    k=0;
    for (i=0;i<130;i++)
    {
        LCD_SetCursor(x,y+i);
        LCD_WriteRAM_Prepare();     			//��ʼд��GRAM
        for (j=0;j<200;j++)
        {
            da=gImage_huizeit[k*2+1];
            da<<=8;
            da|=gImage_huizeit[k*2];
            LCD_WR_DATA(da);
            k++;
        }
    }    
}

void showcsh(u16 x,u16 y) //��ʾ40*40ͼƬ
{
    u16 i,j,k;
    u16 da;
    k=0;
    for (i=0;i<150;i++)
    {
        LCD_SetCursor(x,y+i);
        LCD_WriteRAM_Prepare();     			//��ʼд��GRAM
        for (j=0;j<200;j++)
        {
            da=gImage_csh[k*2+1];
            da<<=8;
            da|=gImage_csh[k*2];
            LCD_WR_DATA(da);
            k++;
        }
    }    
}



