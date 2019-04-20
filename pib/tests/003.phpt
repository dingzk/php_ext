--TEST--
Test fahrenheit_to_celsius
--SKIPIF--
<?php if (!extension_loaded("pib")) print "skip"; ?>
--FILE--
<?php
printf("%.2f", fahrenheit_to_celsius(70));
?>
--EXPECTF--
21.11
