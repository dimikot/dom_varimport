--TEST--
ArrayObject converting
--SKIPIF--
<?php require_once('skipif.inc'); ?>
--FILE--
<?php
$doc = new DOMDocument();
$doc->formatOutput = true;
class C extends ArrayObject {}
$a = new C();
$a["a"] = "aaa";
dom_varimport($doc, $a);
echo $doc->saveXML();
?>
--EXPECTF--
<?xml version="1.0"?>
<root>
  <a key="a">aaa</a>
</root>
