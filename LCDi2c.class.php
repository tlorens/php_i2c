<?PHP
/**
 * i2C PHP Library
 * @category Interface Integration
 * @package I2C
 * @author Timothy Lorens <tlorens@cyberdyne.org>
 * @license [<url>] mMIT
 * @link http://icebrkr.me iCE Breakers Matrix
 */
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
    define('LCD_BACKLIGHT', 0x08);
    define('LCD_NOBACKLIGHT', 0x00);

    define('EN', 1<<2);
    define('RW', 1<<1);
    define('RS', 1<<0);

/**
 *  i2c Classs
 * @category Interface
 * @package I2C
 * @author Timothy Lorens <tlorens@cyberdyne.org>
 * @license [<url>] mMIT
 * @link http://icebrkr.me iCE Breakers Matrix
 */
class LCDi2c {
    private $_i2c     = null;
    private $_bus     = null;
    private $_address = null;

    public function __construct($bus, $address) {
        $this->_bus     = $bus;
        $this->_address = $address;
        $this->_i2c     = new i2c($this->_bus, $this->_address);

        $this->writeCmd(0x03);
        $this->writeCmd(0x03);
        $this->writeCmd(0x03);
        $this->writeCmd(0x02);

    }

    public function writeString($line, $string) {
        switch($line) {
            case 1:
                $this->writeCmd(0x80);
                break;
            case 2:
                $this->writeCmd(0xC0);
                break;
            case 3:
                $this->writeCmd(0x94);
                break;
            case 4:
                $this->writeCmd(0xD4);
                break;
        }

        for ($x = 0; $x < strlen($string); $x++) {
            $this->LCDWrite(ord($string[$x]), RS);
        }
    }

    public function writeCmd($cmd) {
        $this->_i2c->write_cmd($cmd, $this->_address);
    }

    private function strobe($value) {
        $this->_i2c->write_byte($value | LCD_BACKLIGHT);
        $this->_i2c->write_byte($value | EN | LCD_BACKLIGHT);
        usleep(1);
        $this->_i2c->write_byte($value | LCD_BACKLIGHT);
    }

    private function write_four_bits($data) {
        $this->_i2c->write_cmd($data | LCD_BACKLIGHT, $this->_address);
        $this->strobe($data);
    }

    public function write_char($char) {
        $this->strobe(($char >> 4) << 4);
        usleep(5000);
        $this->strobe(($char & 0xF) << 4);
        usleep(1);
    }

    private function LCDWrite($cmd, $mode = 0) {
        $this->write_four_bits($mode | ($cmd & 0xF0));
        $this->write_four_bits($mode | (($cmd << 4) & 0xF0));
    }

}
