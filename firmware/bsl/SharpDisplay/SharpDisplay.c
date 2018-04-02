//
//  Sharp BoosterPackLCD SPI
//  Example for library for Sharp BoosterPack LCD with hardware SPI
//
//
//  Author :  Stefan Schauer
//  Date   :  Mar 05, 2015
//  Version:  1.02
//  File   :  SharpDisplay_main.c
//
//  Version:  1.01 : added support for CC3200
//  Version:  1.02 : added print class
//
//  Based on the LCD5110 Library
//  Created by Rei VILO on 28/05/12
//  Copyright (c) 2012 http://embeddedcomputing.weebly.com
//  Licence CC = BY SA NC
//
//  Edited 2015-07-11 by ReiVilo
//  Added setOrientation(), setReverse() and flushReverse()
//  Unchanged #include <OneMsTaskTimer.h>
//
#include "SharpDisplay.h"
#include "SourceSansProBlack15pt.h"
#include "SourceSansProBold7pt.h"
#include "SourceSansProBold10pt.h"
#include "bsp_init.h"

uint8_t _pinReset;
uint8_t _pinSerialData;
uint8_t _pinDISP;
uint8_t _pinVCC;
uint8_t _pinChipSelect;
uint8_t _pinSerialClock;
uint8_t    _autoVCOM;
GFXfont *gfxFont;

uint8_t (*DisplayBuffer)[LCD_HORIZONTAL_MAX/8];

//uint8_t DisplayBuffer[LCD_VERTICAL_MAX][LCD_HORIZONTAL_MAX/8] __attribute__ ((section (".noinit")));  // Put in FRAM

// Booster Pack Pins
    //  7 - P2.2 for SPI_CLK mode
    // 15 - P1.6 for SPI_SIMO mode
    //  6 - P2.4 output pin for SPI_CS
    //  2 - P4.2 as output to supply the LCD
    //  5 - P4.3 as output for DISP
    // Set display's VCC and DISP pins to high


static const uint8_t P_CS   = 6;
static const uint8_t P_VCC  = 2;
static const uint8_t P_DISP = 5;

#define SHARP_LCD_TRAILER_BYTE              0x00

#define SHARP_LCD_CMD_CHANGE_VCOM           0x00
#define SHARP_LCD_CMD_CLEAR_SCREEN          0x20
#define SHARP_LCD_CMD_WRITE_LINE            0x80


unsigned char VCOMbit = 0x40;
#define SHARP_VCOM_TOGGLE_BIT               0x40

unsigned char flagSendToggleVCOMCommand = 0;
#define SHARP_SEND_COMMAND_RUNNING          0x01
#define SHARP_REQUEST_TOGGLE_VCOM           0x02

static void SendToggleVCOMCommand(void);

const uint8_t sun_image[] =
{
    0x1e,    // Image width in pixels
    0x04,    // Image height in rows (1 row = 8 pixels)
    0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x07, 0x03, 0x03, 0x03, 0x01, 0x03, 0x07, 0x0f, 0x7f, 0x7f,
    0x0f, 0x07, 0x03, 0x03, 0x03, 0x03, 0x03, 0x07, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03,
    0x03, 0x03, 0x07, 0x07, 0x9f, 0xff, 0xf0, 0xc0, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x80, 0x80, 0xc0, 0xf0, 0xff, 0xdf, 0x07, 0x07, 0x03, 0x03, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x80, 0x81, 0xe7, 0xff, 0x3f, 0x0f, 0x06, 0x07, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x07, 0x07,
    0x0f, 0x3f, 0xff, 0xe7, 0xc1, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80,
    0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xc0, 0xf8, 0xf8, 0xc0, 0x80, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00
};

