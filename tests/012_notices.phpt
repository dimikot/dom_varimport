--TEST--
Notices on errors
--SKIPIF--
<?php require_once('skipif.inc'); ?>
--EXTENSIONS--
dom
--FILE--
<?php
require_once('init.inc');
$doc = newDom();

dom_varimport($doc, array("a" => 123, '-' => 456, 789), 'root', 'item', 'key', true);
dumpDom($doc); echo "\n";

dom_varimport($doc, array("a" => 123, '-' => 456, 789), 'root', null, 'key', true);
dumpDom($doc); echo "\n";

dom_varimport($doc, array("a" => opendir('.')), 'root', 'item', 'key', true);
dumpDom($doc); echo "\n";

?>
--EXPECTF--
Error[8]: dom_varimport(): Array key or object property "-" cannot be used as XML element name; "item" is used instead
Error[8]: dom_varimport(): Array key or object property "0" cannot be used as XML element name; "item" is used instead
<?xml version="1.0"?>
<root>
  <a key="a">123</a>
  <item key="-">456</item>
  <item key="0">789</item>
</root>

<?xml version="1.0"?>
<root>
  <a key="a">123</a>
</root>

Error[8]: dom_varimport(): Unsupported value type: resource, key: "a"
<?xml version="1.0"?>
<root>
  <a key="a"/>
</root>
