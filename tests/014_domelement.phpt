--TEST--
populate DOMElement
--SKIPIF--
<?php require_once('skipif.inc'); ?>
--EXTENSIONS--
dom
--FILE--
<?php
require_once('init.inc');
$doc = newDom();

$root = $doc->createElement('root');
$element = $doc->createElement('element');

$doc->appendChild($root);
$root->appendChild($element);

dom_varimport($element, array('a' => 'a', 'b' => 123, 'c' => 0.23), 'root', 'item', 'key', true);
dumpDom($doc); echo "\n";

?>
--EXPECTF--
<?xml version="1.0"?>
<root>
  <element>
    <a key="a">a</a>
    <b key="b">123</b>
    <c key="c">0.23</c>
  </element>
</root>