const uint8_t heart_image[] =
{
    0x20,    // Image width in pixels
    0x04,    // Image height in rows (1 row = 8 pixels)
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x07, 0x07, 0x07, 0x07, 0x07, 0x03, 0x03, 0x01,
    0x01, 0x03, 0x03, 0x07, 0x07, 0x07, 0x07, 0x07, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x3e, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x3e, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xc0, 0xe0, 0xf0, 0xf8, 0xfc, 0xfc, 0xfe, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xfe, 0xfc, 0xfc, 0xf8, 0xf0, 0xe0, 0xc0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xc0,
    0xc0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const uint8_t walkerman_image[] =
{
   0x28,    // Image width in pixels
   0x05,    // Image height in rows (1 row = 8 pixels)
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x0e, 0x1f, 0x3f, 0x3f, 0x3f, 0x1f, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x01, 0x03, 0x07, 0x0e, 0x1c, 0x1c, 0x3f, 0x3f, 0x3f, 0x3f, 0xbf, 0x9f, 0x81,
   0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x78, 0xf8, 0xe0, 0x80, 0x00, 0x00,
   0x0e, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf3, 0xe1, 0x70, 0x70, 0x38, 0x38, 0x38, 0x18, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x0f, 0xff, 0xff, 0xfc, 0xc0, 0xe0, 0xf0,
   0xf8, 0x7e, 0x3f, 0x0f, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1c, 0x3c, 0x78, 0xf0, 0xf0,
   0xe0, 0xc0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0xf8, 0xfc, 0xf8, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};


uint8_t textx = 0;
uint8_t texty = 0;
uint8_t textstartx =0;
uint8_t textstarty =0;

/* START: Amulet functions for old method of writing display using lines */
// DONT USE IN NEW APPS
// DONT USE IN NEW APPS
void display_text_on_line(char * line, uint8_t linenum, uint8_t stat_bar_height)
{
  setFont(SourceSansProBold7);
  if(_reverse)
    drawTextCentered(stat_bar_height + (linenum * gfxFont->yAdvance), line, 0);
  else
    drawTextCentered(stat_bar_height + (linenum * gfxFont->yAdvance), line, 1);
}

void clear_text_on_line(uint8_t linenum, uint8_t stat_bar_height)
{
  setFont(SourceSansProBold7);
  if(_reverse)
    fillRect(0, stat_bar_height + (linenum * gfxFont->yAdvance), LCD_HORIZONTAL_MAX, gfxFont->yAdvance, 1);
  else
    fillRect(0, stat_bar_height + (linenum * gfxFont->yAdvance), LCD_HORIZONTAL_MAX, gfxFont->yAdvance, 0);
}
// DONT USE IN NEW APPS
// DONT USE IN NEW APPS
/* END: Amulet functions for old method of writing display using lines */


/**
 * Consolidate these SPI libraries!
 * spi_send() - send a byte and recv response.
 *
 */
uint8_t SPI_transfer_display(const uint8_t _data)
{
  /* Wait for previous tx to complete. */
  while (!(UCxxIFG_DISP & UCTXIFG))
    ;

  /* Setting TXBUF clears the TXIFG flag. */
  UCxxTXBUF_DISP = _data;

  /* Wait for a rx character? */
  while (!(UCxxIFG_DISP & UCRXIFG))
    ;

  /* Reading clears RXIFG flag. */
  return UCxxRXBUF_DISP;
}


void setOrientation(uint8_t orientation)
{
    _orientation = orientation % 4;
}

void setReverse(uint8_t reverse)
{
    _reverse = reverse;
}

void reverseFlush()
{
    for (uint8_t i=0; i< LCD_VERTICAL_MAX; i++)
    {
        for (uint8_t j=0; j< (LCD_HORIZONTAL_MAX>>3); j++)
        {
            DisplayBuffer[i][j] = 0xff ^ DisplayBuffer[i][j];
        }
    }
    display_refresh();
}

void setXY(uint8_t x, uint8_t y, uint8_t  ulValue) 
{
    uint8_t x0;
    uint8_t y0;

    // Bound the pixels, if you write to anything larger then the display freaks out
    if(x < 0 || y < 0 || y > LCD_VERTICAL_MAX-1 || x > LCD_HORIZONTAL_MAX - 1) return;

    switch (_orientation)
    {
        case 1:
            x0 = LCD_HORIZONTAL_MAX - y;
            y0 = x;
            break;

        case 2:
            x0 = LCD_HORIZONTAL_MAX - x;
            y0 = LCD_VERTICAL_MAX   - y;
            break;

        case 3:
            x0 = y;
            y0 = LCD_VERTICAL_MAX   - x;
            break;

        default:
            x0 = x;
            y0 = y;
            break;
    }

    if (_reverse) ulValue = (ulValue == 0);

    if (ulValue != 0)   DisplayBuffer[y0][x0>>3] &= ~(0x80 >> (x0 & 0x7));
    else                DisplayBuffer[y0][x0>>3] |=  (0x80 >> (x0 & 0x7));
}

// CS   P9.3
// SI   P1.6
// SCLK P1.4
void display_begin() {
    //SPI.begin();
    //SPI.setBitOrder(MSBFIRST);
    //SPI.setDataMode(SPI_MODE0);
    /* Put USCI in reset mode, source USCI clock from SMCLK. */
    UCxxCTLW0_DISP = UCSWRST | UCSSEL_2;

    /* SPI in master MODE 0 - CPOL=0 SPHA=0. */
    UCxxCTLW0_DISP |= SPI_MODE_0 | UCMSB | UCSYNC | UCMST;

    /* Set pins to SPI mode. */
    P4SEL1 |= (BIT0 | BIT1 | BIT2);
    P4SEL0 &= ~(BIT0 | BIT1 | BIT2);
    /*DISP_SPI_CLK_SEL0 |= DISP_SPI_CLK;
    DISP_SPI_SEL0 |= DISP_SPI_SIMO;
    DISP_SPI_SEL0 |= DISP_SPI_SOMI;
    DISP_SPI_CLK_SEL1 &= ~DISP_SPI_CLK;
    DISP_SPI_SEL1 &= ~DISP_SPI_SIMO;
    DISP_SPI_SEL1 &= ~DISP_SPI_SOMI; */
    //pinMode_int(SCK, SPISCK_SET_MODE);
    //pinMode_int(MOSI, SPIMOSI_SET_MODE);
    //pinMode_int(MISO, SPIMISO_SET_MODE);


    /* Set initial speed to 4MHz. */
    UCxxBR0_DISP = 2 & 0xFF;
    UCxxBR1_DISP = (2 >> 8 ) & 0xFF;

    /* Release USCI for operation. */
    UCxxCTLW0_DISP &= ~UCSWRST;
    DISP_CS_PxDIR |= DISP_CS_BITx;
    DISP_CS_PxOUT &= ~DISP_CS_BITx;
    //display_clear(); // Cant do this before the display buffer is set!
    _orientation = 0;
    _reverse = 0;
    gfxFont = (GFXfont *) &SourceSansPro_Black15pt7b;
}

void display_clear() {

  unsigned char command = SHARP_LCD_CMD_CLEAR_SCREEN;

  // set flag to indicate command transmit is running
  flagSendToggleVCOMCommand |= SHARP_SEND_COMMAND_RUNNING;

  command |= VCOMbit;                    //COM inversion bit

  // Set P2.4 High for CS, active high
  //digitalWrite(_pinChipSelect, HIGH);
  //P9OUT |= BIT3;
  DISP_CS_PxOUT |= DISP_CS_BITx;

  SPI_transfer_display(command);
  SPI_transfer_display(SHARP_LCD_TRAILER_BYTE);

  // Wait for last byte to be sent, then drop SCS
  delayMicroseconds(10);

  // Set P2.4 High for CS
  //digitalWrite(_pinChipSelect, LOW);
  //P9OUT &= ~BIT3;
  DISP_CS_PxOUT &= ~DISP_CS_BITx;

  // clear flag to indicate command transmit is free
  flagSendToggleVCOMCommand &= ~SHARP_SEND_COMMAND_RUNNING;
  SendToggleVCOMCommand(); // send toggle if required

  display_clearBuffer();

}

void display_clear_no_buffer() {
  unsigned char command = SHARP_LCD_CMD_CLEAR_SCREEN;

  // set flag to indicate command transmit is running
  flagSendToggleVCOMCommand |= SHARP_SEND_COMMAND_RUNNING;

  command |= VCOMbit;                    //COM inversion bit

  // Set P2.4 High for CS, active high
  //digitalWrite(_pinChipSelect, HIGH);
  //P9OUT |= BIT3;
  DISP_CS_PxOUT |= DISP_CS_BITx;

  SPI_transfer_display(command);
  SPI_transfer_display(SHARP_LCD_TRAILER_BYTE);

  // Wait for last byte to be sent, then drop SCS
  delayMicroseconds(10);

  // Set P2.4 High for CS
  //digitalWrite(_pinChipSelect, LOW);
  //P9OUT &= ~BIT3;
  DISP_CS_PxOUT &= ~DISP_CS_BITx;

  // clear flag to indicate command transmit is free
  flagSendToggleVCOMCommand &= ~SHARP_SEND_COMMAND_RUNNING;
  SendToggleVCOMCommand(); // send toggle if required
  
}

void display_clearBuffer() {
    for (uint8_t i = 0; i< LCD_VERTICAL_MAX; i++)
        for (uint8_t j = 0; j< (LCD_HORIZONTAL_MAX>>3); j++)
            DisplayBuffer[i][j] = _reverse ? 0x00 : 0xff;
}

//*****************************************************************************
// drawImage
// Draw an Image on the display
// Image can be generated with ImageDog (check for ImageDog on GitHub)
// Header : width in pixel
//          hight in pixel/8
//
//*****************************************************************************
void drawImage(const uint8_t * image, uint8_t x,uint8_t y)
{
    // height in rows (row = 8 pixels), width in columns
    volatile uint8_t imgheight, imgwidth;
    volatile uint8_t data=0;
    imgwidth = *image++;
    imgheight = (*image++)*8;

    for (uint8_t a = 0; a < imgheight; a+=8)
    {
      data=0;
      for (uint8_t i=0; i<imgwidth; i++) {
        data  = *image++;
        for (uint8_t j=0; j<8; j++) {
          data & 0x80 ? setXY(x+i,y+a+j,1) : setXY(x+i,y+a+j,0);
          data <<= 1;
        }
      }
    }
}

void drawHeartImage(uint8_t x,uint8_t y) {
  drawImage(heart_image, x, y);
}

void drawWalkerManImage(uint8_t x,uint8_t y) {
  drawImage(walkerman_image, x, y);
}

void drawSunImage(uint8_t x,uint8_t y) {
  drawImage(sun_image, x, y);
}

const uint8_t referse_data[] = {0x0, 0x8, 0x4, 0xC, 0x2, 0xA, 0x6, 0xE, 0x1, 0x9, 0x5, 0xD, 0x3, 0xB, 0x7, 0xF};
uint8_t reverse(uint8_t x)
{
  uint8_t b = 0;

  b  = referse_data[x & 0xF]<<4;
  b |= referse_data[(x & 0xF0)>>4];
  return b;
}


void display_refresh (void)
{
    __dint(); // Disable interrupts before screen refresh

    unsigned char *pucData = &DisplayBuffer[0][0];
    long xi =0;
    long xj = 0;
    //image update mode(1X000000b)
    unsigned char command = SHARP_LCD_CMD_WRITE_LINE;

    // set flag to indicate command transmit is running
    flagSendToggleVCOMCommand |= SHARP_SEND_COMMAND_RUNNING;
    //COM inversion bit
    command |= VCOMbit;
    // Set P2.4 High for CS
    //digitalWrite(_pinChipSelect, HIGH);
    //P9OUT |= BIT3;
    DISP_CS_PxOUT |= DISP_CS_BITx;

    SPI_transfer_display((char)command);
    for(xj=0; xj<LCD_VERTICAL_MAX; xj++)
    {
        SPI_transfer_display((char)reverse(xj + 1));

        for(xi=0; xi<(LCD_HORIZONTAL_MAX>>3); xi++)
        {
            SPI_transfer_display((char)*(pucData++));
        }
        SPI_transfer_display(SHARP_LCD_TRAILER_BYTE);
    }

    SPI_transfer_display((char)SHARP_LCD_TRAILER_BYTE);
    delayMicroseconds(10);

    // Set P2.4 Low for CS
    //digitalWrite(_pinChipSelect, LOW);
    //P9OUT &= ~BIT3;
    DISP_CS_PxOUT &= ~DISP_CS_BITx;
    // clear flag to indicate command transmit is free
    flagSendToggleVCOMCommand &= ~SHARP_SEND_COMMAND_RUNNING;
    SendToggleVCOMCommand(); // send toggle if required

    __eint(); //Reenable interrupts
}

uint8_t display_height(void) {
    return LCD_HORIZONTAL_MAX;
}

uint8_t display_width(void) {
    return LCD_VERTICAL_MAX;
}

void drawPixel(uint8_t x, uint8_t y, uint8_t ulValue) {
    setXY(x, y, ulValue);
}

// Draw a circle outline
void drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color) {
  int16_t f = 1 - r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * r;
  int16_t x = 0;
  int16_t y = r;

  drawPixel(x0  , y0+r, color);
  drawPixel(x0  , y0-r, color);
  drawPixel(x0+r, y0  , color);
  drawPixel(x0-r, y0  , color);

  while (x<y) {
    if (f >= 0) {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }
    x++;
    ddF_x += 2;
    f += ddF_x;

    drawPixel(x0 + x, y0 + y, color);
    drawPixel(x0 - x, y0 + y, color);
    drawPixel(x0 + x, y0 - y, color);
    drawPixel(x0 - x, y0 - y, color);
    drawPixel(x0 + y, y0 + x, color);
    drawPixel(x0 - y, y0 + x, color);
    drawPixel(x0 + y, y0 - x, color);
    drawPixel(x0 - y, y0 - x, color);
  }
}


