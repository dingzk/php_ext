ZEND_DONT_UNLOAD_MODULES=1 USE_ZEND_ALLOC=0 valgrind --leak-check=full --show-reachable=yes --track-origins=yes /usr/local/php7/bin/php valgrind.php 
