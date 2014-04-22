--TEST--
Check version
--SKIPIF--
<?php require_once('skipif.inc'); ?>
--EXTENSIONS--
dom
--FILE--
<?php
require_once('init.inc');
$refl = new ReflectionExtension('dom_varimport');
$extVersion = $refl->getVersion();
$readmeVersion = preg_match('/^Version:\s*([\d.]+)/m', file_get_contents(dirname(__FILE__) . '/../README.txt'), $m)? $m[1] : 'not found';

echo($extVersion === $readmeVersion? 'OK' : "Mismatch: $extVersion != $readmeVersion");
?>
--EXPECTF--
OK
