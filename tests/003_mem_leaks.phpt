--TEST--
Check for memory leaks absense
--SKIPIF--
<?php require_once('skipif.inc'); ?>
--EXTENSIONS--
dom
--FILE--
<?php
require_once('init.inc');
$delta = $d = $i = 0;
$mem = memory_get_usage();
$delta = $d = 0;
for ($i = 0; $i < 8; $i++) {
    $doc = newDom();
    dom_varimport($doc, array("a" => 111, true, false, "b" => null, 123, 0.5, "arr" => array("1a" => "1a"), "obj" => (object)array("prop" => "val")));
    $d = memory_get_usage() - $mem;
    if ($i < 2) {
        $delta = $d + 0; // +0 to avoid aliasing
        printf("Iteration %d: initial (%d)\n", $i, $d);
    } else {
        if ($delta >= $d) {
            printf("Iteration %d: memory usage is stable (%d)\n", $i, $d);
        } else {
            printf("Iteration %d: memory usage increased (was %d at iteration %d, now %d)\n", $i, $delta, $i - 1, $d);
        }
    }
}
?>
--EXPECTF--
Iteration 0: initial (%d)
Iteration 1: initial (%d)
Iteration 2: memory usage is stable (%d)
Iteration 3: memory usage is stable (%d)
Iteration 4: memory usage is stable (%d)
Iteration 5: memory usage is stable (%d)
Iteration 6: memory usage is stable (%d)
Iteration 7: memory usage is stable (%d)