void drawCircleHelper( int16_t x0, int16_t y0,
 int16_t r, uint8_t cornername, uint16_t color) {
  int16_t f     = 1 - r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * r;
  int16_t x     = 0;
  int16_t y     = r;

  while (x<y) {
    if (f >= 0) {
      y--;
      ddF_y += 2;
      f     += ddF_y;
    }
    x++;
    ddF_x += 2;
    f     += ddF_x;
    if (cornername & 0x4) {
      drawPixel(x0 + x, y0 + y, color);
      drawPixel(x0 + y, y0 + x, color);
    }
    if (cornername & 0x2) {
      drawPixel(x0 + x, y0 - y, color);
      drawPixel(x0 + y, y0 - x, color);
    }
    if (cornername & 0x8) {
      drawPixel(x0 - y, y0 + x, color);
      drawPixel(x0 - x, y0 + y, color);
    }
    if (cornername & 0x1) {
      drawPixel(x0 - y, y0 - x, color);
      drawPixel(x0 - x, y0 - y, color);
    }
  }
}

// Bresenham's algorithm - thx wikpedia
void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1,
 uint16_t color) {
  int16_t steep = _abs_display(y1 - y0) > _abs_display(x1 - x0);
  if (steep) {
    adagfxswap(x0, y0);
    adagfxswap(x1, y1);
  }

  if (x0 > x1) {
    adagfxswap(x0, x1);
    adagfxswap(y0, y1);
  }

  int16_t dx, dy;
  dx = x1 - x0;
  dy = _abs_display(y1 - y0);

  int16_t err = dx / 2;
  int16_t ystep;

  if (y0 < y1) {
    ystep = 1;
  } else {
    ystep = -1;
  }

  for (; x0<=x1; x0++) {
    if (steep) {
      drawPixel(y0, x0, color);
    } else {
      drawPixel(x0, y0, color);
    }
    err -= dy;
    if (err < 0) {
      y0 += ystep;
      err += dx;
    }
  }
}

