/*****************************************************************************
* | File      	:   EPD_2in13_V2.c
* | Author      :   Waveshare team
* | Function    :   2.13inch e-paper V2
* | Info        :
*----------------
* |	This version:   V3.0
* | Date        :   2019-06-13
* | Info        :
* -----------------------------------------------------------------------------
* V3.0(2019-06-13):
* 1.Change name:
*    EPD_Reset() => EPD_2IN13_V2_Reset()
*    EPD_SendCommand() => EPD_2IN13_V2_SendCommand()
*    EPD_SendData() => EPD_2IN13_V2_SendData()
*    EPD_WaitUntilIdle() => EPD_2IN13_V2_ReadBusy()
*    EPD_Init() => EPD_2IN13_V2_Init()
*    EPD_Clear() => EPD_2IN13_V2_Clear()
*    EPD_Display() => EPD_2IN13_V2_Display()
*    EPD_Sleep() => EPD_2IN13_V2_Sleep()
* 2.add:
*    EPD_2IN13_V2_DisplayPartBaseImage()
* -----------------------------------------------------------------------------
* V2.0(2018-11-14):
* 1.Remove:ImageBuff[EPD_HEIGHT * EPD_WIDTH / 8]
* 2.Change:EPD_2IN13_V2_Display(UBYTE *Image)
*   Need to pass parameters: pointer to cached data
* 3.Change:
*   EPD_RST -> EPD_RST_PIN
*   EPD_DC -> EPD_DC_PIN
*   EPD_CS -> EPD_CS_PIN
*   EPD_BUSY -> EPD_BUSY_PIN
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documnetation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to  whom the Software is
# furished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS OR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
#
******************************************************************************/
#include "EPD_2in13_V2.h"
#include "Debug.h"

const unsigned char EPD_2IN13_V2_lut_full_update[] = {
        0x80, 0x60, 0x40, 0x00, 0x00, 0x00, 0x00,             //LUT0: BB:     VS 0 ~7
        0x10, 0x60, 0x20, 0x00, 0x00, 0x00, 0x00,             //LUT1: BW:     VS 0 ~7
        0x80, 0x60, 0x40, 0x00, 0x00, 0x00, 0x00,             //LUT2: WB:     VS 0 ~7
        0x10, 0x60, 0x20, 0x00, 0x00, 0x00, 0x00,             //LUT3: WW:     VS 0 ~7
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,             //LUT4: VCOM:   VS 0 ~7

        0x03, 0x03, 0x00, 0x00, 0x02,                       // TP0 A~D RP0
        0x09, 0x09, 0x00, 0x00, 0x02,                       // TP1 A~D RP1
        0x03, 0x03, 0x00, 0x00, 0x02,                       // TP2 A~D RP2
        0x00, 0x00, 0x00, 0x00, 0x00,                       // TP3 A~D RP3
        0x00, 0x00, 0x00, 0x00, 0x00,                       // TP4 A~D RP4
        0x00, 0x00, 0x00, 0x00, 0x00,                       // TP5 A~D RP5
        0x00, 0x00, 0x00, 0x00, 0x00,                       // TP6 A~D RP6

        0x15, 0x41, 0xA8, 0x32, 0x30, 0x0A,
};

const unsigned char EPD_2IN13_V2_lut_partial_update[] = {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,             //LUT0: BB:     VS 0 ~7
        0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,             //LUT1: BW:     VS 0 ~7
        0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,             //LUT2: WB:     VS 0 ~7
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,             //LUT3: WW:     VS 0 ~7
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,             //LUT4: VCOM:   VS 0 ~7

        0x0A, 0x00, 0x00, 0x00, 0x00,                       // TP0 A~D RP0
        0x00, 0x00, 0x00, 0x00, 0x00,                       // TP1 A~D RP1
        0x00, 0x00, 0x00, 0x00, 0x00,                       // TP2 A~D RP2
        0x00, 0x00, 0x00, 0x00, 0x00,                       // TP3 A~D RP3
        0x00, 0x00, 0x00, 0x00, 0x00,                       // TP4 A~D RP4
        0x00, 0x00, 0x00, 0x00, 0x00,                       // TP5 A~D RP5
        0x00, 0x00, 0x00, 0x00, 0x00,                       // TP6 A~D RP6

        0x15, 0x41, 0xA8, 0x32, 0x30, 0x0A,
};

