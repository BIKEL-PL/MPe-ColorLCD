#ifndef UGUI_BAFANG_LCD
#define UGUI_BAFANG_LCD

#include "ugui.h"
    
/* *** Configuration. *** */
    
    /* Set here the name you have given to display CD and RESET signals in your schematic. */
//#define DISPLAY_RESET           GraphicLCDIntf_Write8(0, 0x01)
//#define DISPLAY_CD              DISP_CD
    
    /* Set here your display resolution. */
#ifdef USE_WITH_8X0C
#define DISPLAY_WIDTH           320
#define DISPLAY_HEIGHT          480
#endif    

#ifdef USE_WITH_560C
#define DISPLAY_WIDTH           240
#define DISPLAY_HEIGHT          376
#endif   

#define CONCATENATE(name, function)                 CONCAT(name, function)
#define CONCAT(name, function)                      name##function

//#define mmDisplayReset_Write(value) CONCATENATE(DISPLAY_RESET, _Write(value))
//#define mmDisplayCD_Write(value)    CONCATENATE(DISPLAY_CD, _Write(value))
    
/* *** Glogal GUI structure for this. *** */
extern UG_GUI ugui_lcd;

typedef enum
{
    //LCD_ILI9481 = 0,
   // LCD_ST7796,
   // LCD_Unknown
	LCD_Unknown = 0,
	  LCD_ILI9481,
	  LCD_ST7796,
} lcd_IC_t;

/* *** Function prototypes. *** */
void bafang_8X0C_lcd_init();
void bafang_560C_lcd_init();
void lcd_pixel_set(UG_S16 x, UG_S16 y, UG_COLOR c);
void lcd_window_set(unsigned int s_x,unsigned int e_x,unsigned int s_y,unsigned int e_y);
void lcd_write_command(uint16_t ui32_command);
void lcd_write_data_8bits(uint16_t ui32_data);
void lcd_write_color_9bits(uint16_t ui32_data);
uint32_t convertColorTo18bit(uint16_t color);
uint16_t* getLcdDevcode(void); // per 8.2.39 of datasheet, six words, first will be filled with garbage

    // Accelerators.
UG_RESULT HW_FillFrame(UG_S16 x1, UG_S16 y1, UG_S16 x2, UG_S16 y2, UG_COLOR c);
UG_RESULT HW_DrawLine(UG_S16 x1 , UG_S16 y1 , UG_S16 x2 , UG_S16 y2 , UG_COLOR c );
UG_RESULT HW_DrawImage(UG_S16 x1, UG_S16 y1, UG_S16 x2, UG_S16 y2, uint8_t *image, uint16_t pSize);

#endif

/* [] END OF FILE */