void drawFastVLine(int16_t x, int16_t y,
 int16_t h, uint16_t color) {
  // Update in subclasses if desired!
  drawLine(x, y, x, y+h-1, color);
}

/*void drawFastHLine(int16_t x, int16_t y,
 int16_t w, uint16_t color) {
  // Update in subclasses if desired!
  drawLine(x, y, x+w-1, y, color);
}*/

  
void drawFastHLine(uint8_t x, uint8_t y, uint8_t w, uint16_t color) {
  if(x > LCD_HORIZONTAL_MAX-1) return;
  if( y > LCD_VERTICAL_MAX-1) return;
  if(x + w > LCD_HORIZONTAL_MAX) {
    w -= (x+w) - LCD_HORIZONTAL_MAX;
  }
  // Bound
  uint16_t next_8_multiple = (x + 7) & ~7;
  uint8_t mod_start = next_8_multiple - x;
  uint8_t end = x+w;
  uint8_t last_8_multiple = (end) & ~7;
  uint8_t mod_end = end - last_8_multiple;

  uint8_t start_val = 0;
  uint8_t end_val = 0;
  if (color != 0) {
    start_val = ~(0xff >> (x & 0x7));
    end_val = (0xff >> mod_end);
  } else {
    start_val = (0xff >> (x & 0x7));
    end_val = ~(0xff >> mod_end);
  }
  
  // Get the begining edge bitmask if not on byte boundary 
  if(mod_start > 0) {
   uint8_t current_byte_ndx = ((x) & ~7) >> 3;
   if (color != 0) DisplayBuffer[y][current_byte_ndx] &= start_val;
   else DisplayBuffer[y][current_byte_ndx] |= start_val;
  }
  
  // Gets the next multiple of eight, then fills in 
  // the section of line that is on byte boundary
  for(uint8_t i=next_8_multiple;i<end;i+=8) {
    if (color != 0) DisplayBuffer[y][i>>3] &= 0x00;
    else DisplayBuffer[y][i>>3] |= 0xff;
  }

  // Get the end portion bitmask if not on byte boundary
  if(mod_end > 0) {
   uint8_t last_byte_ndx = last_8_multiple >> 3;
   if (color != 0) DisplayBuffer[y][last_byte_ndx] |= end_val;
   else DisplayBuffer[y][last_byte_ndx] &= end_val;
  }
}

