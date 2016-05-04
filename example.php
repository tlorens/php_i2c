<?PHP
//# LCD Address
define('ADDRESS', 0x27);

//# commands
define('LCD_CLEARDISPLAY', 0x01);
define('LCD_RETURNHOME', 0x02);
define('LCD_ENTRYMODESET', 0x04);
define('LCD_DISPLAYCONTROL', 0x08);
define('LCD_CURSORSHIFT', 0x10);
define('LCD_FUNCTIONSET', 0x20);
define('LCD_SETCGRAMADDR', 0x40);
define('LCD_SETDDRAMADDR', 0x80);

//# flags for display entry mode
define('LCD_ENTRYRIGHT', 0x00);
define('LCD_ENTRYLEFT', 0x02);
define('LCD_ENTRYSHIFTINCREMENT', 0x01);
define('LCD_ENTRYSHIFTDECREMENT', 0x00);

//# flags for display on/off control
define('LCD_DISPLAYON', 0x04);
define('LCD_DISPLAYOFF', 0x00);
define('LCD_CURSORON', 0x02);
define('LCD_CURSOROFF', 0x00);
define('LCD_BLINKON', 0x01);
define('LCD_BLINKOFF', 0x00);

//# flags for display/cursor shift
define('LCD_DISPLAYMOVE', 0x08);
define('LCD_CURSORMOVE', 0x00);
define('LCD_MOVERIGHT', 0x04);
define('LCD_MOVELEFT', 0x00);

//# flags for function set
define('LCD_8BITMODE', 0x10);
define('LCD_4BITMODE', 0x00);
define('LCD_2LINE', 0x08);
define('LCD_1LINE', 0x00);
define('LCD_5x10DOTS', 0x04);
define('LCD_5x8DOTS', 0x00);

//# flags for backlight control
define('LCD_BACKLIGHT', 0x08);
define('LCD_NOBACKLIGHT', 0x00);

define('En', 0b00000100); //# Enable bit
define('Rw', 0b00000010); //# Read/Write bit
define('Rs', 0b00000001); //# Register select bit


function lcdWrite($cmd, $mode=0)
{
  i2c_write_four_bits($mode | ($cmd & 0xF0));
  //i2c_write_four_bits($mode | (($cmd << 4) & 0xF0));
}

   # clocks EN to latch command
function strobe($data) 
{
      i2c_write_cmd($data | En | LCD_BACKLIGHT);
      sleep(.0005);
      i2c_write_cmd((($data & ~En) | LCD_BACKLIGHT));
      sleep(.0001);
}

function i2c_write_four_bits($data) 
{
      i2c_write_cmd(LCD_BACKLIGHT);  
}


i2c_init(1, ADDRESS);
//i2c_write_cmd(LCD_BACKLIGHT);
