dom_varimport: a simple PHP extension to convert nested arrays into DOMDocument
Version: 1.02
Dmitry Koterov, http://en.dklab.ru/lib/dom_varimport/
License: PHP License 3.01


MOTIVATION
----------

Some of projects use XSLT as a templating engine. To build XML for such engines,
we need a very fast and memory efficient way to convert PHP nested arrays and
objects into DOMDocument object. Also, produced XML should be straight-forward
and as simple as it could be (BTW XMLRPC, SOAP etc. are very bad in this case).


HOW TO BUILD
------------

phpize
./configure
make
make test
make install  # or copy modules/dom_varimport.so manually
phpize --clean


SYNOPSIS
--------

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
      "root" // optional, defaults to "root"
  );
  echo $doc->saveXML();

prints:

  <?xml version="1.0"?>
  <root>
    <a key="a">111</a>               <!-- plain key=value -->
    <item key="0">123</item>         <!-- numeric keys are "item" tags -->
    <item key="1">0.5</item>         <!-- double -->
    <arr key="arr">                  <!-- nested array -->
      <item key="1a">1a</item>         <!-- invalid tag names are converted to "item" -->
    </arr>
    <obj key="obj">                  <!-- nested object -->
      <prop key="prop">val</prop>
    </obj>
    <item key="2">1</item>           <!-- true converts to 1 -->
    <item key="3"/>                  <!-- false converts to an empty string -->
    <b key="b"/>                     <!-- null also converts to an empty string -->
    <empty key="empty"/>             <!-- empty array is an empty element -->
  </root>
