
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_i2c.h"

#include <linux/i2c-dev.h>
#include <linux/types.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

/* If you declare any globals in php_i2c.h uncomment this:
ZEND_DECLARE_MODULE_GLOBALS(i2c)
*/

/* True global resources - no need for thread safety here */
static int le_i2c;

/* {{{ i2c_functions[]
 *
 * Every user visible function must have an entry in i2c_functions[].
 */
const zend_function_entry i2c_functions[] = {
	PHP_FE_END	/* Must be the last line in i2c_functions[] */
};
/* }}} */

/* {{{ i2c_functions[]
 *
 * Every user visible function must have an entry in i2c_functions[].
 */
static zend_function_entry i2c_methods[] = {
    PHP_ME(i2c, __construct, i2c____construct_args, /**/ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
    PHP_ME(i2c, __destruct, i2c____destruct_args, /**/ZEND_ACC_PUBLIC | ZEND_ACC_DTOR)
    PHP_ME(i2c, write_quick, i2c____write_byte_args, /**/ZEND_ACC_PUBLIC | ZEND_ACC_DTOR)
    PHP_ME(i2c, read_byte, i2c____read_byte_args, /**/ZEND_ACC_PUBLIC | ZEND_ACC_DTOR)
    PHP_ME(i2c, write_byte, i2c____write_byte_args, /**/ZEND_ACC_PUBLIC | ZEND_ACC_DTOR)
    PHP_ME(i2c, read_byte_data, i2c____read_byte_data_args, /**/ZEND_ACC_PUBLIC | ZEND_ACC_DTOR)
    PHP_ME(i2c, write_block_data, i2c____write_block_data_args, /**/ZEND_ACC_PUBLIC | ZEND_ACC_DTOR)


    PHP_ME(i2c, write_cmd, i2c____write_cmd_args, /**/ZEND_ACC_PUBLIC | ZEND_ACC_DTOR)
    PHP_ME(i2c, println, i2c____println_args, /**/ZEND_ACC_PUBLIC | ZEND_ACC_DTOR)
  	PHP_FE_END
};
/* }}} */

/* {{{ i2c_module_entry
 */
zend_module_entry i2c_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	"i2c",
	i2c_functions,
	PHP_MINIT(i2c),
	PHP_MSHUTDOWN(i2c),
	PHP_RINIT(i2c),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(i2c),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(i2c),
#if ZEND_MODULE_API_NO >= 20010901
	PHP_I2C_VERSION,
#endif
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_I2C
ZEND_GET_MODULE(i2c)
#endif

/* {{{ PHP_INI
 */
// PHP_INI_BEGIN()
//     STD_PHP_INI_ENTRY("i2c.bus_path", "/dev/i2c-", PHP_INI_ALL, OnUpdateString, bus_path, zend_i2c_globals, i2c_globals)
// PHP_INI_END()
/* }}} */

/* {{{ php_i2c_init_globals
 */
// static void php_i2c_init_globals(zend_i2c_globals *i2c_globals)
// {
// 	i2c_globals->bus_path = "/dev/i2c-";
// }
/* }}} */

zend_class_entry *i2c_ce;

/* {{{ proto void __construct(int bus, int chipselect[, array options])
   */
PHP_METHOD(i2c, __construct)
{
    char bus_path[20];
    long bus;
    int addr;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ll", &bus, &addr) == FAILURE) {
        WRONG_PARAM_COUNT;
        RETURN_FALSE;
    }

    snprintf(bus_path, 19, "/dev/i2c-%ld", bus);
    long fd = open(bus_path, O_RDWR);
    if (fd < 0) {
        php_printf("Unable to open: %s\n\n", bus_path);
        RETURN_FALSE;
    }

    if (ioctl(fd, I2C_SLAVE, addr) < 0) {
        php_printf("Unable to controll: %x\n\n", addr);
        RETURN_FALSE;
    }

    zend_update_property_long(Z_OBJCE_P(getThis()), getThis(), ZEND_STRL("fd"), fd TSRMLS_CC);
    zend_update_property_long(Z_OBJCE_P(getThis()), getThis(), ZEND_STRL("addr"), addr TSRMLS_CC);

}
/* }}} __construct */

