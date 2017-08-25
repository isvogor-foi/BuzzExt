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

int main ( int argc, char *argv[] )
{

    std::string s = "<?xml version='1.0' encoding='UTF-8'?><graphml xmlns='http://graphml.graphdrawing.org/xmlns' xmlns:xsi='http://www.w3.org/2001/XMLSchema-instance' xsi:schemaLocation='http://graphml.graphdrawing.org/xmlns http://graphml.graphdrawing.org/xmlns/1.0/graphml.xsd'> <key id='d0' for='node' attr.name='vertex_name' attr.type='string'/> <graph id='G' edgedefault='undirected'><node id='n0'><data key='d0'>0</data> </node><node id='n1'><data key='d0'>1</data> </node><node id='n2'><data key='d0'>2</data> </node><node id='n3'><data key='d0'>3</data> </node><node id='n4'><data key='d0'>4</data> </node><node id='n5'><data key='d0'>5</data> </node><node id='n6'><data key='d0'>6</data> </node><node id='n7'><data key='d0'>7</data> </node><node id='n8'><data key='d0'>8</data> </node><node id='n9'><data key='d0'>9</data> </node><node id='n10'><data key='d0'>10</data> </node><node id='n11'><data key='d0'>11</data> </node><node id='n12'><data key='d0'>12</data> </node><node id='n13'><data key='d0'>13</data> </node><node id='n14'><data key='d0'>14</data> </node><node id='n15'><data key='d0'>15</data> </node><node id='n16'><data key='d0'>16</data> </node><node id='n17'><data key='d0'>17</data> </node><node id='n18'><data key='d0'>18</data> </node><node id='n19'><data key='d0'>19</data> </node><node id='n20'><data key='d0'>20</data> </node><node id='n21'><data key='d0'>21</data> </node><node id='n22'><data key='d0'>22</data> </node><node id='n23'><data key='d0'>23</data> </node><node id='n24'><data key='d0'>24</data> </node><node id='n25'><data key='d0'>25</data> </node><node id='n26'><data key='d0'>26</data> </node><node id='n27'><data key='d0'>27</data> </node><node id='n28'><data key='d0'>28</data> </node><node id='n29'><data key='d0'>29</data> </node><node id='n30'><data key='d0'>30</data> </node><node id='n31'><data key='d0'>31</data> </node><node id='n32'><data key='d0'>32</data> </node><node id='n33'><data key='d0'>33</data> </node><node id='n34'><data key='d0'>34</data> </node><node id='n35'><data key='d0'>35</data> </node><node id='n36'><data key='d0'>36</data> </node><node id='n37'><data key='d0'>37</data> </node><node id='n38'><data key='d0'>38</data> </node><node id='n39'><data key='d0'>39</data> </node><edge id='e0' source='n0' target='n21'></edge><edge id='e1' source='n0' target='n22'></edge><edge id='e2' source='n0' target='n12'></edge><edge id='e3' source='n0' target='n13'></edge><edge id='e4' source='n1' target='n3'></edge><edge id='e5' source='n1' target='n34'></edge><edge id='e6' source='n1' target='n16'></edge><edge id='e7' source='n1' target='n17'></edge><edge id='e8' source='n1' target='n37'></edge><edge id='e9' source='n1' target='n19'></edge><edge id='e10' source='n2' target='n7'></edge><edge id='e11' source='n2' target='n8'></edge><edge id='e12' source='n2' target='n28'></edge><edge id='e13' source='n2' target='n30'></edge><edge id='e14' source='n2' target='n33'></edge><edge id='e15' source='n3' target='n1'></edge><edge id='e16' source='n3' target='n23'></edge><edge id='e17' source='n3' target='n10'></edge><edge id='e18' source='n3' target='n31'></edge><edge id='e19' source='n3' target='n34'></edge><edge id='e20' source='n3' target='n17'></edge><edge id='e21' source='n3' target='n39'></edge><edge id='e22' source='n4' target='n27'></edge><edge id='e23' source='n4' target='n30'></edge><edge id='e24' source='n5' target='n20'></edge><edge id='e25' source='n5' target='n35'></edge><edge id='e26' source='n5' target='n15'></edge><edge id='e27' source='n5' target='n17'></edge><edge id='e28' source='n5' target='n37'></edge><edge id='e29' source='n6' target='n22'></edge><edge id='e30' source='n6' target='n23'></edge><edge id='e31' source='n6' target='n26'></edge><edge id='e32' source='n6' target='n29'></edge><edge id='e33' source='n6' target='n12'></edge><edge id='e34' source='n6' target='n32'></edge><edge id='e35' source='n7' target='n2'></edge><edge id='e36' source='n7' target='n24'></edge><edge id='e37' source='n8' target='n2'></edge><edge id='e38' source='n8' target='n28'></edge><edge id='e39' source='n8' target='n31'></edge><edge id='e40' source='n8' target='n33'></edge><edge id='e41' source='n8' target='n34'></edge><edge id='e42' source='n8' target='n19'></edge><edge id='e43' source='n9' target='n21'></edge><edge id='e44' source='n9' target='n25'></edge><edge id='e45' source='n9' target='n10'></edge><edge id='e46' source='n9' target='n31'></edge><edge id='e47' source='n9' target='n33'></edge><edge id='e48' source='n9' target='n14'></edge><edge id='e49' source='n10' target='n21'></edge><edge id='e50' source='n10' target='n22'></edge><edge id='e51' source='n10' target='n3'></edge><edge id='e52' source='n10' target='n23'></edge><edge id='e53' source='n10' target='n9'></edge><edge id='e54' source='n10' target='n31'></edge><edge id='e55' source='n11' target='n20'></edge><edge id='e56' source='n11' target='n16'></edge><edge id='e57' source='n11' target='n37'></edge><edge id='e58' source='n12' target='n0'></edge><edge id='e59' source='n12' target='n22'></edge><edge id='e60' source='n12' target='n6'></edge><edge id='e61' source='n12' target='n29'></edge><edge id='e62' source='n13' target='n0'></edge><edge id='e63' source='n13' target='n21'></edge><edge id='e64' source='n13' target='n14'></edge><edge id='e65' source='n13' target='n36'></edge><edge id='e66' source='n14' target='n21'></edge><edge id='e67' source='n14' target='n25'></edge><edge id='e68' source='n14' target='n9'></edge><edge id='e69' source='n14' target='n13'></edge><edge id='e70' source='n14' target='n36'></edge><edge id='e71' source='n15' target='n20'></edge><edge id='e72' source='n15' target='n5'></edge><edge id='e73' source='n15' target='n35'></edge><edge id='e74' source='n16' target='n1'></edge><edge id='e75' source='n16' target='n11'></edge><edge id='e76' source='n16' target='n37'></edge><edge id='e77' source='n16' target='n38'></edge><edge id='e78' source='n16' target='n19'></edge><edge id='e79' source='n17' target='n1'></edge><edge id='e80' source='n17' target='n3'></edge><edge id='e81' source='n17' target='n5'></edge><edge id='e82' source='n17' target='n35'></edge><edge id='e83' source='n17' target='n37'></edge><edge id='e84' source='n17' target='n39'></edge><edge id='e85' source='n18' target='n26'></edge><edge id='e86' source='n18' target='n35'></edge><edge id='e87' source='n19' target='n1'></edge><edge id='e88' source='n19' target='n8'></edge><edge id='e89' source='n19' target='n28'></edge><edge id='e90' source='n19' target='n34'></edge><edge id='e91' source='n19' target='n16'></edge><edge id='e92' source='n19' target='n38'></edge><edge id='e93' source='n20' target='n5'></edge><edge id='e94' source='n20' target='n11'></edge><edge id='e95' source='n20' target='n15'></edge><edge id='e96' source='n20' target='n37'></edge><edge id='e97' source='n21' target='n0'></edge><edge id='e98' source='n21' target='n22'></edge><edge id='e99' source='n21' target='n9'></edge><edge id='e100' source='n21' target='n10'></edge><edge id='e101' source='n21' target='n13'></edge><edge id='e102' source='n21' target='n14'></edge><edge id='e103' source='n22' target='n0'></edge><edge id='e104' source='n22' target='n21'></edge><edge id='e105' source='n22' target='n23'></edge><edge id='e106' source='n22' target='n6'></edge><edge id='e107' source='n22' target='n10'></edge><edge id='e108' source='n22' target='n12'></edge><edge id='e109' source='n23' target='n22'></edge><edge id='e110' source='n23' target='n3'></edge><edge id='e111' source='n23' target='n6'></edge><edge id='e112' source='n23' target='n10'></edge><edge id='e113' source='n23' target='n39'></edge><edge id='e114' source='n24' target='n7'></edge><edge id='e115' source='n24' target='n28'></edge><edge id='e116' source='n25' target='n27'></edge><edge id='e117' source='n25' target='n9'></edge><edge id='e118' source='n25' target='n30'></edge><edge id='e119' source='n25' target='n33'></edge><edge id='e120' source='n25' target='n14'></edge><edge id='e121' source='n26' target='n6'></edge><edge id='e122' source='n26' target='n32'></edge><edge id='e123' source='n26' target='n35'></edge><edge id='e124' source='n26' target='n18'></edge><edge id='e125' source='n26' target='n39'></edge><edge id='e126' source='n27' target='n4'></edge><edge id='e127' source='n27' target='n25'></edge><edge id='e128' source='n27' target='n30'></edge><edge id='e129' source='n28' target='n2'></edge><edge id='e130' source='n28' target='n24'></edge><edge id='e131' source='n28' target='n8'></edge><edge id='e132' source='n28' target='n19'></edge><edge id='e133' source='n29' target='n6'></edge><edge id='e134' source='n29' target='n12'></edge><edge id='e135' source='n30' target='n2'></edge><edge id='e136' source='n30' target='n4'></edge><edge id='e137' source='n30' target='n25'></edge><edge id='e138' source='n30' target='n27'></edge><edge id='e139' source='n30' target='n33'></edge><edge id='e140' source='n31' target='n3'></edge><edge id='e141' source='n31' target='n8'></edge><edge id='e142' source='n31' target='n9'></edge><edge id='e143' source='n31' target='n10'></edge><edge id='e144' source='n31' target='n33'></edge><edge id='e145' source='n31' target='n34'></edge><edge id='e146' source='n32' target='n6'></edge><edge id='e147' source='n32' target='n26'></edge><edge id='e148' source='n33' target='n2'></edge><edge id='e149' source='n33' target='n25'></edge><edge id='e150' source='n33' target='n8'></edge><edge id='e151' source='n33' target='n9'></edge><edge id='e152' source='n33' target='n30'></edge><edge id='e153' source='n33' target='n31'></edge><edge id='e154' source='n34' target='n1'></edge><edge id='e155' source='n34' target='n3'></edge><edge id='e156' source='n34' target='n8'></edge><edge id='e157' source='n34' target='n31'></edge><edge id='e158' source='n34' target='n19'></edge><edge id='e159' source='n35' target='n5'></edge><edge id='e160' source='n35' target='n26'></edge><edge id='e161' source='n35' target='n15'></edge><edge id='e162' source='n35' target='n17'></edge><edge id='e163' source='n35' target='n18'></edge><edge id='e164' source='n35' target='n39'></edge><edge id='e165' source='n36' target='n13'></edge><edge id='e166' source='n36' target='n14'></edge><edge id='e167' source='n37' target='n20'></edge><edge id='e168' source='n37' target='n1'></edge><edge id='e169' source='n37' target='n5'></edge><edge id='e170' source='n37' target='n11'></edge><edge id='e171' source='n37' target='n16'></edge><edge id='e172' source='n37' target='n17'></edge><edge id='e173' source='n38' target='n16'></edge><edge id='e174' source='n38' target='n19'></edge><edge id='e175' source='n39' target='n3'></edge><edge id='e176' source='n39' target='n23'></edge><edge id='e177' source='n39' target='n26'></edge><edge id='e178' source='n39' target='n35'></edge><edge id='e179' source='n39' target='n17'></edge></graph></graphml>";
  
    GraphOperations go;
    // source, destination
    // graph, trees, max depth
    std::cout<<"Starting..."<<std::endl;
    std::string result = go.CreateBalancedForest(s, 7, 5);
    //std::string result = go.GetShortestPath (s , 2, 3);

    std::cout <<"Result: "<< result << std::endl;

    return 0;

}
