/*
 * FindCenter.cpp
 *
 *  Created on: 2017-05-26
 *      Author: ivan
 */

#include <iostream>
#include <iomanip>
#include <algorithm>

#include "GraphOperations.h"

using namespace std;
using namespace graph_buzz;

void GraphOperations::WriteGraphToFile(const Graph& g, const std::string& filename)
{
  std::ofstream graphStream;
  graphStream.open(filename.c_str());
  boost::write_graphviz(graphStream, g );
  graphStream.close();
}
std::string GraphOperations::WriteGraphToDotString(const Graph& g, dynamic_properties& dp)
{
  std::stringstream stream;
  boost::write_graphviz_dp(stream, g, dp);
  return stream.str();
}

std::string GraphOperations::WriteGraphToString(const Graph& g, dynamic_properties& dp)
{
  std::stringstream stream;
  boost::write_graphviz_dp(stream, g, dp);

  std::string delimiter = "\n";
  std::string result = "";
  std::string s = stream.str();

  size_t pos = 0;
  std::string token;
  int i = 0;
  while((pos = s.find(delimiter)) != std::string::npos){
	  token = s.substr(0, pos);
	  if(i > num_vertices(g)){
		  result += token;
	  }
	  s.erase(0, pos + delimiter.length());
	  i++;
  }

  boost::replace_all(result, ";}",";");
  boost::replace_all(result, "--","-");
  boost::replace_all(result, " ","");
  return result;
}

void GraphOperations::OpenFromXML(Graph& g, dynamic_properties& dp, const std::string& filename){
	std::ifstream inFile;

	inFile.open(filename.c_str(), std::ifstream::in);
	read_graphml(inFile, g, dp);
}

void GraphOperations::OpenFromString(Graph& g, dynamic_properties& dp, char buffer []){
	membuf sbuf(buffer, buffer + strlen(buffer));
	std::istream in(&sbuf);
	read_graphml(in, g, dp);
}

void GraphOperations::SetNames(Graph& g, NameMap& nameMap){
    for(int i = 0; i < num_vertices(g); i++){
    	std::stringstream ss;
    	ss << (i + 15);
    	nameMap[i] = i;
    }
}

void GraphOperations::SetWeights(Graph& g, float weight){
    // sets the weight
    graph_traits<Graph>::edge_iterator ei, ee;
    for(tie(ei, ee) = edges(g); ei != ee; ++ei){
    	put(edge_weight, g, *ei, weight);
    }
}

void GraphOperations::RemoveEdges(Graph &g){
    graph_traits<Graph>::vertex_iterator i, end;
    for(tie(i, end) = vertices(g); i != end; ++i) {
    	//cout << "Name: " << get(nameMap, *i) << endl;
    	clear_vertex(*i, g);
    }
}

void GraphOperations::PrintGraphProperties(Graph& g, NameMap& nameMap, DistanceMap& distanceMap){
    graph_traits<Graph>::vertex_iterator i, end;
    for(tie(i, end) = vertices(g); i != end; ++i) {
    	cout << "Name: " << get(nameMap, *i) << endl;
    }

    for(int i = 0; i < num_vertices(g); i++){
    	cout << "Distance: " << distanceMap[i] << endl;
    }
}

std::vector< std::pair<int, float> > GraphOperations::GetCentralities(Graph& g, NameMap& nameMap, IndexMap& indexMap){
	DistanceMatrix dsts(num_vertices(g));
	DistanceMatrixMap dm(dsts, g);
	WeightMap wm(1);
    floyd_warshall_all_pairs_shortest_paths(g, dm, weight_map(wm));

    // centrality
    ClosenessContainer cents(num_vertices(g));
    ClosenessMap cm(cents, g);
    all_closeness_centralities(g, dm, cm);

    std::vector< std::pair<int, float> > centralities;
    graph_traits<Graph>::vertex_iterator i, end;
    for(boost::tie(i, end) = vertices(g); i != end; ++i) {
        centralities.push_back(std::make_pair((int)get(indexMap, *i),(float)get(cm, *i)));
    }

    std::sort(centralities.begin(), centralities.end(), boost::bind(&std::pair<int, float>::second, _1) > boost::bind(&std::pair<int, float>::second, _2));
    return centralities;
}

std::string GraphOperations::CreateTree(std::string text){

		Graph g;
		dynamic_properties dp;

		 const std::string vn = "vertex_name";
		 dp.property(vn,get(vertex_name,g));

		// convert string to char array
		char *a = new char[text.size()+1];
		a[text.size()]=0;
		memcpy(a,text.c_str(),text.size());

		OpenFromString(g, dp, a);
		//OpenFromXML(g, dp, "../samples/graph-li.xml");

		// Create data for Dijkstra
		std::vector<Vertex> predecessors(boost::num_vertices(g)); 	// To store parents
		std::vector<Weight> distances(boost::num_vertices(g)); 		// To store distances

		IndexMap indexMap = boost::get(boost::vertex_index, g);
		NameMap nameMap = boost::get(boost::vertex_name, g);

		PredecessorMap predecessorMap(&predecessors[0], indexMap);
		DistanceMap distanceMap(&distances[0], indexMap);

		// set names and weights
		//SetNames(g, nameMap);
		SetWeights(g, 1);
		//PrintGraphProperties(g, nameMap, distanceMap);

		// floyd warshall
		std::vector< std::pair<int, float> > centralities = GetCentralities(g, nameMap, indexMap);

		// dijkstra
		Vertex v0 = vertex(centralities[0].first, g); // < change
		boost::dijkstra_shortest_paths(g, v0, boost::distance_map(distanceMap).predecessor_map(predecessorMap));

		Graph tree;
		boost::copy_graph(g, tree);
		RemoveEdges(tree);

		typedef std::vector<Graph::edge_descriptor> PathType;

		 for(int i = 0; i < num_vertices(g); i++){
			 PathType path;
			 Vertex v3= vertex(i, g);
			 Vertex v = v3; // We want to start at the destination and work our way back to the source
			 for(Vertex u = predecessorMap[v];  u != v; v = u, u = predecessorMap[v])
			 {
				std::pair<Graph::edge_descriptor, bool> edgePair = boost::edge(u, v, g);
				Graph::edge_descriptor edge = edgePair.first;
				path.push_back( edge );
				if(boost::edge(u,v, tree).second == false)
					add_edge(u, v, 1, tree);	// source, destination, weight, tree
			 }
		 }

		 dynamic_properties dp2;
		 dp2.property("node_id", get(vertex_name, tree));
		 return WriteGraphToString(tree, dp2);
		//return text;
}

std::string GraphOperations::SayHello(){
	return "Hello";
}
