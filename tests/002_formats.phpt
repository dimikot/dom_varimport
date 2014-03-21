--TEST--
Various data types
--SKIPIF--
<?php require_once('skipif.inc'); ?>
--FILE--
<?php
$doc = new DOMDocument();
$doc->formatOutput = true;
dom_varimport($doc, array(
    "a" => 111,
    123,
    0.5,
    "arr" => array("1a" => "1a"),
    "obj" => (object)array("prop" => "val"),
    true,
    false,
    "b" => null,
    "empty" => array(),
));
echo $doc->saveXML();
?>
--EXPECT--
<?xml version="1.0"?>
<root>
  <a key="a">111</a>
  <item key="0">123</item>
  <item key="1">0.500000</item>
  <arr key="arr">
    <item key="1a">1a</item>
  </arr>
  <obj key="obj">
    <prop key="prop">val</prop>
  </obj>
  <item key="2">1</item>
  <item key="3"/>
  <b key="b"/>
  <empty key="empty"/>
</root>
