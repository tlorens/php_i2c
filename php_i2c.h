
#ifndef PHP_I2C_H
#define PHP_I2C_H

extern zend_module_entry i2c_module_entry;
#define phpext_i2c_ptr &i2c_module_entry

#define PHP_I2C_VERSION "0.1.0" /* Replace with version number for your extension */

#ifdef PHP_WIN32
#	define PHP_I2C_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#	define PHP_I2C_API __attribute__ ((visibility("default")))
#else
#	define PHP_I2C_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

PHP_MINIT_FUNCTION(i2c);
PHP_MSHUTDOWN_FUNCTION(i2c);
PHP_RINIT_FUNCTION(i2c);
PHP_RSHUTDOWN_FUNCTION(i2c);
PHP_MINFO_FUNCTION(i2c);

PHP_METHOD(i2c, __construct);
#if (PHP_MAJOR_VERSION >= 5)
ZEND_BEGIN_ARG_INFO_EX(i2c____construct_args, ZEND_SEND_BY_VAL, ZEND_RETURN_VALUE, 1)
  ZEND_ARG_INFO(0, bus)
  ZEND_ARG_INFO(0, addr)
#if (PHP_MINOR_VERSION > 0)
  ZEND_ARG_ARRAY_INFO(0, options, 1)
#else
  ZEND_ARG_INFO(0, options)
#endif
ZEND_END_ARG_INFO()
#else /* PHP 4.x */
#define i2c____construct_args NULL
#endif

PHP_METHOD(i2c, __destruct);
#if (PHP_MAJOR_VERSION >= 5)
  ZEND_BEGIN_ARG_INFO_EX(i2c____destruct_args, ZEND_SEND_BY_VAL, ZEND_RETURN_VALUE, 0)
  ZEND_END_ARG_INFO()
#else /* PHP 4.x */
#define i2c____destruct_args NULL
#endif


PHP_METHOD(i2c, write_quick);
#if (PHP_MAJOR_VERSION >= 5)
  ZEND_BEGIN_ARG_INFO_EX(i2c____write_quick_args, ZEND_SEND_BY_VAL, ZEND_RETURN_VALUE, 0)
  ZEND_END_ARG_INFO()
#else /* PHP 4.x */
#define i2c____write_quick_args NULL
#endif

PHP_METHOD(i2c, read_byte);
#if (PHP_MAJOR_VERSION >= 5)
  ZEND_BEGIN_ARG_INFO_EX(i2c____read_byte_args, ZEND_SEND_BY_VAL, ZEND_RETURN_VALUE, 0)
  ZEND_END_ARG_INFO()
#else /* PHP 4.x */
  #define i2c____read_byte_args NULL
#endif

PHP_METHOD(i2c, write_byte);
#if (PHP_MAJOR_VERSION >= 5)
  ZEND_BEGIN_ARG_INFO_EX(i2c____write_byte_args, ZEND_SEND_BY_VAL, ZEND_RETURN_VALUE, 0)
  ZEND_END_ARG_INFO()
#else /* PHP 4.x */
#define i2c____write_byte_args NULL
#endif

PHP_METHOD(i2c, read_byte_data);
#if (PHP_MAJOR_VERSION >= 5)
  ZEND_BEGIN_ARG_INFO_EX(i2c____read_byte_data_args, ZEND_SEND_BY_VAL, ZEND_RETURN_VALUE, 0)
  ZEND_END_ARG_INFO()
#else /* PHP 4.x */
  #define i2c____read_byte_data_args NULL
#endif

PHP_METHOD(i2c, write_block_data);
#if (PHP_MAJOR_VERSION >= 5)
  ZEND_BEGIN_ARG_INFO_EX(i2c____write_block_data_args, ZEND_SEND_BY_VAL, ZEND_RETURN_VALUE, 0)
  ZEND_END_ARG_INFO()
#else /* PHP 4.x */
  #define i2c____write_block_data_args NULL
#endif

















PHP_METHOD(i2c, write_cmd);
#if (PHP_MAJOR_VERSION >= 5)
  ZEND_BEGIN_ARG_INFO_EX(i2c____write_cmd_args, ZEND_SEND_BY_VAL, ZEND_RETURN_VALUE, 0)
  ZEND_END_ARG_INFO()
#else /* PHP 4.x */
#define i2c____write_cmd_args NULL
#endif


PHP_METHOD(i2c, println);
#if (PHP_MAJOR_VERSION >= 5)
  ZEND_BEGIN_ARG_INFO_EX(i2c____println_args, ZEND_SEND_BY_VAL, ZEND_RETURN_VALUE, 0)
  ZEND_END_ARG_INFO()
#else /* PHP 4.x */
#define i2c____println_args NULL
#endif



/*
  	Declare any global variables you may need between the BEGIN
	and END macros here:

ZEND_BEGIN_MODULE_GLOBALS(i2c)
	long  global_value;
	char *global_string;
ZEND_END_MODULE_GLOBALS(i2c)
*/

/* In every utility function you add that needs to use variables
   in php_i2c_globals, call TSRMLS_FETCH(); after declaring other
   variables used by that function, or better yet, pass in TSRMLS_CC
   after the last function argument and declare your utility function
   with TSRMLS_DC after the last declared argument.  Always refer to
   the globals in your function as I2C_G(variable).  You are
   encouraged to rename these macros something shorter, see
   examples in any other php module directory.
*/

#ifdef ZTS
#define I2C_G(v) TSRMG(i2c_globals_id, zend_i2c_globals *, v)
#else
#define I2C_G(v) (i2c_globals.v)
#endif

#endif	/* PHP_I2C_H */

