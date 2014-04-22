--TEST--
Various data types
--SKIPIF--
<?php require_once('skipif.inc'); ?>
--EXTENSIONS--
dom
--FILE--
<?php
require_once('init.inc');
$doc = newDom();
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
    doubleval(123),
));
dumpDom($doc);
?>
--EXPECT--
<?xml version="1.0"?>
<root>
  <a key="a">111</a>
  <item key="0">123</item>
  <item key="1">0.5</item>
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
  <item key="4">123</item>
</root>
