<?php

//echo "Input value: " . $_REQUEST['tree'] . "\n \n";
$tree = escapeshellarg($_REQUEST['tree']);
$call = "/home/ivan/dev/buzz-ext/BoostGraphCentrality/build/forest_call $tree";

$return = -1;
exec( $call, $out, $return);
$out_len = count($out);
//echo "Return value: ". $out[$out_len - 1] . "\n";
//echo "Return result: $return\n";
echo $out[$out_len - 1];

?>