void drawRect(int16_t x, int16_t y, int16_t w, int16_t h,
 uint16_t color) {
  drawFastHLine(x, y, w, color);
  drawFastHLine(x, y+h-1, w, color);
  drawFastVLine(x, y, h, color);
  drawFastVLine(x+w-1, y, h, color);
}

// Dont do anything where h > w, as that will be slowww
// Need to update this to account for rect of width 1, to just pass to bresenham
// Also need to update to not recalculate masks every time, as this hurts performance
// for non squares
void fillRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint16_t color) {
  if(x > LCD_HORIZONTAL_MAX-1) return;
  if( y > LCD_VERTICAL_MAX-1) return;
  if((y + h) > LCD_VERTICAL_MAX) {
    h -= (y+h) - (LCD_VERTICAL_MAX);
  }
  if((x + w) > LCD_HORIZONTAL_MAX) {
    w -= (x+w) - (LCD_HORIZONTAL_MAX);
  }
  
  // Bound
  uint8_t end_height = y+h;
  uint16_t next_8_multiple = (x + 7) & ~7;
  uint8_t mod_start = next_8_multiple - x;
  uint8_t end = x+w;
  uint8_t last_8_multiple = (end) & ~7;
  uint8_t mod_end = end - last_8_multiple;

  uint8_t start_val = 0;
  uint8_t end_val = 0;
  if (color != 0) {
    start_val = ~(0xff >> (x & 0x7));
    end_val = (0xff >> mod_end);
  } else {
    start_val = (0xff >> (x & 0x7));
    end_val = ~(0xff >> mod_end);
  }
  for(uint8_t y0=y;y0<end_height;y0++) {
    // Get the begining edge bitmask if not on byte boundary 
    if(mod_start > 0) {
     uint8_t current_byte_ndx = ((x) & ~7) >> 3;
     if (color != 0) DisplayBuffer[y0][current_byte_ndx] &= start_val;
     else DisplayBuffer[y0][current_byte_ndx] |= start_val;
    }
    
    // Gets the next multiple of eight, then fills in 
    // the section of line that is on byte boundary
    for(uint8_t i=next_8_multiple;i<end;i+=8) {
      if (color != 0) DisplayBuffer[y0][i>>3] &= 0x00;
      else DisplayBuffer[y0][i>>3] |= 0xff;
    }

    // Get the end portion bitmask if not on byte boundary
    if(mod_end > 0) {
     uint8_t last_byte_ndx = last_8_multiple >> 3;
     if (color != 0) DisplayBuffer[y0][last_byte_ndx] |= end_val;
     else DisplayBuffer[y0][last_byte_ndx] &= end_val;
    }
  }
}
/*
void fillRect(int16_t x, int16_t y, int16_t w, int16_t h,
 uint16_t color) {
  // Update in subclasses if desired!
  for (int16_t i=x; i<x+w; i++) {
    drawFastVLine(i, y, h, color);
  }
}*/