void EPD_GPIO_init(void){
    //EPD_VCC ENABLE
    GPIO_ConfigurePin(GPIO_PORT_2, GPIO_PIN_3, OUTPUT, PID_GPIO, true);   //en管脚
    GPIO_SetActive(GPIO_PORT_2, GPIO_PIN_3);                              //en管脚
    // DEV_Delay_ms(10);
    GPIO_ConfigurePin(GPIO_PORT_2, GPIO_PIN_0, INPUT, PID_GPIO, false);    //busy管脚
    GPIO_ConfigurePin(GPIO_PORT_0, GPIO_PIN_7, OUTPUT, PID_GPIO, true);    //rst管脚
    EPD_RST_H;
    GPIO_ConfigurePin(GPIO_PORT_0, GPIO_PIN_5, OUTPUT, PID_GPIO, false);    //dc管脚
    GPIO_ConfigurePin(GPIO_PORT_2, GPIO_PIN_1, OUTPUT, PID_GPIO, false);    //cs管脚
    GPIO_ConfigurePin(GPIO_PORT_0, GPIO_PIN_0, OUTPUT, PID_GPIO, false);    //sclk管脚
    GPIO_ConfigurePin(GPIO_PORT_0, GPIO_PIN_6, OUTPUT, PID_GPIO, false);    //sdi管脚
    
    
    EPD_SDI_L; 
    EPD_SCLK_L;
    EPD_DC_H;
    EPD_CS_L;
  
}

void DEV_SPI_WriteByte(uint8_t sdbyte)
{
    uint8_t i;
    for (i = 0; i < 8; i++)
    {
        EPD_SCLK_L;
        if (sdbyte & 0x80)
        {
            EPD_SDI_H;
        }
        else
        {
            EPD_SDI_L;
        }
        sdbyte <<= 1;
        EPD_SCLK_H;
    }
    EPD_SCLK_L;
}

/******************************************************************************
function :	Software reset
parameter:
******************************************************************************/
static void EPD_2IN13_V2_Reset(void)
{
     EPD_RST_L;
    DEV_Delay_ms(10);
     EPD_RST_H;
    DEV_Delay_ms(10);
}

/******************************************************************************
function :	send command
parameter:
     Reg : Command register
******************************************************************************/
static void EPD_2IN13_V2_SendCommand(UBYTE Reg)
{
    EPD_DC_L;
    EPD_CS_L;
    DEV_SPI_WriteByte(Reg);
    EPD_CS_H;
}

/******************************************************************************
function :	send data
parameter:
    Data : Write data
******************************************************************************/
static void EPD_2IN13_V2_SendData(UBYTE Data)
{
    EPD_DC_H;
    EPD_CS_L;
    DEV_SPI_WriteByte(Data);
    EPD_CS_H;
}

/******************************************************************************
function :	Wait until the busy_pin goes LOW
parameter:
******************************************************************************/
void EPD_2IN13_V2_ReadBusy(void)
{
    Debug("e-Paper busy\r\n");
    while (EPD_BUSY !=0)
    { //LOW: idle, HIGH: busy
      
    }
    Debug("e-Paper busy release\r\n");
}

/******************************************************************************
function :	Turn On Display
parameter:
******************************************************************************/
 void EPD_2IN13_V2_TurnOnDisplay(void)
{
    EPD_2IN13_V2_SendCommand(0x22);
    EPD_2IN13_V2_SendData(0xC7);
    EPD_2IN13_V2_SendCommand(0x20);
    EPD_2IN13_V2_ReadBusy();
}
 void EPD_2IN13_V2_TurnOnDisplay_RED(void)
{
    EPD_2IN13_V2_SendCommand(0x22);
    EPD_2IN13_V2_SendData(0xF7);
    EPD_2IN13_V2_SendCommand(0x20);
    EPD_2IN13_V2_ReadBusy();
}
/******************************************************************************
function :	Turn On Display
parameter:
******************************************************************************/
 void EPD_2IN13_V2_TurnOnDisplayPart(void)
{
    EPD_2IN13_V2_SendCommand(0x22);
    EPD_2IN13_V2_SendData(0x0C);
    EPD_2IN13_V2_SendCommand(0x20);
    EPD_2IN13_V2_ReadBusy();
}

