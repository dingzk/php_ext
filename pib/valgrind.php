<?php
printf("%.2f", celsius_to_fahrenheit(70));
printf("%.2f", fahrenheit_to_celsius(70));
$a = 70;
fahrenheit_to_celsius_ref($a);
printf("%.2f", $a);
?>