// Draw a triangle
void drawTriangle(int16_t x0, int16_t y0,
 int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color) {
  drawLine(x0, y0, x1, y1, color);
  drawLine(x1, y1, x2, y2, color);
  drawLine(x2, y2, x0, y0, color);
}

// Fill a triangle
void fillTriangle(int16_t x0, int16_t y0,
 int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color) {

  int16_t a, b, y, last;

  // Sort coordinates by Y order (y2 >= y1 >= y0)
  if (y0 > y1) {
    adagfxswap(y0, y1); adagfxswap(x0, x1);
  }
  if (y1 > y2) {
    adagfxswap(y2, y1); adagfxswap(x2, x1);
  }
  if (y0 > y1) {
    adagfxswap(y0, y1); adagfxswap(x0, x1);
  }

  if(y0 == y2) { // Handle awkward all-on-same-line case as its own thing
    a = b = x0;
    if(x1 < a)      a = x1;
    else if(x1 > b) b = x1;
    if(x2 < a)      a = x2;
    else if(x2 > b) b = x2;
    drawFastHLine(a, y0, b-a+1, color);
    return;
  }

  int16_t
    dx01 = x1 - x0,
    dy01 = y1 - y0,
    dx02 = x2 - x0,
    dy02 = y2 - y0,
    dx12 = x2 - x1,
    dy12 = y2 - y1;
  int32_t
    sa   = 0,
    sb   = 0;

  // For upper part of triangle, find scanline crossings for segments
  // 0-1 and 0-2.  If y1=y2 (flat-bottomed triangle), the scanline y1
  // is included here (and second loop will be skipped, avoiding a /0
  // error there), otherwise scanline y1 is skipped here and handled
  // in the second loop...which also avoids a /0 error here if y0=y1
  // (flat-topped triangle).
  if(y1 == y2) last = y1;   // Include y1 scanline
  else         last = y1-1; // Skip it

  for(y=y0; y<=last; y++) {
    a   = x0 + sa / dy01;
    b   = x0 + sb / dy02;
    sa += dx01;
    sb += dx02;
    /* longhand:
    a = x0 + (x1 - x0) * (y - y0) / (y1 - y0);
    b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
    */
    if(a > b) adagfxswap(a,b);
    drawFastHLine(a, y, b-a+1, color);
  }

  // For lower part of triangle, find scanline crossings for segments
  // 0-2 and 1-2.  This loop is skipped if y1=y2.
  sa = dx12 * (y - y1);
  sb = dx02 * (y - y0);
  for(; y<=y2; y++) {
    a   = x1 + sa / dy12;
    b   = x0 + sb / dy02;
    sa += dx12;
    sb += dx02;
    /* longhand:
    a = x1 + (x2 - x1) * (y - y1) / (y2 - y1);
    b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
    */
    if(a > b) adagfxswap(a,b);
    drawFastHLine(a, y, b-a+1, color);
  }
}

