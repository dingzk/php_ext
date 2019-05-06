/*
  +----------------------------------------------------------------------+
  | PHP Version 7                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2017 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author:                                                              |
  +----------------------------------------------------------------------+
*/

/* $Id$ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_pib.h"

#define TEMP_CONVERTER_TO_FAHRENHEIT 2
#define TEMP_CONVERTER_TO_CELSIUS 1

ZEND_BEGIN_MODULE_GLOBALS(pib)
    zend_long rnd;
    zend_ulong cur_score;
    zval scores;
ZEND_END_MODULE_GLOBALS(pib)

ZEND_DECLARE_MODULE_GLOBALS(pib)

/* True global resources - no need for thread safety here */
static int le_pib;

/* {{{ PHP_INI
 */
/* Remove comments and fill if you need to have entries in php.ini
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("pib.global_value",      "42", PHP_INI_ALL, OnUpdateLong, global_value, zend_pib_globals, pib_globals)
    STD_PHP_INI_ENTRY("pib.global_string", "foobar", PHP_INI_ALL, OnUpdateString, global_string, zend_pib_globals, pib_globals)
PHP_INI_END()
*/
/* }}} */

static void pib_rnd_init(void)
{
    PIB_G(cur_score) = 0;
    php_random_int(0, 100, &PIB_G(rnd), 0);
}

PHP_GINIT_FUNCTION(pib)
{
    //ZEND_SECURE_ZERO(zend_pib_globals, sizeof(*zend_pib_globals));
    ZEND_SECURE_ZERO(pib_globals, sizeof(*pib_globals));
}

ZEND_BEGIN_ARG_INFO_EX(arginfo_guess, 0, 0, 1)
    ZEND_ARG_INFO(0, num)
ZEND_END_ARG_INFO()

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(pib)
{

    REGISTER_LONG_CONSTANT("TEMP_CONVERTER_TO_CELSIUS", TEMP_CONVERTER_TO_CELSIUS, CONST_CS|CONST_PERSISTENT);
    REGISTER_LONG_CONSTANT("TEMP_CONVERTER_TO_FAHRENHEIT", TEMP_CONVERTER_TO_FAHRENHEIT, CONST_CS|CONST_PERSISTENT);


	/* If you have INI entries, uncomment these lines
	REGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(pib)
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
PHP_RINIT_FUNCTION(pib)
{
#if defined(COMPILE_DL_PIB) && defined(ZTS)
	ZEND_TSRMLS_CACHE_UPDATE();
#endif
    array_init(&PIB_G(scores));
    pib_rnd_init();

	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(pib)
{
    zval_dtor(&PIB_G(scores));

	return SUCCESS;
}
/* }}} */

PHP_FUNCTION(pib_guess)
{
    zend_long r;
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "l", &r) == FAILURE) {
        return;
    }

    if (r == PIB_G(rnd)) {
        add_next_index_long(&PIB_G(scores), PIB_G(cur_score));
        pib_rnd_init();
        RETURN_TRUE;
    }
    PIB_G(cur_score) ++;
    if (r < PIB_G(rnd)) {
        RETURN_STRING("more");
    }

    RETURN_STRING("less");
}

PHP_FUNCTION(pib_get_scores)
{
    if (zend_parse_parameters_none() == FAILURE) {
        return;
    }
    RETVAL_ZVAL(&PIB_G(scores), 1, 0);
}

PHP_FUNCTION(pib_reset)
{
    if (zend_parse_parameters_none() == FAILURE) {
        return;
    }
    pib_rnd_init();
}


/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(pib)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "pib support", "enabled");
	php_info_print_table_end();

	/* Remove comments if you have entries in php.ini
	DISPLAY_INI_ENTRIES();
	*/
}
/* }}} */

/* {{{ pib_functions[]
 *
 * Every user visible function must have an entry in pib_functions[].
 */
const zend_function_entry pib_functions[] = {
    PHP_FE(pib_reset, NULL)
    PHP_FE(pib_get_scores, NULL)
    PHP_FE(pib_guess, arginfo_guess)
	PHP_FE_END	/* Must be the last line in pib_functions[] */
};
/* }}} */

/* {{{ pib_module_entry
 */
zend_module_entry pib_module_entry = {
	STANDARD_MODULE_HEADER,
	"pib",
	pib_functions,
	PHP_MINIT(pib),
	PHP_MSHUTDOWN(pib),
	PHP_RINIT(pib),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(pib),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(pib),
	PHP_PIB_VERSION,
    PHP_MODULE_GLOBALS(pib),
    PHP_GINIT(pib),
    NULL,
    NULL,
	STANDARD_MODULE_PROPERTIES_EX
};
/* }}} */

#ifdef COMPILE_DL_PIB
#ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
#endif
ZEND_GET_MODULE(pib)
#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