/* {{{ proto void __destruct()
   */
PHP_METHOD(i2c, __destruct)
{
    zend_class_entry * _this_ce;
    zval * _this_zval = NULL;

    if (zend_parse_method_parameters(ZEND_NUM_ARGS() TSRMLS_CC, getThis(), "O", &_this_zval, i2c_ce) == FAILURE) {
        return;
    }

    _this_ce = Z_OBJCE_P(_this_zval);

    int fd = Z_LVAL_P(zend_read_property(_this_ce, _this_zval, "fd", 2, 0 TSRMLS_CC));
    close(fd);
}
/* }}} __destruct */

/* {{{ proto void i2c_smbus_write_quick()
   */
PHP_METHOD(i2c, write_quick)
{
    int data;

	__s32 res;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &data) == FAILURE) {
        WRONG_PARAM_COUNT;
        RETURN_FALSE;
    }

	zval *zfile = zend_read_property(Z_OBJCE_P(getThis()), getThis(), ZEND_STRL("fd"), 0 TSRMLS_CC);
	int fd = Z_LVAL_P(zfile);

	res = i2c_smbus_write_quick(fd, data);

	if (res < 0 ) {
		RETURN_FALSE;
	}

	RETURN_TRUE;
}
/* }}} i2c_smbus_write_quick */

/* {{{ proto void i2c_smbus_write_quick()
   */
PHP_METHOD(i2c, read_byte)
{
	__s32 res;

	zval *zfile = zend_read_property(Z_OBJCE_P(getThis()), getThis(), ZEND_STRL("fd"), 0 TSRMLS_CC);
	int fd = Z_LVAL_P(zfile);

	res = i2c_smbus_read_byte(fd);
	RETVAL_LONG(res);
}
/* }}} i2c_smbus_write_quick */


/* {{{ proto void write_cmd()
   */
PHP_METHOD(i2c, write_byte)
{
    long data;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &data) == FAILURE) {
        WRONG_PARAM_COUNT;
        RETURN_FALSE;
    }

	zval *zfile = zend_read_property(Z_OBJCE_P(getThis()), getThis(), ZEND_STRL("fd"), 0 TSRMLS_CC);
	int fd = Z_LVAL_P(zfile);

	__s32 res = i2c_smbus_write_byte(fd, data);

	if (res < 0 ) {
		RETURN_FALSE;
	}

	RETURN_TRUE;
}
/* }}} __destruct */


/* {{{ proto void write_cmd()
   */
PHP_METHOD(i2c, read_byte_data)
{
    long data;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &data) == FAILURE) {
        WRONG_PARAM_COUNT;
        RETURN_FALSE;
    }

	zval *zfile = zend_read_property(Z_OBJCE_P(getThis()), getThis(), ZEND_STRL("fd"), 0 TSRMLS_CC);
	int fd = Z_LVAL_P(zfile);

	__s32 res = i2c_smbus_read_byte_data(fd, data);

	RETURN_LONG(res)
}
/* }}} __destruct */

/* {{{ proto void write_cmd()
   */
PHP_METHOD(i2c, write_byte_data)
{
    long cmd, data;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ll", &cmd, &data) == FAILURE) {
        WRONG_PARAM_COUNT;
        RETURN_FALSE;
    }

	zval *zfile = zend_read_property(Z_OBJCE_P(getThis()), getThis(), ZEND_STRL("fd"), 0 TSRMLS_CC);
	int fd = Z_LVAL_P(zfile);

	__s32 res = i2c_smbus_write_byte_data(fd, cmd, data);

	if (res < 0 ) {
		RETURN_FALSE;
	}

	RETURN_TRUE;
}
/* }}} __destruct */

/* {{{ proto void write_cmd()
   */
PHP_METHOD(i2c, write_block_data)
{
    long cmd;
    char *data;
    int length;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "lss", &cmd, &length, &data) == FAILURE) {
        WRONG_PARAM_COUNT;
        RETURN_FALSE;
    }

	zval *zfile = zend_read_property(Z_OBJCE_P(getThis()), getThis(), ZEND_STRL("fd"), 0 TSRMLS_CC);
	int fd = Z_LVAL_P(zfile);

	__s32 res = i2c_smbus_write_block_data(fd, cmd, length, data);

	if (res < 0 ) {
		RETURN_FALSE;
	}

	RETURN_TRUE;
}
/* }}} __destruct */









