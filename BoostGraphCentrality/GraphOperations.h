/*
 * FindCenter.h
 *
 *  Created on: 2017-05-26
 *      Author: ivan
 */

#ifndef GRAPHOPERATIONS_H_
#define GRAPHOPERATIONS_H_

#include <boost/graph/undirected_graph.hpp>
#include <boost/graph/exterior_property.hpp>
#include <boost/graph/floyd_warshall_shortest.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/closeness_centrality.hpp>
#include <boost/graph/property_maps/constant_property_map.hpp>
#include <boost/graph/graphml.hpp>
#include <boost/graph/graphviz.hpp>
#include <boost/graph/copy.hpp>
#include <streambuf>

// define graph properties: weight and name
typedef float Weight;
typedef boost::property<boost::edge_weight_t, Weight> WeightProperty;
typedef boost::property<boost::vertex_name_t, std::string> NameProperty;

// graph definition
typedef boost::adjacency_list < boost::listS, boost::vecS, boost::undirectedS, NameProperty, WeightProperty > Graph;

// create graph edge and vertex descriptors (accesable via iterators)
typedef boost::graph_traits < Graph >::vertex_descriptor Vertex;
typedef boost::graph_traits < Graph >::edge_descriptor Edge;

typedef boost::constant_property_map<Edge, int> WeightMap;
typedef boost::property_map < Graph, boost::vertex_index_t >::type IndexMap;
typedef boost::property_map < Graph, boost::vertex_name_t >::type NameMap;

typedef boost::iterator_property_map < Vertex*, IndexMap, Vertex, Vertex& > PredecessorMap;
typedef boost::iterator_property_map < Weight*, IndexMap, Weight, Weight& > DistanceMap;

// Dijkstra

typedef boost::exterior_vertex_property<Graph, int> DistanceProperty;
typedef DistanceProperty::matrix_type DistanceMatrix;
typedef DistanceProperty::matrix_map_type DistanceMatrixMap;

// Declare a container and its corresponding property map that
// will contain the resulting closeness centralities of each
// vertex in the graph.
typedef boost::exterior_vertex_property<Graph, float> ClosenessProperty;
typedef ClosenessProperty::container_type ClosenessContainer;
typedef ClosenessProperty::map_type ClosenessMap;


#endif /* FINDCENTER_H_ */

using namespace boost;


namespace graph_buzz{

	struct membuf : std::streambuf
	{
		membuf(char* begin, char* end) {
			this->setg(begin, begin, end);
		}
	};

	class GraphOperations {
	public:
		void WriteGraphToFile(const Graph& g, const std::string& filename);
		std::string WriteGraphToString(const Graph& g, dynamic_properties& dp);
		std::string WriteGraphToDotString(const Graph& g, dynamic_properties& dp);
		void OpenFromXML(Graph& g, dynamic_properties& dp, const std::string& filename);
		void OpenFromString(Graph& g, dynamic_properties& dp, char buffer []);
		void SetNames(Graph& g, NameMap& nameMap);
		void SetWeights(Graph& g, float weight);
		void PrintGraphProperties(Graph& g, NameMap& nameMap, DistanceMap& distanceMap);
		void RemoveEdges(Graph &g);
		std::string CreateTree(std::string text);
		std::string SayHello();
	private:
		std::vector< std::pair<int, float> > GetCentralities(Graph& g, NameMap& nameMap, IndexMap& indexMap);
	};
}


