--TEST--
Warnings
--SKIPIF--
<?php require_once('skipif.inc'); ?>
--FILE--
<?php
require_once('init.inc');
$doc = newDom();

dom_varimport($doc, array("a" => 123), '-', 'item', 'key');
dumpDom($doc); echo "\n";

dom_varimport($doc, array("a" => 123), 'root', '-', 'key');
dumpDom($doc); echo "\n";

dom_varimport($doc, array("a" => 123), 'root', 'item', '-');
dumpDom($doc); echo "\n";

?>
--EXPECTF--
Error[2]: dom_varimport(): Passed invalid root_element "-"; default value "root" is used instead
<?xml version="1.0"?>
<root>
  <a key="a">123</a>
</root>

Error[2]: dom_varimport(): Passed invalid badname_element "-"; default value "item" is used instead
<?xml version="1.0"?>
<root>
  <a key="a">123</a>
</root>

Error[2]: dom_varimport(): Passed invalid key_attr "-"; default value "key" is used instead
<?xml version="1.0"?>
<root>
  <a key="a">123</a>
</root>
