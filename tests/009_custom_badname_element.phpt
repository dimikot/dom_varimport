--TEST--
Custom "bad name" element
--SKIPIF--
<?php require_once('skipif.inc'); ?>
--EXTENSIONS--
dom
--FILE--
<?php
require_once('init.inc');
$doc = newDom();

dom_varimport($doc, array("a" => 123, "-" => 456, 789), 'root', 'one');
dumpDom($doc); echo "\n";

dom_varimport($doc, array("a" => 123, "-" => 456, 789), 'root', null);
dumpDom($doc); echo "\n";

dom_varimport($doc, array("a" => 123, "-" => 456, 789), 'root', '');
dumpDom($doc); echo "\n";

?>
--EXPECTF--
<?xml version="1.0"?>
<root>
  <a key="a">123</a>
  <one key="-">456</one>
  <one key="0">789</one>
</root>

<?xml version="1.0"?>
<root>
  <a key="a">123</a>
</root>

Error[2]: dom_varimport(): Passed invalid badname_element ""; default value "item" is used instead
<?xml version="1.0"?>
<root>
  <a key="a">123</a>
  <item key="-">456</item>
  <item key="0">789</item>
</root>