void EPD_2IN13_V2_Init_RED(void) {

    /* EPD hardware init start */
        EPD_2IN13_V2_Reset();
    
        EPD_2IN13_V2_ReadBusy();
        EPD_2IN13_V2_SendCommand(0x12); // soft reset
        EPD_2IN13_V2_ReadBusy();

        EPD_2IN13_V2_SendCommand(0x74); //set analog block control
        EPD_2IN13_V2_SendData(0x54);
        EPD_2IN13_V2_SendCommand(0x7E); //set digital block control
        EPD_2IN13_V2_SendData(0x3B);

        EPD_2IN13_V2_SendCommand(0x01); //Driver output control
        EPD_2IN13_V2_SendData(0xD3);
        EPD_2IN13_V2_SendData(0x00);
        EPD_2IN13_V2_SendData(0x00);

        EPD_2IN13_V2_SendCommand(0x11); //data entry mode
        EPD_2IN13_V2_SendData(0x03);

        EPD_2IN13_V2_SendCommand(0x44); //set Ram-X address start/end position
        EPD_2IN13_V2_SendData(0x00);
        EPD_2IN13_V2_SendData(0x0C);    //0x0C-->(15+1)*8=128

        EPD_2IN13_V2_SendCommand(0x45); //set Ram-Y address start/end position
        EPD_2IN13_V2_SendData(0x00);   //0xF9-->(249+1)=250
        EPD_2IN13_V2_SendData(0x00);
        EPD_2IN13_V2_SendData(0xD3);
        EPD_2IN13_V2_SendData(0x00);

        EPD_2IN13_V2_SendCommand(0x3C); //BorderWavefrom
        EPD_2IN13_V2_SendData(0x05);

        EPD_2IN13_V2_SendCommand(0x18); // set built in temperature sensor
        EPD_2IN13_V2_SendData(0x80); //	
        EPD_2IN13_V2_SendCommand(0x4E);   // set RAM x address count to 0;
        EPD_2IN13_V2_SendData(0x00);
        EPD_2IN13_V2_SendCommand(0x4F);   // set RAM y address count to 0X127;
        EPD_2IN13_V2_SendData(0x00);
        EPD_2IN13_V2_SendData(0x00);
    
}

