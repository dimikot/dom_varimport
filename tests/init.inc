<?php
function newDom()
{
    return new DOMDocument();
}

function dumpDom(DOMDocument $doc)
{
    $doc->formatOutput = true;
    echo $doc->saveXML();
}

function onError($errno, $errstr)
{
    echo "Error[$errno]: $errstr\n";
}

set_error_handler('onError');

?>
