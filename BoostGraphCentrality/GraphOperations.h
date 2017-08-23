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
#include <boost/graph/stoer_wagner_min_cut.hpp>
#include <boost/graph/closeness_centrality.hpp>
#include <boost/graph/betweenness_centrality.hpp>
#include <boost/graph/property_maps/constant_property_map.hpp>
#include <boost/graph/strong_components.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_utility.hpp>
#include <boost/config.hpp>
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


namespace graph_buzz
{

struct membuf : std::streambuf {
    membuf ( char* begin, char* end ) {
        this->setg ( begin, begin, end );
    }
};

class GraphOperations
{
public:
    class TreeVertex
    {
    private:
        int _depth;
        Vertex _m;
        Vertex _m_parent;
        std::vector<TreeVertex>* _children;

    public:
        TreeVertex ( Vertex id, int depth );
        void SetParent ( Vertex parent );
        void SetChild ( TreeVertex* child );
        std::vector<TreeVertex>* GetChildren();
        Vertex GetParent();
        Vertex GetId();
    };

public:
    void WriteGraphToFile ( const Graph& g, const std::string& filename );
    std::string WriteGraphToString ( const Graph& g, dynamic_properties& dp );
    std::string WriteGraphToDotString ( const Graph& g, dynamic_properties& dp );
    void OpenFromXML ( Graph& g, dynamic_properties& dp, const std::string& filename );
    void OpenFromString ( Graph& g, dynamic_properties& dp, char buffer [] );
    void SetNames ( Graph& g, NameMap& nameMap );
    void SetWeights ( Graph& g, DistanceMap& distanceMap, float weight );
    void PrintGraphProperties ( Graph& g, NameMap& nameMap, DistanceMap& distanceMap );
    void RemoveEdges ( Graph& g );
    Vertex GetFreeNeighbor ( Graph& g, Vertex vertex, std::vector<Vertex> taken );
    std::string CreateTree ( std::string text );
    std::string CreateBalancedForest ( std::string text, int num_partitions, int max_depth);
    std::string GetShortestPath(std::string text, int target, int source);

private:
    std::vector< std::pair<int, float> > GetCentralities ( Graph& g, NameMap& nameMap, IndexMap& indexMap );
    std::vector<TreeVertex*> zipit ( std::vector<TreeVertex*> next_level_nodes, std::vector<TreeVertex>* children, int depth );
    Graph ExtractSubgraph ( Graph g, TreeVertex* branch, int max_depth );
    bool IsIn ( std::vector<int> elements, int element );
    bool EdgeExists ( Graph g, int member, int submember );
    Graph ConstructSubgraph ( Graph g, std::vector<int> subtree_vertices );
    std::vector<int> NonNeigbourVertices(Graph g, std::vector< std::pair<int, float> > centralities);
    std::vector<int> SortedByDegree(Graph g, std::vector<int> existing_candidates);
    std::vector<int> GetBorderCycle(Graph g, std::vector<int> existing_candidates);
    bool AreNeigbours(Graph g, int starting_node, int ending_node);
    Graph ConstructTreeFromGraph(Graph g);
};
}


