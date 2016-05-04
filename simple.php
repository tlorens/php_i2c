<?PHP
    require('LCDi2c.class.php');

    define('ADDRESS', 0x27);
    define('BUS', 1);

    $lcd = new LCDi2c(BUS, ADDRESS);
    //$lcd->write_char('A');
    $lcd->writeString(1, 'Hello from PHP');
