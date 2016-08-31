/* *
 *
 * Copyright 2016 by the Trustees of Dartmouth College and Clemson University, and
 * distributed under the terms of the "Dartmouth College Non-Exclusive Research Use
 * Source Code License Agreement" (for NON-COMMERCIAL research purposes only), as
 * detailed in a file named LICENSE.pdf within this repository.
 */

// SharpDisplay library for Amulet
//
//  Based on the Sharp BoosterPackLCD SPI
//  Created by Stefan Schauer on Mar 05, 2015
//  Licence CC = BY SA NC
//
//  Based on Adafruit GFX library as well

#ifndef SharpDisplay_h
#define SharpDisplay_h

#include <stdint.h>
#include <string.h>
#include "msp430fr5989.h"

typedef struct { // Data stored PER GLYPH
    uint16_t bitmapOffset;     // Pointer into GFXfont->bitmap
    uint8_t  width, height;    // Bitmap dimensions in pixels
    uint8_t  xAdvance;         // Distance to advance cursor (x axis)
    int8_t   xOffset, yOffset; // Dist from cursor pos to UL corner
} GFXglyph;

typedef struct { // Data stored for FONT AS A WHOLE:
    uint8_t  *bitmap;      // Glyph bitmaps, concatenated
    GFXglyph *glyph;       // Glyph array
    uint8_t   first, last; // ASCII extents
    uint8_t   yAdvance;    // Newline distance (y axis)
} GFXfont;

#define LCD_VERTICAL_MAX    128
#define LCD_HORIZONTAL_MAX  128

#define delayMicroseconds(x) __delay_cycles(x*8);
#define delay(x) __delay_cycles(x * 8000UL)


#define _abs_display(x) ((x)>0?(x):-(x))
#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))
#define round(x)     ((x)>=0?(long)((x)+0.5):(long)((x)-0.5))
#define radians(deg) ((deg)*DEG_TO_RAD)
#define degrees(rad) ((rad)*RAD_TO_DEG)
#define sq(x) ((x)*(x))
#define adagfxswap(a, b) { int16_t t = a; a = b; b = t; }
#define pgm_read_byte(addr) (*(const unsigned char *)(addr))

#define SPI_MODE_0 (UCCKPH)     /* CPOL=0 CPHA=0 */
#define SPI_MODE_1 (0)          /* CPOL=0 CPHA=1 */
#define SPI_MODE_2 (UCCKPL | UCCKPH)    /* CPOL=1 CPHA=0 */
#define SPI_MODE_3 (UCCKPL)     /* CPOL=1 CPHA=1 */

#define SPI_MODE_MASK (UCCKPL | UCCKPH)
#define SPI_CLOCK_DIV1   1
#define SPI_CLOCK_DIV2   2
#define SPI_CLOCK_DIV16  16


typedef enum
{
    SourceSansProBlack15,
    SourceSansProBold10,
    SourceSansProBold7

}
tFonts;

///
/// @brief	Initialise the screen
///
void display_begin();

///
/// @brief	Clear the screen
///
void display_clear();

///
/// @brief  Clear the screen but leave the buffer untouched
///
void display_clear_no_buffer();

///
/// @brief	Clear the buffer
///
void display_clearBuffer();

///
/// @brief	Set the orientation
/// @param	orientation 0=0°, 1=90°, 2=180°, 3=-90°
/// @note   Screen initialised at 0=0°.
///
void setOrientation(uint8_t orientation);

///
/// @brief	Set the reverse mode
/// @param	reverse false=silver on white, default=true=white on silver
/// @note   Screen initialised with false=silver on white.
///
void setReverse(uint8_t reverse);

///
/// @brief	Reverse and display the screen
///
void reverseFlush();

/* START: Amulet functions for old method of writing display using lines */
// DONT USE IN NEW APPS
// DONT USE IN NEW APPS
void display_text_on_line(char * line, uint8_t linenum, uint8_t stat_bar_height);
void clear_text_on_line(uint8_t linenum, uint8_t stat_bar_height);
// DONT USE IN NEW APPS
// DONT USE IN NEW APPS
/* END: Amulet functions for old method of writing display using lines */

void setXY(uint8_t x, uint8_t y, uint8_t ulValue);
void display_refresh();
void drawImage(const uint8_t * image, uint8_t x,uint8_t y); // This works
void drawHeartImage(uint8_t x,uint8_t y);
void drawWalkerManImage(uint8_t x,uint8_t y);
void drawSunImage(uint8_t x,uint8_t y);
size_t write(uint8_t c);

uint8_t display_height(void);
uint8_t display_width(void);

// AdaFruit methods, ported
void drawPixel(uint8_t x, uint8_t y, uint8_t ulValue);
void drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
void drawCircleHelper( int16_t x0, int16_t y0, int16_t r, uint8_t cornername, uint16_t color);
//void fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
//void fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, int16_t delta, uint16_t color);
void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
void drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
void drawFastHLine(uint8_t x, uint8_t y, uint8_t w, uint16_t color);
void drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
void fillRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint16_t color);
void drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
void fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
// Not working? Maybe wrong image format
void drawBitmap(int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint16_t color);

/* START:  From AdaFruit GFX */
void drawChar(int16_t x, int16_t y, unsigned char c, uint16_t color, uint16_t bg, uint8_t size);
void drawText(uint8_t x, uint8_t y, char *line, uint8_t color);
void drawTextCentered(uint8_t y, char *line, uint8_t color);
uint8_t textWidth(char *s);
uint8_t textHeight();
void setFont(tFonts font);
void setDisplayBuffer(uint8_t display_buf[][LCD_HORIZONTAL_MAX/8]);

/* END: From AdaFruit GFX */

//private:
void TA0_enableVCOMToggle();
void TA0_turnOff();
uint8_t _orientation;
uint8_t _reverse;

#endif