/******************************************************************************
function :	Initialize the e-Paper register
parameter:
******************************************************************************/
void EPD_2IN13_V2_Init(UBYTE Mode)
{
    UBYTE count;
    EPD_2IN13_V2_Reset();

    if (Mode == EPD_2IN13_V2_FULL)
    {
        EPD_2IN13_V2_ReadBusy();
        EPD_2IN13_V2_SendCommand(0x12); // soft reset
        EPD_2IN13_V2_ReadBusy();

        EPD_2IN13_V2_SendCommand(0x74); //set analog block control
        EPD_2IN13_V2_SendData(0x54);
        EPD_2IN13_V2_SendCommand(0x7E); //set digital block control
        EPD_2IN13_V2_SendData(0x3B);

        EPD_2IN13_V2_SendCommand(0x01); //Driver output control
        EPD_2IN13_V2_SendData((EPD_2IN13_V2_HEIGHT-1)&0xFF);    //0xF9-->(249+1)=250
        EPD_2IN13_V2_SendData(((EPD_2IN13_V2_HEIGHT-1)&0xFF00)>>8);
        EPD_2IN13_V2_SendData(0x00);

        EPD_2IN13_V2_SendCommand(0x11); //data entry mode
        EPD_2IN13_V2_SendData(0x11);

        EPD_2IN13_V2_SendCommand(0x44); //set Ram-X address start/end position
        EPD_2IN13_V2_SendData(0x00);
        EPD_2IN13_V2_SendData(EPD_2IN13_V2_WIDTH/8-1); //0x0C-->(15+1)*8=128

        EPD_2IN13_V2_SendCommand(0x45); //set Ram-Y address start/end position
        EPD_2IN13_V2_SendData((EPD_2IN13_V2_HEIGHT-1)&0xFF);    //0xF9-->(249+1)=250
        EPD_2IN13_V2_SendData(((EPD_2IN13_V2_HEIGHT-1)&0xFF00)>>8);
        EPD_2IN13_V2_SendData(0x00);
        EPD_2IN13_V2_SendData(0x00);
  

        EPD_2IN13_V2_SendCommand(0x3C); //BorderWavefrom
        EPD_2IN13_V2_SendData(0x01);

        EPD_2IN13_V2_SendCommand(0x2C); //VCOM Voltage
        EPD_2IN13_V2_SendData(0x55);    //

        EPD_2IN13_V2_SendCommand(0x03);
        EPD_2IN13_V2_SendData(EPD_2IN13_V2_lut_full_update[70]);

        EPD_2IN13_V2_SendCommand(0x04); //
        EPD_2IN13_V2_SendData(EPD_2IN13_V2_lut_full_update[71]);
        EPD_2IN13_V2_SendData(EPD_2IN13_V2_lut_full_update[72]);
        EPD_2IN13_V2_SendData(EPD_2IN13_V2_lut_full_update[73]);

        EPD_2IN13_V2_SendCommand(0x3A); //Dummy Line
        EPD_2IN13_V2_SendData(EPD_2IN13_V2_lut_full_update[74]);
        EPD_2IN13_V2_SendCommand(0x3B); //Gate time
        EPD_2IN13_V2_SendData(EPD_2IN13_V2_lut_full_update[75]);
        EPD_2IN13_V2_SendCommand(0x18); // set built in temperature sensor
        EPD_2IN13_V2_SendData(0x80); //
          EPD_2IN13_V2_SendCommand(0x22); // 
        EPD_2IN13_V2_SendData(0xB1); //
        
        // EPD_2IN13_V2_SendCommand(0x20); // load LUT from OTP
        
        EPD_2IN13_V2_SendCommand(0x32);
        for (count = 0; count < 70; count++)
        {
            EPD_2IN13_V2_SendData(EPD_2IN13_V2_lut_full_update[count]);
        }

        EPD_2IN13_V2_ReadBusy();
        EPD_2IN13_V2_SendCommand(0x4E); // set RAM x address count to 0;
        EPD_2IN13_V2_SendData(0x00);
        EPD_2IN13_V2_SendCommand(0x4F); // set RAM y address count to 0X127;
        EPD_2IN13_V2_SendData((EPD_2IN13_V2_HEIGHT-1)&0xFF);    //0xF9-->(249+1)=250
        EPD_2IN13_V2_SendData(((EPD_2IN13_V2_HEIGHT-1)&0xFF00)>>8);
        // EPD_2IN13_V2_SendData(EPD_2IN13_V2_HEIGHT-1);
        // EPD_2IN13_V2_SendData(0x00);
       
    }
    else if (Mode == EPD_2IN13_V2_PART)
    {
        EPD_2IN13_V2_SendCommand(0x2C); //VCOM Voltage
        EPD_2IN13_V2_SendData(0x28);

        EPD_2IN13_V2_ReadBusy();

        EPD_2IN13_V2_SendCommand(0x32);
        for (count = 0; count < 70; count++)
        {
            EPD_2IN13_V2_SendData(EPD_2IN13_V2_lut_partial_update[count]);
        }

        EPD_2IN13_V2_SendCommand(0x37);
        EPD_2IN13_V2_SendData(0x00);
        EPD_2IN13_V2_SendData(0x00);
        EPD_2IN13_V2_SendData(0x00);
        EPD_2IN13_V2_SendData(0x00);
        EPD_2IN13_V2_SendData(0x40);
        EPD_2IN13_V2_SendData(0x00);
        EPD_2IN13_V2_SendData(0x00);

        EPD_2IN13_V2_SendCommand(0x22);
        EPD_2IN13_V2_SendData(0xC0);

        EPD_2IN13_V2_SendCommand(0x20);
        EPD_2IN13_V2_ReadBusy();

        // EPD_2IN13_V2_SendCommand(0x3C); //BorderWavefrom
        // EPD_2IN13_V2_SendData(0x01);
    }
    else
    {
        Debug("error, the Mode is EPD_2IN13_FULL or EPD_2IN13_PART");
    }
}

/**
 * @brief  设置EPD显示窗口位置和大小。
 * @param  x 显示窗口起始X位置。
 * @param  y_x8 显示窗口起始Y位置，设置1等于8像素。
 * @param  x_size 显示窗口X方向大小。
 * @param  y_size_x8 显示窗口Y方向大小，设置1等于8像素。
 * @note   指针已被自动设置至窗口的左上角。
 */
