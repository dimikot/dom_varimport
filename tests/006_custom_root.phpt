--TEST--
Custom root element name
--SKIPIF--
<?php require_once('skipif.inc'); ?>
--FILE--
<?php
$doc = new DOMDocument();
$doc->formatOutput = true;
dom_varimport($doc, array("a" => 123), 'data');
echo $doc->saveXML();
?>
--EXPECTF--
<?xml version="1.0"?>
<data>
  <a key="a">123</a>
</data>
