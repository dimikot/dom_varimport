--TEST--
special key @attributes 
--SKIPIF--
<?php require_once('skipif.inc'); ?>
--EXTENSIONS--
dom
--FILE--
<?php
require_once('init.inc');
$doc = newDom();

dom_varimport(
    $doc,
    array("@attributes" => array(
        '1' => 'z',
        '1abc' => 'y',
        'a' => true, 'b' => 123, 'c' => 0.23, 'd' => 'test',
        'e' => array(), 'f' => new StdClass
    )),
    'root', 'item', 'key', true
);
dumpDom($doc); echo "\n";

class C
{
    private $priv;
    public $pub = 10;
}

dom_varimport(
    $doc,
    array("@attributes" => new C()),
    'root', 'item', 'key', true
);
dumpDom($doc); echo "\n";

dom_varimport(
    $doc,
    array("@attributes" => 1234),
    'root', 'item', 'key', true
);
dumpDom($doc); echo "\n";

?>
--EXPECTF--
Error[8]: dom_varimport(): Passed a numeric attribute name, skipping
Error[8]: dom_varimport(): Passed invalid attribute name, skipping
Error[8]: dom_varimport(): Invalid attribute value: array/resource/object, attribute: "e"
Error[8]: dom_varimport(): Invalid attribute value: array/resource/object, attribute: "f"
<?xml version="1.0"?>
<root a="true" b="123" c="0.23" d="test"/>

<?xml version="1.0"?>
<root pub="10"/>

Error[8]: dom_varimport(): Addtibutes must be specified as an array or object
<?xml version="1.0"?>
<root/>
