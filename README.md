[![Build Status](https://travis-ci.org/DmitryKoterov/dom_varimport.png?branch=master)](https://travis-ci.org/DmitryKoterov/dom_varimport)

dom_varimport: a simple PHP extension to convert nested arrays into DOMDocument  
Dmitry Koterov, http://en.dklab.ru/lib/dom_varimport/  
License: PHP License 3.01


## Motivation

Some of projects use XSLT as a templating engine. To build XML for such engines,
we need a very fast and memory efficient way to convert PHP nested arrays and
objects into DOMDocument object. Also, produced XML should be straight-forward
and as simple as it could be (BTW XMLRPC, SOAP and other XML-based formats are quite
sloppy in this case). So dom_varimport has been introduced: it produces DOMDocument
from a nested array near 20 times faster than a hand-made code in native PHP
(1 MB XML with thousands of nodes could be generated in 1-2 ms).

## How to build

    phpize
    ./configure
    make
    make test
    make install  # or copy modules/dom_varimport.so manually
    phpize --clean

Then, add the following to your `php.ini` file:

    extension=dom_varimport.so

RPMs for Fedora, RHEL and CentOS are available in [remi](http://rpms.remirepo.net/) repository.

## Synopsis

The code:

    $doc = new DOMDocument();
    $doc->formatOutput = true;
    dom_varimport(
        $doc,
        array(
            "a" => 111,
            123,
            0.5,
            "arr" => array("1a" => "1a"),
            "obj" => (object)array("prop" => "val"),
            true,
            false,
            "b" => null,
            "empty" => array(),
        ),
        "root", // optional, defaults to "root"
        "item", // optional, defaults to "item", if NULL - invalid tag names are hidden
        "key",  // optional, defaults to "key", if NULL - no key attributes are inserted
        false   // optional, if true - show notices when keys are not valid XML elements
    );
    echo $doc->saveXML();

prints:

    <?xml version="1.0"?>
    <root>
      <a key="a">111</a>           <!-- plain key=value -->
      <item key="0">123</item>     <!-- numeric keys are "item" tags -->
      <item key="1">0.5</item>     <!-- double -->
      <arr key="arr">              <!-- nested array -->
        <item key="1a">1a</item>   <!-- invalid tag names are converted to "item" -->
      </arr>
      <obj key="obj">              <!-- nested object -->
        <prop key="prop">val</prop>
      </obj>
      <item key="2">1</item>       <!-- true converts to 1 -->
      <item key="3"/>              <!-- false converts to an empty string -->
      <b key="b"/>                 <!-- null also converts to an empty string -->
      <empty key="empty"/>         <!-- empty array is an empty element -->
    </root>
