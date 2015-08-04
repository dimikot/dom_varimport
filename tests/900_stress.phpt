--TEST--
Stress test
--SKIPIF--
<?php require_once('skipif.inc'); ?>
--EXTENSIONS--
dom
--FILE--
<?php
$tmpl = array(
    str_repeat("key", 1024) => 123456789,
    123456789,
    str_repeat("value", 1024),
    0.987654,
    true,
    false,
    "key" => null,
    "empty" => array(),
);

$longData = makeArray(5, $tmpl);
$mems = array_fill(0, 40, round(1));
$mem0 = memory_get_usage();
for ($i = 0; $i < count($mems); $i++) {
    $doc = new DOMDocument();
    $doc->formatOutput = true;
    dom_varimport($doc, $longData);
    $doc->saveXML();
    $mem1 = memory_get_usage(true);
    $mems[$i] = round(($mem1 - $mem0) * 1.0);
}

for ($i = count($mems) - 1 - 5; $i < count($mems) - 1; $i++) {
    if ($mems[$i] != $mems[count($mems) - 1]) {
        echo "Memory usage increased at " . ($i + 1) . "'th iteration. Memory usages in bytes: ";
        print_r($mems);
    }
}

function makeArray($depth, $tmpl)
{
    if (!$depth) return array();
    $data = unserialize(serialize($tmpl)); // data clone (break refs)
    for ($i = 0; $i < $depth; $i++) {
        $data["arr$i"] = makeArray($depth - 1, $tmpl);
        $data["obj$i"] = new ArrayObject(makeArray($depth - 1, $tmpl));
    }
    return $data;
}
?>
--EXPECTF--
