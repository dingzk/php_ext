--TEST--
Test fahrenheit_to_celsius
--SKIPIF--
<?php if (!extension_loaded("pib")) print "skip"; ?>
--FILE--
<?php
printf("%.2f", celsius_to_fahrenheit(70));
?>
--EXPECTF--
158.00