void EPD_SetWindow(uint16_t x, uint8_t y_x8, uint16_t x_size, uint8_t y_size_x8) {
   
    x = EPD_2IN13_V2_HEIGHT - 1 - x;
    x_size = x - x_size + 1;          /* x_size已变为x结束地址 */
    y_size_x8 = y_size_x8 + y_x8 - 1; /* y_size已变为y结束地址 */

    EPD_2IN13_V2_SendCommand(0x44); /* 设置X（短边）起始地址和结束地址，根据扫描方式不同，地址设置也不同 */
    EPD_2IN13_V2_SendData(y_x8);
    EPD_2IN13_V2_SendData(y_size_x8 & 0xFF);
    EPD_2IN13_V2_SendCommand(0x45); /* 设置Y（长边）起始地址和结束地址，根据扫描方式不同，地址设置也不同 */
    EPD_2IN13_V2_SendData(x & 0xFF);
    EPD_2IN13_V2_SendData(0);
    EPD_2IN13_V2_SendData(x_size & 0xFF);
    EPD_2IN13_V2_SendData(0);

    EPD_2IN13_V2_SendCommand(0x4E); /* 设置X（短边）地址计数器 */
    EPD_2IN13_V2_SendData(y_x8);
    EPD_2IN13_V2_SendCommand(0x4F); /* 设置Y（长边）地址计数器 */
    EPD_2IN13_V2_SendData(x & 0xFF);
    EPD_2IN13_V2_SendData(0);
}
/**
 * @brief  向EPD控制器发送指定大小的显示数据。
 * @param  data 要发送数据的指针。
 * @param  data_size 要发送数据的大小。
 */
void EPD_SendRAM(const uint8_t *data, uint16_t data_size)
{
    uint16_t i;
    EPD_2IN13_V2_SendCommand(0x24);
    for(i=0;i<data_size;i++){
        EPD_2IN13_V2_SendData(*(data+i));
    }
}

void EPD_SendOLDRAM(const uint8_t *data, uint16_t data_size)
{
    uint16_t i;
    EPD_2IN13_V2_SendCommand(0x26);
    for(i=0;i<data_size;i++){
        EPD_2IN13_V2_SendData(*(data+i));
    }
}
/**
 * @brief  清除EPD控制器指定区域显示RAM。
 * @note   执行完成后窗口会恢复至全屏幕。
 */
void EPD_ClearArea(uint16_t x, uint8_t y_x8, uint16_t x_size, uint8_t y_size_x8, uint8_t color)
{
    uint16_t i, data_size;

    data_size = x_size * (y_size_x8>>3);
    EPD_SetWindow(x, y_x8, x_size, y_size_x8>>3);
    EPD_2IN13_V2_SendCommand(0x24);
    for (i = 0; i < data_size; i++)
    {
        EPD_2IN13_V2_SendData(color);
    }
    EPD_SetWindow(0, 0, EPD_2IN13_V2_HEIGHT, EPD_2IN13_V2_WIDTH>>3);
}


void EPD_DrawImage( const uint8_t *image, uint16_t x, uint16_t y, uint16_t w, uint16_t h ,bool part)
{
    uint16_t i = 0;
    uint16_t size = w * h / 8;
    EPD_SetWindow(x, y, w, h / 8);
    EPD_2IN13_V2_SendCommand(0x24);
    for (i = 0; i < size; i++)
    {
        EPD_2IN13_V2_SendData(image[i]);
    }
    if (part)
    {
        EPD_2IN13_V2_SendCommand(0x26);
        for (i = 0; i < size; i++)
        {
            EPD_2IN13_V2_SendData(image[i]);
        }
    }
}


/******************************************************************************
function :	Clear screen
parameter:
******************************************************************************/
void EPD_2IN13_V2_Clear(void)
{
    UWORD Width, Height;
    Width = (EPD_2IN13_V2_WIDTH % 8 == 0) ? (EPD_2IN13_V2_WIDTH / 8) : (EPD_2IN13_V2_WIDTH / 8 + 1);
    Height = EPD_2IN13_V2_HEIGHT;

    EPD_2IN13_V2_SendCommand(0x24);
    for (UWORD j = 0; j < Height; j++)
    {
        for (UWORD i = 0; i < Width; i++)
        {
            EPD_2IN13_V2_SendData(0xff);
        }
    }
     EPD_2IN13_V2_SendCommand(0x26);
    for (UWORD j = 0; j < Height; j++)
    {
        for (UWORD i = 0; i < Width; i++)
        {
            EPD_2IN13_V2_SendData(0xff);
        }
    }

    EPD_2IN13_V2_TurnOnDisplay();
}

