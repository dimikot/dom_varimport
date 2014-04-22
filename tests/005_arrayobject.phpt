--TEST--
ArrayObject converting
--SKIPIF--
<?php require_once('skipif.inc'); ?>
--EXTENSIONS--
dom
--FILE--
<?php
require_once('init.inc');
$doc = newDom();
class C extends ArrayObject {}
$a = new C();
$a["a"] = "aaa";
dom_varimport($doc, $a);
dumpDom($doc);
?>
--EXPECTF--
<?xml version="1.0"?>
<root>
  <a key="a">aaa</a>
</root>
