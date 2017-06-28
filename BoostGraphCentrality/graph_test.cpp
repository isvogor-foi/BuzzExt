/*
 * FindCenter.cpp
 *
 *  Created on: 2017-05-26
 *      Author: ivan
 */

#include <iostream>
#include "GraphOperations.h"

using namespace std;
using namespace graph_buzz;

int main(int argc, char *argv[])
{
	//std::string s = "<?xml version='1.0' encoding='UTF-8'?><graphml xmlns='http://graphml.graphdrawing.org/xmlns' xmlns:xsi='http://www.w3.org/2001/XMLSchema-instance' xsi:schemaLocation='http://graphml.graphdrawing.org/xmlns http://graphml.graphdrawing.org/xmlns/1.0/graphml.xsd'><key id='d0' for='node' attr.name='vertex_name' attr.type='string'/><graph id='G' edgedefault='undirected'><node id='n0'></node><node id='n1'></node><node id='n2'></node><node id='n3'></node><node id='n100'></node><edge id='e0' source='n0' target='n1'></edge><edge id='e1' source='n0' target='n2'></edge><edge id='e2' source='n0' target='n100'></edge><edge id='e3' source='n1' target='n0'></edge><edge id='e4' source='n1' target='n2'></edge><edge id='e5' source='n1' target='n3'></edge><edge id='e6' source='n1' target='n100'></edge><edge id='e7' source='n2' target='n0'></edge><edge id='e8' source='n2' target='n1'></edge><edge id='e9' source='n2' target='n3'></edge><edge id='e10' source='n2' target='n100'></edge><edge id='e11' source='n3' target='n1'></edge><edge id='e12' source='n3' target='n2'></edge><edge id='e13' source='n100' target='n0'></edge><edge id='e14' source='n100' target='n1'></edge><edge id='e15' source='n100' target='n2'></edge></graph></graphml>";
	//std::string s = "<?xml version='1.0' encoding='UTF-8'?> <graphml xmlns='http://graphml.graphdrawing.org/xmlns' xmlns:xsi='http://www.w3.org/2001/XMLSchema-instance' xsi:schemaLocation='http://graphml.graphdrawing.org/xmlns http://graphml.graphdrawing.org/xmlns/1.0/graphml.xsd'><key id='d0' for='node' attr.name='vertex_name' attr.type='string'/><graph id='G' edgedefault='undirected'><node id='n0'><data key='d0'>A</data> </node><node id='n1'><data key='d0'>B</data> </node><node id='n2'><data key='d0'>C</data> </node><node id='n3'><data key='d0'>D</data> </node><node id='n100'><data key='d0'>E</data> </node><edge id='e0' source='n0' target='n1' /><edge id='e1' source='n0' target='n2' /><edge id='e2' source='n0' target='n100' /><edge id='e3' source='n1' target='n0' /><edge id='e4' source='n1' target='n2' /><edge id='e5' source='n1' target='n3' /><edge id='e6' source='n1' target='n100' /><edge id='e7' source='n2' target='n0' /><edge id='e8' source='n2' target='n1' /><edge id='e9' source='n2' target='n3' /><edge id='e10' source='n2' target='n100' /><edge id='e11' source='n3' target='n1' /><edge id='e12' source='n3' target='n2' /><edge id='e13' source='n100' target='n0' /><edge id='e14' source='n100' target='n1' /><edge id='e15' source='n100' target='n2' /></graph></graphml>";
	//std::string s = "<?xml version='1.0' encoding='UTF-8'?> <graphml xmlns='http://graphml.graphdrawing.org/xmlns' xmlns:xsi='http://www.w3.org/2001/XMLSchema-instance' xsi:schemaLocation='http://graphml.graphdrawing.org/xmlns http://graphml.graphdrawing.org/xmlns/1.0/graphml.xsd'> <!-- property keys --> <graph id='G' edgedefault='undirected' parse.nodeids='canonical' parse.edgeids='canonical' parse.order='nodesfirst'> <!-- graph properties --> <!-- vertices --> <node id='n0'> </node> <node id='n1'> </node> <node id='n2'> </node> <node id='n3'> </node> <node id='n4'> </node> <node id='n5'> </node> <node id='n6'> </node> <node id='n7'> </node> <node id='n8'> </node> <node id='n9'> </node> <node id='n10'> </node> <node id='n11'> </node> <node id='n12'> </node> <node id='n13'> </node> <node id='n14'> </node> <node id='n15'> </node> <node id='n16'> </node> <node id='n17'> </node> <node id='n18'> </node> <node id='n19'> </node> <node id='n20'> </node> <node id='n21'> </node> <node id='n22'> </node> <node id='n23'> </node> <node id='n24'> </node> <node id='n25'> </node> <node id='n26'> </node> <node id='n27'> </node> <node id='n28'> </node> <node id='n29'> </node> <!-- edges --> <edge id='e0' source='n0' target='n23'> </edge> <edge id='e1' source='n0' target='n2'> </edge> <edge id='e2' source='n0' target='n5'> </edge> <edge id='e3' source='n0' target='n3'> </edge> <edge id='e4' source='n1' target='n20'> </edge> <edge id='e5' source='n1' target='n14'> </edge> <edge id='e6' source='n1' target='n11'> </edge> <edge id='e7' source='n1' target='n5'> </edge> <edge id='e8' source='n1' target='n6'> </edge> <edge id='e9' source='n1' target='n18'> </edge> <edge id='e10' source='n2' target='n23'> </edge> <edge id='e11' source='n2' target='n3'> </edge> <edge id='e12' source='n2' target='n15'> </edge> <edge id='e13' source='n2' target='n10'> </edge> <edge id='e14' source='n3' target='n15'> </edge> <edge id='e15' source='n3' target='n11'> </edge> <edge id='e16' source='n3' target='n5'> </edge> <edge id='e17' source='n4' target='n29'> </edge> <edge id='e18' source='n4' target='n25'> </edge> <edge id='e19' source='n4' target='n23'> </edge> <edge id='e20' source='n4' target='n22'> </edge> <edge id='e21' source='n5' target='n29'> </edge> <edge id='e22' source='n5' target='n23'> </edge> <edge id='e23' source='n5' target='n26'> </edge> <edge id='e24' source='n5' target='n6'> </edge> <edge id='e25' source='n5' target='n11'> </edge> <edge id='e26' source='n6' target='n26'> </edge> <edge id='e27' source='n6' target='n21'> </edge> <edge id='e28' source='n6' target='n18'> </edge> <edge id='e29' source='n7' target='n19'> </edge> <edge id='e30' source='n7' target='n18'> </edge> <edge id='e31' source='n7' target='n12'> </edge> <edge id='e32' source='n7' target='n14'> </edge> <edge id='e33' source='n8' target='n28'> </edge> <edge id='e34' source='n8' target='n15'> </edge> <edge id='e35' source='n8' target='n17'> </edge> <edge id='e36' source='n8' target='n10'> </edge> <edge id='e37' source='n9' target='n29'> </edge> <edge id='e38' source='n9' target='n26'> </edge> <edge id='e39' source='n9' target='n25'> </edge> <edge id='e40' source='n9' target='n24'> </edge> <edge id='e41' source='n9' target='n21'> </edge> <edge id='e42' source='n10' target='n15'> </edge> <edge id='e43' source='n11' target='n28'> </edge> <edge id='e44' source='n11' target='n20'> </edge> <edge id='e45' source='n11' target='n15'> </edge> <edge id='e46' source='n12' target='n27'> </edge> <edge id='e47' source='n12' target='n19'> </edge> <edge id='e48' source='n12' target='n16'> </edge> <edge id='e49' source='n12' target='n14'> </edge> <edge id='e50' source='n13' target='n27'> </edge> <edge id='e51' source='n13' target='n24'> </edge> <edge id='e52' source='n13' target='n19'> </edge> <edge id='e53' source='n13' target='n18'> </edge> <edge id='e54' source='n13' target='n21'> </edge> <edge id='e55' source='n14' target='n28'> </edge> <edge id='e56' source='n14' target='n17'> </edge> <edge id='e57' source='n14' target='n20'> </edge> <edge id='e58' source='n14' target='n18'> </edge> <edge id='e59' source='n14' target='n16'> </edge> <edge id='e60' source='n15' target='n28'> </edge> <edge id='e61' source='n16' target='n27'> </edge> <edge id='e62' source='n16' target='n17'> </edge> <edge id='e63' source='n17' target='n28'> </edge> <edge id='e64' source='n17' target='n27'> </edge> <edge id='e65' source='n18' target='n21'> </edge> <edge id='e66' source='n18' target='n19'> </edge> <edge id='e67' source='n19' target='n27'> </edge> <edge id='e68' source='n20' target='n28'> </edge> <edge id='e69' source='n21' target='n26'> </edge> <edge id='e70' source='n21' target='n24'> </edge> <edge id='e71' source='n22' target='n25'> </edge> <edge id='e72' source='n22' target='n23'> </edge> <edge id='e73' source='n23' target='n29'> </edge> <edge id='e74' source='n24' target='n27'> </edge> <edge id='e75' source='n24' target='n25'> </edge> <edge id='e76' source='n25' target='n29'> </edge> <edge id='e77' source='n26' target='n29'> </edge> </graph> </graphml> ";
	//std::string s = "<?xml version='1.0' encoding='UTF-8'?><graphml xmlns='http://graphml.graphdrawing.org/xmlns' xmlns:xsi='http://www.w3.org/2001/XMLSchema-instance' xsi:schemaLocation='http://graphml.graphdrawing.org/xmlns http://graphml.graphdrawing.org/xmlns/1.0/graphml.xsd'> <key id='d0' for='node' attr.name='vertex_name' attr.type='string'/> <graph id='G' edgedefault='undirected'><node id='n0'><data key='d0'>0</data> </node><node id='n1'><data key='d0'>1</data> </node><node id='n2'><data key='d0'>2</data> </node><node id='n3'><data key='d0'>3</data> </node><node id='n4'><data key='d0'>4</data> </node><node id='n5'><data key='d0'>5</data> </node><node id='n6'><data key='d0'>6</data> </node><node id='n7'><data key='d0'>7</data> </node><node id='n8'><data key='d0'>8</data> </node><node id='n9'><data key='d0'>9</data> </node><node id='n10'><data key='d0'>10</data> </node><node id='n11'><data key='d0'>11</data> </node><node id='n12'><data key='d0'>12</data> </node><node id='n13'><data key='d0'>13</data> </node><node id='n14'><data key='d0'>14</data> </node><node id='n15'><data key='d0'>15</data> </node><node id='n16'><data key='d0'>16</data> </node><node id='n17'><data key='d0'>17</data> </node><node id='n18'><data key='d0'>18</data> </node><node id='n19'><data key='d0'>19</data> </node><edge id='e0' source='n0' target='n4'></edge><edge id='e1' source='n0' target='n6'></edge><edge id='e2' source='n0' target='n10'></edge><edge id='e3' source='n0' target='n7'></edge><edge id='e4' source='n0' target='n8'></edge><edge id='e5' source='n0' target='n9'></edge><edge id='e6' source='n0' target='n12'></edge><edge id='e7' source='n0' target='n13'></edge><edge id='e8' source='n0' target='n17'></edge><edge id='e9' source='n0' target='n15'></edge><edge id='e10' source='n1' target='n6'></edge><edge id='e11' source='n1' target='n9'></edge><edge id='e12' source='n1' target='n11'></edge><edge id='e13' source='n1' target='n13'></edge><edge id='e14' source='n1' target='n15'></edge><edge id='e15' source='n1' target='n16'></edge><edge id='e16' source='n1' target='n18'></edge><edge id='e17' source='n1' target='n19'></edge><edge id='e18' source='n2' target='n4'></edge><edge id='e19' source='n2' target='n5'></edge><edge id='e20' source='n2' target='n6'></edge><edge id='e21' source='n2' target='n10'></edge><edge id='e22' source='n2' target='n8'></edge><edge id='e23' source='n2' target='n11'></edge><edge id='e24' source='n2' target='n12'></edge><edge id='e25' source='n3' target='n5'></edge><edge id='e26' source='n3' target='n11'></edge><edge id='e27' source='n3' target='n13'></edge><edge id='e28' source='n3' target='n18'></edge><edge id='e29' source='n3' target='n19'></edge><edge id='e30' source='n4' target='n0'></edge><edge id='e31' source='n4' target='n2'></edge><edge id='e32' source='n4' target='n5'></edge><edge id='e33' source='n4' target='n6'></edge><edge id='e34' source='n4' target='n10'></edge><edge id='e35' source='n4' target='n8'></edge><edge id='e36' source='n4' target='n9'></edge><edge id='e37' source='n4' target='n12'></edge><edge id='e38' source='n4' target='n13'></edge><edge id='e39' source='n4' target='n14'></edge><edge id='e40' source='n5' target='n3'></edge><edge id='e41' source='n5' target='n2'></edge><edge id='e42' source='n5' target='n4'></edge><edge id='e43' source='n5' target='n8'></edge><edge id='e44' source='n5' target='n11'></edge><edge id='e45' source='n5' target='n13'></edge><edge id='e46' source='n6' target='n0'></edge><edge id='e47' source='n6' target='n1'></edge><edge id='e48' source='n6' target='n2'></edge><edge id='e49' source='n6' target='n4'></edge><edge id='e50' source='n6' target='n10'></edge><edge id='e51' source='n6' target='n8'></edge><edge id='e52' source='n6' target='n9'></edge><edge id='e53' source='n6' target='n11'></edge><edge id='e54' source='n6' target='n12'></edge><edge id='e55' source='n6' target='n13'></edge><edge id='e56' source='n6' target='n17'></edge><edge id='e57' source='n6' target='n15'></edge><edge id='e58' source='n7' target='n0'></edge><edge id='e59' source='n7' target='n10'></edge><edge id='e60' source='n7' target='n12'></edge><edge id='e61' source='n7' target='n14'></edge><edge id='e62' source='n7' target='n17'></edge><edge id='e63' source='n8' target='n0'></edge><edge id='e64' source='n8' target='n2'></edge><edge id='e65' source='n8' target='n4'></edge><edge id='e66' source='n8' target='n5'></edge><edge id='e67' source='n8' target='n6'></edge><edge id='e68' source='n8' target='n10'></edge><edge id='e69' source='n8' target='n9'></edge><edge id='e70' source='n8' target='n11'></edge><edge id='e71' source='n8' target='n12'></edge><edge id='e72' source='n8' target='n13'></edge><edge id='e73' source='n8' target='n19'></edge><edge id='e74' source='n9' target='n0'></edge><edge id='e75' source='n9' target='n1'></edge><edge id='e76' source='n9' target='n4'></edge><edge id='e77' source='n9' target='n6'></edge><edge id='e78' source='n9' target='n8'></edge><edge id='e79' source='n9' target='n13'></edge><edge id='e80' source='n9' target='n17'></edge><edge id='e81' source='n9' target='n15'></edge><edge id='e82' source='n9' target='n16'></edge><edge id='e83' source='n9' target='n19'></edge><edge id='e84' source='n10' target='n0'></edge><edge id='e85' source='n10' target='n2'></edge><edge id='e86' source='n10' target='n4'></edge><edge id='e87' source='n10' target='n6'></edge><edge id='e88' source='n10' target='n7'></edge><edge id='e89' source='n10' target='n8'></edge><edge id='e90' source='n10' target='n12'></edge><edge id='e91' source='n10' target='n14'></edge><edge id='e92' source='n10' target='n17'></edge><edge id='e93' source='n11' target='n3'></edge><edge id='e94' source='n11' target='n1'></edge><edge id='e95' source='n11' target='n2'></edge><edge id='e96' source='n11' target='n5'></edge><edge id='e97' source='n11' target='n6'></edge><edge id='e98' source='n11' target='n8'></edge><edge id='e99' source='n11' target='n13'></edge><edge id='e100' source='n11' target='n19'></edge><edge id='e101' source='n12' target='n0'></edge><edge id='e102' source='n12' target='n2'></edge><edge id='e103' source='n12' target='n4'></edge><edge id='e104' source='n12' target='n6'></edge><edge id='e105' source='n12' target='n10'></edge><edge id='e106' source='n12' target='n7'></edge><edge id='e107' source='n12' target='n8'></edge><edge id='e108' source='n12' target='n14'></edge><edge id='e109' source='n13' target='n0'></edge><edge id='e110' source='n13' target='n3'></edge><edge id='e111' source='n13' target='n1'></edge><edge id='e112' source='n13' target='n4'></edge><edge id='e113' source='n13' target='n5'></edge><edge id='e114' source='n13' target='n6'></edge><edge id='e115' source='n13' target='n8'></edge><edge id='e116' source='n13' target='n9'></edge><edge id='e117' source='n13' target='n11'></edge><edge id='e118' source='n13' target='n16'></edge><edge id='e119' source='n13' target='n18'></edge><edge id='e120' source='n13' target='n19'></edge><edge id='e121' source='n14' target='n4'></edge><edge id='e122' source='n14' target='n10'></edge><edge id='e123' source='n14' target='n7'></edge><edge id='e124' source='n14' target='n12'></edge><edge id='e125' source='n15' target='n0'></edge><edge id='e126' source='n15' target='n1'></edge><edge id='e127' source='n15' target='n6'></edge><edge id='e128' source='n15' target='n9'></edge><edge id='e129' source='n15' target='n17'></edge><edge id='e130' source='n15' target='n16'></edge><edge id='e131' source='n16' target='n1'></edge><edge id='e132' source='n16' target='n9'></edge><edge id='e133' source='n16' target='n13'></edge><edge id='e134' source='n16' target='n15'></edge><edge id='e135' source='n16' target='n18'></edge><edge id='e136' source='n17' target='n0'></edge><edge id='e137' source='n17' target='n6'></edge><edge id='e138' source='n17' target='n10'></edge><edge id='e139' source='n17' target='n7'></edge><edge id='e140' source='n17' target='n9'></edge><edge id='e141' source='n17' target='n15'></edge><edge id='e142' source='n18' target='n3'></edge><edge id='e143' source='n18' target='n1'></edge><edge id='e144' source='n18' target='n13'></edge><edge id='e145' source='n18' target='n16'></edge><edge id='e146' source='n18' target='n19'></edge><edge id='e147' source='n19' target='n3'></edge><edge id='e148' source='n19' target='n1'></edge><edge id='e149' source='n19' target='n8'></edge><edge id='e150' source='n19' target='n9'></edge><edge id='e151' source='n19' target='n11'></edge><edge id='e152' source='n19' target='n13'></edge><edge id='e153' source='n19' target='n18'></edge></graph></graphml>";
	std::string s = "<?xml version='1.0' encoding='UTF-8'?><graphml xmlns='http://graphml.graphdrawing.org/xmlns' xmlns:xsi='http://www.w3.org/2001/XMLSchema-instance' xsi:schemaLocation='http://graphml.graphdrawing.org/xmlns http://graphml.graphdrawing.org/xmlns/1.0/graphml.xsd'> <key id='d0' for='node' attr.name='vertex_name' attr.type='string'/> <graph id='G' edgedefault='undirected'><node id='n0'><data key='d0'>0</data> </node><node id='n1'><data key='d0'>1</data> </node><node id='n2'><data key='d0'>2</data> </node><node id='n3'><data key='d0'>3</data> </node><node id='n4'><data key='d0'>4</data> </node><node id='n5'><data key='d0'>5</data> </node><node id='n6'><data key='d0'>6</data> </node><node id='n7'><data key='d0'>7</data> </node><node id='n8'><data key='d0'>8</data> </node><node id='n9'><data key='d0'>9</data> </node><node id='n10'><data key='d0'>10</data> </node><node id='n11'><data key='d0'>11</data> </node><node id='n12'><data key='d0'>12</data> </node><node id='n13'><data key='d0'>13</data> </node><node id='n14'><data key='d0'>14</data> </node><node id='n15'><data key='d0'>15</data> </node><node id='n16'><data key='d0'>16</data> </node><node id='n17'><data key='d0'>17</data> </node><node id='n18'><data key='d0'>18</data> </node><node id='n19'><data key='d0'>19</data> </node><node id='n20'><data key='d0'>20</data> </node><node id='n21'><data key='d0'>21</data> </node><node id='n22'><data key='d0'>22</data> </node><node id='n23'><data key='d0'>23</data> </node><node id='n24'><data key='d0'>24</data> </node><node id='n25'><data key='d0'>25</data> </node><node id='n26'><data key='d0'>26</data> </node><node id='n27'><data key='d0'>27</data> </node><node id='n28'><data key='d0'>28</data> </node><node id='n29'><data key='d0'>29</data> </node><edge id='e0' source='n0' target='n10'></edge><edge id='e1' source='n0' target='n7'></edge><edge id='e2' source='n0' target='n17'></edge><edge id='e3' source='n0' target='n27'></edge><edge id='e4' source='n1' target='n9'></edge><edge id='e5' source='n1' target='n13'></edge><edge id='e6' source='n1' target='n16'></edge><edge id='e7' source='n1' target='n24'></edge><edge id='e8' source='n1' target='n28'></edge><edge id='e9' source='n1' target='n29'></edge><edge id='e10' source='n2' target='n5'></edge><edge id='e11' source='n2' target='n8'></edge><edge id='e12' source='n2' target='n11'></edge><edge id='e13' source='n2' target='n12'></edge><edge id='e14' source='n2' target='n23'></edge><edge id='e15' source='n2' target='n26'></edge><edge id='e16' source='n3' target='n19'></edge><edge id='e17' source='n4' target='n6'></edge><edge id='e18' source='n4' target='n10'></edge><edge id='e19' source='n4' target='n12'></edge><edge id='e20' source='n4' target='n14'></edge><edge id='e21' source='n4' target='n22'></edge><edge id='e22' source='n4' target='n23'></edge><edge id='e23' source='n5' target='n2'></edge><edge id='e24' source='n5' target='n11'></edge><edge id='e25' source='n5' target='n26'></edge><edge id='e26' source='n6' target='n4'></edge><edge id='e27' source='n6' target='n10'></edge><edge id='e28' source='n6' target='n9'></edge><edge id='e29' source='n6' target='n23'></edge><edge id='e30' source='n6' target='n27'></edge><edge id='e31' source='n7' target='n0'></edge><edge id='e32' source='n7' target='n10'></edge><edge id='e33' source='n7' target='n14'></edge><edge id='e34' source='n8' target='n2'></edge><edge id='e35' source='n8' target='n11'></edge><edge id='e36' source='n8' target='n13'></edge><edge id='e37' source='n8' target='n19'></edge><edge id='e38' source='n8' target='n23'></edge><edge id='e39' source='n9' target='n1'></edge><edge id='e40' source='n9' target='n6'></edge><edge id='e41' source='n9' target='n13'></edge><edge id='e42' source='n9' target='n23'></edge><edge id='e43' source='n9' target='n24'></edge><edge id='e44' source='n9' target='n27'></edge><edge id='e45' source='n10' target='n0'></edge><edge id='e46' source='n10' target='n4'></edge><edge id='e47' source='n10' target='n6'></edge><edge id='e48' source='n10' target='n7'></edge><edge id='e49' source='n10' target='n14'></edge><edge id='e50' source='n10' target='n27'></edge><edge id='e51' source='n11' target='n2'></edge><edge id='e52' source='n11' target='n5'></edge><edge id='e53' source='n11' target='n8'></edge><edge id='e54' source='n12' target='n2'></edge><edge id='e55' source='n12' target='n4'></edge><edge id='e56' source='n12' target='n21'></edge><edge id='e57' source='n12' target='n23'></edge><edge id='e58' source='n12' target='n26'></edge><edge id='e59' source='n13' target='n1'></edge><edge id='e60' source='n13' target='n8'></edge><edge id='e61' source='n13' target='n9'></edge><edge id='e62' source='n13' target='n19'></edge><edge id='e63' source='n13' target='n28'></edge><edge id='e64' source='n14' target='n4'></edge><edge id='e65' source='n14' target='n10'></edge><edge id='e66' source='n14' target='n7'></edge><edge id='e67' source='n14' target='n22'></edge><edge id='e68' source='n15' target='n29'></edge><edge id='e69' source='n16' target='n1'></edge><edge id='e70' source='n16' target='n18'></edge><edge id='e71' source='n16' target='n25'></edge><edge id='e72' source='n16' target='n28'></edge><edge id='e73' source='n16' target='n29'></edge><edge id='e74' source='n17' target='n0'></edge><edge id='e75' source='n17' target='n20'></edge><edge id='e76' source='n17' target='n24'></edge><edge id='e77' source='n17' target='n27'></edge><edge id='e78' source='n18' target='n16'></edge><edge id='e79' source='n18' target='n28'></edge><edge id='e80' source='n19' target='n3'></edge><edge id='e81' source='n19' target='n8'></edge><edge id='e82' source='n19' target='n13'></edge><edge id='e83' source='n19' target='n28'></edge><edge id='e84' source='n20' target='n17'></edge><edge id='e85' source='n21' target='n12'></edge><edge id='e86' source='n21' target='n22'></edge><edge id='e87' source='n21' target='n26'></edge><edge id='e88' source='n22' target='n4'></edge><edge id='e89' source='n22' target='n14'></edge><edge id='e90' source='n22' target='n21'></edge><edge id='e91' source='n23' target='n2'></edge><edge id='e92' source='n23' target='n4'></edge><edge id='e93' source='n23' target='n6'></edge><edge id='e94' source='n23' target='n8'></edge><edge id='e95' source='n23' target='n9'></edge><edge id='e96' source='n23' target='n12'></edge><edge id='e97' source='n24' target='n1'></edge><edge id='e98' source='n24' target='n9'></edge><edge id='e99' source='n24' target='n17'></edge><edge id='e100' source='n24' target='n27'></edge><edge id='e101' source='n24' target='n29'></edge><edge id='e102' source='n25' target='n16'></edge><edge id='e103' source='n25' target='n29'></edge><edge id='e104' source='n26' target='n2'></edge><edge id='e105' source='n26' target='n5'></edge><edge id='e106' source='n26' target='n12'></edge><edge id='e107' source='n26' target='n21'></edge><edge id='e108' source='n27' target='n0'></edge><edge id='e109' source='n27' target='n6'></edge><edge id='e110' source='n27' target='n10'></edge><edge id='e111' source='n27' target='n9'></edge><edge id='e112' source='n27' target='n17'></edge><edge id='e113' source='n27' target='n24'></edge><edge id='e114' source='n28' target='n1'></edge><edge id='e115' source='n28' target='n13'></edge><edge id='e116' source='n28' target='n16'></edge><edge id='e117' source='n28' target='n18'></edge><edge id='e118' source='n28' target='n19'></edge><edge id='e119' source='n29' target='n1'></edge><edge id='e120' source='n29' target='n15'></edge><edge id='e121' source='n29' target='n16'></edge><edge id='e122' source='n29' target='n24'></edge><edge id='e123' source='n29' target='n25'></edge></graph></graphml>";
	GraphOperations go;
	std::string result = go.CreateTree(s);
	std::cout << "Result (T): " << result << std::endl;
	return 0;

}