// Draw a 1-bit image (bitmap) at the specified (x,y) position from the
// provided bitmap buffer (must be PROGMEM memory) using the specified
// foreground color (unset bits are transparent).
void drawBitmap(int16_t x, int16_t y,
 const uint8_t *bitmap, int16_t w, int16_t h, uint16_t color) {

  int16_t i, j, byteWidth = (w + 7) / 8;
  uint8_t byte=0;

  for(j=0; j<h; j++) {
    for(i=0; i<w; i++) {
      if(i & 7) byte <<= 1;
      else      byte   = pgm_read_byte(bitmap + j * byteWidth + i / 8);
      if(byte & 0x80) drawPixel(x+i, y+j, color);
    }
  }
}


static void SendToggleVCOMCommand(void)
{
    if(!(flagSendToggleVCOMCommand & SHARP_REQUEST_TOGGLE_VCOM)){ // no request pending ?
        VCOMbit ^= SHARP_VCOM_TOGGLE_BIT;                 // Toggle VCOM Bit
    }

    if(flagSendToggleVCOMCommand & SHARP_SEND_COMMAND_RUNNING){
        // set request flag
        flagSendToggleVCOMCommand |= SHARP_REQUEST_TOGGLE_VCOM;
    }else{  // if no communication to LCD -> send toggle sequence now
        unsigned char command = SHARP_LCD_CMD_CHANGE_VCOM;
        command |= VCOMbit;                    //COM inversion bit

        // Set P2.4 High for CS
        //digitalWrite(_pinChipSelect, HIGH);
        //P9OUT |= BIT3;
        DISP_CS_PxOUT |= DISP_CS_BITx;

        SPI_transfer_display((char)command);
        SPI_transfer_display((char)SHARP_LCD_TRAILER_BYTE);

        // Wait for last byte to be sent, then drop SCS
    delayMicroseconds(10);
        // Set P2.4 High for CS
        //digitalWrite(_pinChipSelect, LOW);
        //P9OUT &= ~BIT3;
        DISP_CS_PxOUT &= ~DISP_CS_BITx;
        // clear request flag
        flagSendToggleVCOMCommand &= ~SHARP_REQUEST_TOGGLE_VCOM;
    }
}


