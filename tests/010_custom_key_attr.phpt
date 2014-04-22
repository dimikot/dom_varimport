--TEST--
Custom key attribute name
--SKIPIF--
<?php require_once('skipif.inc'); ?>
--FILE--
<?php
require_once('init.inc');
$doc = newDom();

dom_varimport($doc, array("a" => 123, "-" => 456, 789), 'root', 'item', 'k');
dumpDom($doc); echo "\n";

dom_varimport($doc, array("a" => 123, "-" => 456, 789), 'root', 'item', null);
dumpDom($doc); echo "\n";

dom_varimport($doc, array("a" => 123, "-" => 456, 789), 'root', 'item', '---');
dumpDom($doc); echo "\n";

dom_varimport($doc, array("a" => 123, "-" => 456, 789), 'root', null, null);
dumpDom($doc); echo "\n";

?>
--EXPECTF--
<?xml version="1.0"?>
<root>
  <a k="a">123</a>
  <item k="-">456</item>
  <item k="0">789</item>
</root>

<?xml version="1.0"?>
<root>
  <a>123</a>
  <item>456</item>
  <item>789</item>
</root>

Error[2]: dom_varimport(): Passed invalid key_attr "---"; default value "key" is used instead
<?xml version="1.0"?>
<root>
  <a key="a">123</a>
  <item key="-">456</item>
  <item key="0">789</item>
</root>

<?xml version="1.0"?>
<root>
  <a>123</a>
</root>
