--TEST--
Test fahrenheit_to_celsius
--SKIPIF--
<?php if (!extension_loaded("pib")) print "skip"; ?>
--FILE--
<?php
$a = 70;
fahrenheit_to_celsius_ref($a);
printf("%.2f", $a);
?>
--EXPECTF--
21.11