void setFont(tFonts type) {
  switch(type) {
    case SourceSansProBlack15:
      gfxFont = (GFXfont *) &SourceSansPro_Black15pt7b;
      break;
    case SourceSansProBold10:
      gfxFont = (GFXfont *) &SourceSansPro_Bold10pt7b;
      break;
    case SourceSansProBold7:
      gfxFont = (GFXfont *) &SourceSansPro_Bold7pt7b;
      break;
  }
}

void drawTextCentered(uint8_t y, char *line, uint8_t color)
{
  uint8_t offset_t = textWidth(line) / 2;
  drawText(display_width()/2 - offset_t, y, line, color);
}

void drawText(uint8_t x, uint8_t y, char *line, uint8_t textcolor) {
  uint8_t cursor_x = x;
  uint8_t cursor_y = y;
  for (uint8_t j=0; j<strlen(line); j++) {
    char c = line[j];
    if(c == '\n') {
      cursor_x  = 0;
      cursor_y += gfxFont->yAdvance;
    } else if(c != '\r') {
      uint8_t first = gfxFont->first;
      if((c >= first) && (c <= gfxFont->last)) {
        uint8_t   c2    = c - gfxFont->first;
        GFXglyph *glyph = &(gfxFont->glyph)[c2];
        uint8_t   w     = glyph->width,
                  h     = glyph->height;
        if((w > 0) && (h > 0)) { // Is there an associated bitmap?
          int16_t xo = glyph->xOffset; // sic
          if(((cursor_x + (xo + w)) >= LCD_HORIZONTAL_MAX)) {
            // Drawing character would go off right edge; wrap to new line
            cursor_x  = 0;
            cursor_y += gfxFont->yAdvance;
          }
          drawChar(cursor_x, cursor_y, c, textcolor, 0, 1);
        }
        cursor_x += glyph->xAdvance;
      }
    }
  }
}

// Draw a character
void drawChar(int16_t x, int16_t y, unsigned char c,
 uint16_t color, uint16_t bg, uint8_t size) {

    // Character is assumed previously filtered by write() to eliminate
    // newlines, returns, non-printable characters, etc.  Calling drawChar()
    // directly with 'bad' characters of font may cause mayhem!

    c -= gfxFont->first;
    GFXglyph *glyph  = &(gfxFont->glyph)[c];
    uint8_t  *bitmap = gfxFont->bitmap;

    uint16_t bo = glyph->bitmapOffset;
    uint8_t  w  = glyph->width,
             h  = glyph->height,
             xa = glyph->xAdvance;
    int8_t   xo = glyph->xOffset,
             yo = glyph->yOffset + gfxFont->yAdvance;
    uint8_t  xx=0, yy=0, bits=0, bit = 0;
    int16_t  xo16=0, yo16=0;

    if(size > 1) {
      xo16 = xo;
      yo16 = yo;
    }

    for(yy=0; yy<h; yy++) {
      for(xx=0; xx<w; xx++) {
        if(!(bit++ & 7)) {
          bits = bitmap[bo++];
        }
        if(bits & 0x80) {
          if(size == 1) {
            setXY(x+xo+xx, y+yo+yy, color);
          } else {
            fillRect(x+(xo16+xx)*size, y+(yo16+yy)*size, size, size, color);
          }
        }
        //bits <<= 1;
        bits = bits << 1;
      }
    }
}

uint8_t textHeight()
{
  return gfxFont->yAdvance;
}

uint8_t textWidth(char *s)
{
  uint8_t totalwidth=0;
  for(uint8_t i=0; i<strlen(s);i++) {
    unsigned char c = s[i] - gfxFont->first;
    GFXglyph *glyph  = &(gfxFont->glyph)[c];
    totalwidth += glyph->xAdvance;
  }
  return totalwidth;
}

void setDisplayBuffer(uint8_t display_buf[][LCD_HORIZONTAL_MAX/8])
{
  DisplayBuffer = display_buf;
}