void EPD_2IN13_V2_Clear_RED(void)
{
    UWORD Width, Height;
    Width = (EPD_2IN13_V2_WIDTH % 8 == 0) ? (EPD_2IN13_V2_WIDTH / 8) : (EPD_2IN13_V2_WIDTH / 8 + 1);
    Height = EPD_2IN13_V2_HEIGHT;

    EPD_2IN13_V2_SendCommand(0x24);
    for (UWORD j = 0; j < Height; j++)
    {
        for (UWORD i = 0; i < Width; i++)
        {
            EPD_2IN13_V2_SendData(0xff);
        }
    }
     EPD_2IN13_V2_SendCommand(0x26);
    for (UWORD j = 0; j < Height; j++)
    {
        for (UWORD i = 0; i < Width; i++)
        {
            EPD_2IN13_V2_SendData(0x00);
        }
    }

    
}

/******************************************************************************
function :	Sends the image buffer in RAM to e-Paper and displays
parameter:
******************************************************************************/
void EPD_2IN13_V2_Display(UBYTE *Image)
{
    UWORD Width, Height;
    Width = (EPD_2IN13_V2_WIDTH % 8 == 0) ? (EPD_2IN13_V2_WIDTH / 8) : (EPD_2IN13_V2_WIDTH / 8 + 1);
    Height = EPD_2IN13_V2_HEIGHT;

    EPD_2IN13_V2_SendCommand(0x24);
    for (UWORD j = 0; j < Height; j++)
    {
        for (UWORD i = 0; i < Width; i++)
        {
            EPD_2IN13_V2_SendData(Image[i + j * Width]);
        }
    }
    EPD_2IN13_V2_TurnOnDisplay();
}

/******************************************************************************
function :	 The image of the previous frame must be uploaded, otherwise the
		         first few seconds will display an exception.
parameter:
******************************************************************************/
void EPD_2IN13_V2_DisplayPartBaseImage(UBYTE *Image)
{
    UWORD Width, Height;
    Width = (EPD_2IN13_V2_WIDTH % 8 == 0) ? (EPD_2IN13_V2_WIDTH / 8) : (EPD_2IN13_V2_WIDTH / 8 + 1);
    Height = EPD_2IN13_V2_HEIGHT;

    UDOUBLE Addr = 0;
    EPD_2IN13_V2_SendCommand(0x24);
    for (UWORD j = 0; j < Height; j++)
    {
        for (UWORD i = 0; i < Width; i++)
        {
            Addr = i + j * Width;
            EPD_2IN13_V2_SendData(Image[Addr]);
        }
    }
    EPD_2IN13_V2_SendCommand(0x26);
    for (UWORD j = 0; j < Height; j++)
    {
        for (UWORD i = 0; i < Width; i++)
        {
            Addr = i + j * Width;
            EPD_2IN13_V2_SendData(Image[Addr]);
        }
    }
    //   EPD_2IN13_V2_TurnOnDisplayPart();
    // EPD_2IN13_V2_TurnOnDisplay();
}

void EPD_2IN13_V2_DisplayPart(UBYTE *Image)
{
    UWORD Width, Height;
    Width = (EPD_2IN13_V2_WIDTH % 8 == 0) ? (EPD_2IN13_V2_WIDTH / 8) : (EPD_2IN13_V2_WIDTH / 8 + 1);
    Height = EPD_2IN13_V2_HEIGHT;
    UDOUBLE Addr = 0;
    EPD_2IN13_V2_SendCommand(0x24);
    for (UWORD j = 0; j < Height; j++)
    {
        for (UWORD i = 0; i < Width; i++)
        {
            Addr = i + j * Width;
            EPD_2IN13_V2_SendData(Image[Addr]);
        }
    }

    EPD_2IN13_V2_TurnOnDisplayPart();
    // EPD_2IN13_V2_SendCommand(0x26);
  
}

/******************************************************************************
function :	Enter sleep mode
parameter:
******************************************************************************/
void EPD_2IN13_V2_Sleep(void)
{
    EPD_2IN13_V2_SendCommand(0x22); //POWER OFF
    EPD_2IN13_V2_SendData(0xC3);
    EPD_2IN13_V2_SendCommand(0x20);

    EPD_2IN13_V2_SendCommand(0x10); //enter deep sleep
    EPD_2IN13_V2_SendData(0x01);
    // DEV_Delay_ms(100);
}
