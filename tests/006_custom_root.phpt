--TEST--
Custom root element name
--SKIPIF--
<?php require_once('skipif.inc'); ?>
--EXTENSIONS--
dom
--FILE--
<?php
require_once('init.inc');
$doc = newDom();
dom_varimport($doc, array("a" => 123), 'data');
dumpDom($doc);
?>
--EXPECTF--
<?xml version="1.0"?>
<data>
  <a key="a">123</a>
</data>
