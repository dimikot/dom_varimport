--TEST--
Recursion protection
--SKIPIF--
<?php require_once('skipif.inc'); ?>
--EXTENSIONS--
dom
--FILE--
<?php
require_once('init.inc');
$doc = newDom();
$a = array("a" => "aaa");
$a["b"] =& $a;
dom_varimport($doc, $a);
dumpDom($doc);
?>
--EXPECTF--
Error[2]: dom_varimport(): recursion detected
<?xml version="1.0"?>
<root>
  <a key="a">aaa</a>
  <b key="b">
    <a key="a">aaa</a>
    <b key="b"/>
  </b>
</root>