/* {{{ proto void write_cmd()
   */
PHP_METHOD(i2c, write_cmd)
{
    long int cmd, addr;

	__s32 res;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ll", &cmd, &addr) == FAILURE) {
        WRONG_PARAM_COUNT;
        RETURN_FALSE;
    }

	zval *zfile = zend_read_property(Z_OBJCE_P(getThis()), getThis(), ZEND_STRL("fd"), 0 TSRMLS_CC);
	int fd = Z_LVAL_P(zfile);

	res = i2c_smbus_write_byte_data(fd, addr, cmd);

	if (res < 0 ) {
		RETURN_FALSE;
	}

	RETURN_TRUE;
}
/* }}} __destruct */



/* {{{ proto void println()
   */
PHP_METHOD(i2c, println)
{
    long int addr;
    int length;
    char *string = NULL;

	__s32 res;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ls", &string, &length, &addr) == FAILURE) {
        WRONG_PARAM_COUNT;
        RETURN_FALSE;
    }

	zval *zfile = zend_read_property(Z_OBJCE_P(getThis()), getThis(), ZEND_STRL("fd"), 0 TSRMLS_CC);
	int fd = Z_LVAL_P(zfile);

	if (write(fd, string, sizeof(string)) != sizeof(string)) {
		RETURN_FALSE;
	}
	RETURN_TRUE;
}
/* }}} println */


/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(i2c)
{
	//REGISTER_INI_ENTRIES();
	zend_class_entry i2c;

	INIT_CLASS_ENTRY(i2c, "i2c", i2c_functions);
	INIT_CLASS_ENTRY(i2c, "i2c", i2c_methods);

	i2c_ce = zend_register_internal_class_ex(&i2c, NULL, NULL TSRMLS_CC);

	zend_declare_property_null(i2c_ce, ZEND_STRL("fd"), ZEND_ACC_PUBLIC TSRMLS_CC);
	zend_declare_property_null(i2c_ce, ZEND_STRL("addr"), ZEND_ACC_PUBLIC TSRMLS_CC);
	//zend_declare_property_null(i2c_ce, ZEND_STRL("_file"), ZEND_ACC_PRIVATE TSRMLS_CC);

	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(i2c)
{
	/* uncomment this line if you have INI entries
	UNREGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(i2c)
{
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(i2c)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(i2c)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "i2c support", "enabled");
	//php_info_print_table_row(2, "i2c Version", "1.0");
	//php_info_print_table_row(2, "Enable_Functions", "open,read,readblock,write,writeblock");
	php_info_print_table_end();

	DISPLAY_INI_ENTRIES();
}
/* }}} */

/* {{{ proto string i2c_init(string arg)
   Write command with arguments */
PHP_FUNCTION(i2c_write_cmd_arg)
{

}
/* }}} */

/* {{{ proto string i2c_init(string arg)
   Write block of data*/
PHP_FUNCTION(i2c_write_block_data)
{
}
/* }}} */

/* {{{ proto string i2c_init(string arg)
   Write command with arguments */
PHP_FUNCTION(i2c_read)
{
	long int addr,file;
	zval *zfile;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &addr) == FAILURE) {
		WRONG_PARAM_COUNT;
		RETURN_FALSE;
	}

	__s32 res;

	zfile = zend_read_property(Z_OBJCE_P(getThis()), getThis(), ZEND_STRL("_fd"), 0 TSRMLS_CC);
	file = Z_LVAL_P(zfile);

	res = i2c_smbus_read_byte_data(file, addr);

	if (res < 0) {
		RETURN_FALSE;
		//RETURN_LONG(file);
	}

	RETURN_LONG(res);

}
/* }}} */

/* {{{ proto string i2c_init(string arg)
   Write block of data*/
PHP_FUNCTION(i2c_read_data)
{
}
/* }}} */

/* {{{ proto string i2c_init(string arg)
   Write command with arguments */
PHP_FUNCTION(i2c_read_block_data)
{
}
/* }}} */




/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
