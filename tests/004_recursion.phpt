--TEST--
Recursion protection
--SKIPIF--
<?php require_once('skipif.inc'); ?>
--FILE--
<?php
$doc = new DOMDocument();
$doc->formatOutput = true;
$a = array("a" => "aaa");
$a["b"] =& $a;
dom_varimport($doc, $a);
echo $doc->saveXML();
?>
--EXPECTF--
Warning: dom_varimport(): recursion detected in %s
<?xml version="1.0"?>
<root>
  <a key="a">aaa</a>
  <b key="b">
    <a key="a">aaa</a>
    <b key="b"/>
  </b>
</root>
