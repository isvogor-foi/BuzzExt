/*
 * FindCenter.cpp
 *
 *  Created on: 2017-05-26
 *      Author: ivan
 */

#include <iostream>
#include <iomanip>
#include <algorithm>
#include <boost/static_assert.hpp>

#include "zip.h"
#include "GraphOperations.h"
#define BAD_OUTPUT 10101010

using namespace std;
using namespace graph_buzz;

/*******************************************************
 * Tree
 */

GraphOperations::TreeVertex::TreeVertex ( Vertex id, int depth )
{
    _m = id;
    _depth = depth;
    _children = new std::vector<TreeVertex>();
}

void GraphOperations::TreeVertex::SetParent ( Vertex parent )
{
    _m_parent = parent;
}

void GraphOperations::TreeVertex::SetChild ( TreeVertex* child )
{
    _children->push_back ( *child );
}

std::vector<GraphOperations::TreeVertex>* GraphOperations::TreeVertex::GetChildren()
{
    return _children;
}

Vertex GraphOperations::TreeVertex::GetParent()
{
    return _m_parent;
}

Vertex GraphOperations::TreeVertex::GetId()
{
    return _m;
}

/*********************************************************************
***** File related operations
**********************************************************************/

void GraphOperations::WriteGraphToFile ( const Graph& g, const std::string& filename )
{
    std::ofstream graphStream;
    graphStream.open ( filename.c_str() );
    boost::write_graphviz ( graphStream, g );
    graphStream.close();
}
std::string GraphOperations::WriteGraphToDotString ( const Graph& g, dynamic_properties& dp )
{
    std::stringstream stream;
    boost::write_graphviz_dp ( stream, g, dp );
    return stream.str();
}

std::string GraphOperations::WriteGraphToString ( const Graph& g, dynamic_properties& dp )
{
    std::stringstream stream;
    boost::write_graphviz_dp ( stream, g, dp );

    std::string delimiter = "\n";
    std::string result = "";
    std::string s = stream.str();

    size_t pos = 0;
    std::string token;
    int i = 0;
    while ( ( pos = s.find ( delimiter ) ) != std::string::npos ) {
        token = s.substr ( 0, pos );
        if ( i > num_vertices ( g ) ) {
            result += token;
        }
        s.erase ( 0, pos + delimiter.length() );
        i++;
    }

    boost::replace_all ( result, ";}",";" );
    boost::replace_all ( result, "--","-" );
    boost::replace_all ( result, " ","" );
    return result;
}

void GraphOperations::OpenFromXML ( Graph& g, dynamic_properties& dp, const std::string& filename )
{
    std::ifstream inFile;

    inFile.open ( filename.c_str(), std::ifstream::in );
    read_graphml ( inFile, g, dp );
}

void GraphOperations::OpenFromString ( Graph& g, dynamic_properties& dp, char buffer [] )
{
    membuf sbuf ( buffer, buffer + strlen ( buffer ) );
    std::istream in ( &sbuf );
    read_graphml ( in, g, dp );
}

void GraphOperations::SetNames ( Graph& g, NameMap& nameMap )
{
    for ( int i = 0; i < num_vertices ( g ); i++ ) {
        std::stringstream ss;
        ss << ( i + 15 );
        nameMap[i] = i;
    }
}

/*********************************************************************
***** Graph properties
**********************************************************************/

void GraphOperations::SetWeights ( Graph& g, DistanceMap& distanceMap, float weight )
{
    // sets the weight
    graph_traits<Graph>::edge_iterator ei, ee;
    for ( tie ( ei, ee ) = edges ( g ); ei != ee; ++ei ) {
        put ( edge_weight, g, *ei, weight );
    }

    for ( int i = 0; i < num_edges ( g ); i++ ) {
        distanceMap[i] = weight;
    }
}

void GraphOperations::RemoveEdges ( Graph &g )
{
    // Removes all edges to and from vertex.
    graph_traits<Graph>::vertex_iterator i, end;
    for ( tie ( i, end ) = vertices ( g ); i != end; ++i ) {
        //cout << "Name: " << get(nameMap, *i) << endl;
        clear_vertex ( *i, g );
    }
}

void GraphOperations::PrintGraphProperties ( Graph& g, NameMap& nameMap, DistanceMap& distanceMap )
{
    for ( int i = 0; i < num_vertices ( g ); i++ ) {
        cout << "Vertex: " << nameMap[i] << endl;
    }

    for ( int i = 0; i < num_edges ( g ); i++ ) {
        cout << "Weight: " << distanceMap[i] << endl;
    }
}
// pair vertice and value
std::vector< std::pair<int, float> > GraphOperations::GetCentralities ( Graph& g, NameMap& nameMap, IndexMap& indexMap )
{
    DistanceMatrix dsts ( num_vertices ( g ) );
    DistanceMatrixMap dm ( dsts, g );
    WeightMap wm ( 1 );
    floyd_warshall_all_pairs_shortest_paths ( g, dm, weight_map ( wm ) );

    // centrality
    ClosenessContainer cents ( num_vertices ( g ) );
    ClosenessMap cm ( cents, g );
    all_closeness_centralities ( g, dm, cm );

    std::vector< std::pair<int, float> > centralities;
    graph_traits<Graph>::vertex_iterator i, end;
    for ( boost::tie ( i, end ) = vertices ( g ); i != end; ++i ) {
        centralities.push_back ( std::make_pair ( ( int ) get ( indexMap, *i ), ( float ) get ( cm, *i ) ) );
    }

    std::sort ( centralities.begin(), centralities.end(), boost::bind ( &std::pair<int, float>::second, _1 ) > boost::bind ( &std::pair<int, float>::second, _2 ) );
    return centralities;
}

/*********************************************************************
***** Graph related opreations
**********************************************************************/

Vertex GraphOperations::GetFreeNeighbor ( Graph& g, Vertex vertex, std::vector<Vertex> taken )
{
    // loop through neighbours and extract one of the neigbours not in the taken list
    Graph::adjacency_iterator neighbourIt, neighbourEnd;
    tie ( neighbourIt, neighbourEnd ) = adjacent_vertices ( vertex, g );

    for ( ; neighbourIt != neighbourEnd; ++neighbourIt ) {
        Vertex v_current_neighbor = *neighbourIt;
        if ( std::find ( taken.begin(), taken.end(), v_current_neighbor ) == taken.end() ) {
            return v_current_neighbor;
        }
    }
    return BAD_OUTPUT;
}

Graph GraphOperations::ConstructSubgraph ( Graph g, std::vector<int> subtree_vertices )
{
    Graph subgraph;
    BOOST_FOREACH ( int member, subtree_vertices ) {
        //std::cout<<"Member: "<<member<<std::endl;
        NameMap nameMap = boost::get ( boost::vertex_name, subgraph );
        Vertex a = add_vertex ( subgraph );
        std::stringstream int_stream;
        int_stream << member;
        nameMap[a] = int_stream.str();
    }

    NameMap v_names = boost::get ( boost::vertex_name, subgraph );
    graph_traits<Graph>::vertex_iterator s_i, s_end;
    graph_traits<Graph>::vertex_iterator t_i, t_end;

    for ( tie ( s_i, s_end ) = vertices ( subgraph ); s_i != s_end; ++s_i ) {
        for ( tie ( t_i, t_end ) = vertices ( subgraph ); t_i != t_end; ++t_i ) {
            int v_source = *s_i;
            int v_target = *t_i;
            int v_source_name = lexical_cast<int> ( get ( v_names, v_source ) );
            int v_target_name = lexical_cast<int> ( get ( v_names, v_target ) );
            if ( EdgeExists ( g, v_source_name, v_target_name ) && !EdgeExists ( subgraph, v_target, v_source ) ) {
                add_edge ( v_source, v_target, subgraph );
            }
        }
    }
    return subgraph;
}

bool GraphOperations::EdgeExists ( Graph g, int member, int submember )
{
    graph_traits<Graph>::edge_iterator ei, ee;
    for ( tie ( ei, ee ) = edges ( g ); ei != ee; ++ei ) {
        int v_source = source ( *ei, g );
        int v_target = target ( *ei, g );
        if ( v_source == member && v_target == submember ) {
            return true;
        }
    }
    return false;
}


bool GraphOperations::IsIn ( std::vector<int> elements, int element )
{
    BOOST_FOREACH ( int current, elements ) {
        if ( current == element ) {
            return true;
        }
    }
    return false;
}


std::vector<GraphOperations::TreeVertex*> GraphOperations::zipit ( std::vector<TreeVertex*> next_level_nodes, std::vector<TreeVertex>* children, int depth )
{
    std::vector<TreeVertex*> children_list;
    std::vector<TreeVertex*> zipped_list;
    for ( std::vector<TreeVertex>::iterator it = children->begin(); it != children->end(); ++it ) {
        //children_list.push_back ( new TreeVertex ( ( *it ).GetId(), depth ) );
        children_list.push_back ( & ( *it ) );

    }
    int longer_list = ( ( next_level_nodes.size() < children_list.size() ) ? next_level_nodes.size() : children_list.size() );
    //std::cout<<"Main: " << next_level_nodes.size() << "Children" << children_list.size() <<std::endl;
    //std::cout<<"Longer: " << longer_list << std::endl;

    std::vector<TreeVertex*>::iterator nln_iterator = next_level_nodes.begin();
    std::vector<TreeVertex*>::iterator cld_iterator = children_list.begin();

    while ( nln_iterator != next_level_nodes.end() || cld_iterator != children_list.end() ) {
        if ( nln_iterator != next_level_nodes.end() ) {
            //std::cout << "Added " << (*nln_iterator)->GetId() << std::endl;
            //zipped_list.push_back ( new TreeVertex ( ( *nln_iterator )->GetId(), depth ) );
            zipped_list.push_back ( ( *nln_iterator ) );
            ++nln_iterator;
        }
        if ( cld_iterator != children_list.end() ) {
            //std::cout << "Added(2) " << (*cld_iterator)->GetId() << std::endl;
            //zipped_list.push_back ( new TreeVertex ( ( *cld_iterator )->GetId(), depth ) );
            zipped_list.push_back ( ( *cld_iterator ) );
            ++cld_iterator;
        }
    }
    /*
        std::cout << "Returning: " << zipped_list.size() << std::endl;
        BOOST_FOREACH ( TreeVertex* current_vertex, zipped_list ) {
            std::cout<<"Current: " << current_vertex->GetId() << " ";
        }
    */
    //delete children_list;
    return zipped_list;

}

std::vector<int> GraphOperations::NonNeigbourVertices ( Graph g, std::vector< std::pair<int, float> > centralities )
{
    std::vector<int> result;
    NameMap nameMap = boost::get ( boost::vertex_name, g );

    // reverse sort
    std::sort ( centralities.begin(), centralities.end(),
                boost::bind ( &std::pair<int, float>::second, _1 ) <
                boost::bind ( &std::pair<int, float>::second, _2 ) );
    int v_least_central = vertex ( centralities[0].first, g ); // < change
    result.push_back ( v_least_central );

    for ( int node = 0; node < centralities.size(); node++ ) {
        bool push_it = true;
        BOOST_FOREACH ( int root, result ) {
            if ( AreNeigbours ( g, root, centralities[node].first ) || centralities[node].first == root ) {
                push_it = false;
            }
        }
        if ( push_it ) {
            result.push_back ( centralities[node].first );
        }
    }
    /*
    BOOST_FOREACH(int root, result){
      std::cout<< "Candidate: " << root << std::endl;
    }
    */

    return result;
}
std::vector<int> GraphOperations::SortedByDegree ( Graph g, std::vector<int> existing_candidates )
{
    std::vector<int> result = existing_candidates;
    std::vector< std::pair<int, int> > degree_sorted;
    //std::cout<<"Degree:" << degree(0, g) <<std::endl;
    for ( int i = 0; i < num_vertices ( g ); i++ ) {
        degree_sorted.push_back ( std::make_pair ( i, degree ( i, g ) ) );
    }

    std::sort ( degree_sorted.begin(), degree_sorted.end(),
                boost::bind ( &std::pair<int, int>::second, _1 ) <
                boost::bind ( &std::pair<int, int>::second, _2 ) );

    //int v_least_central = vertex ( centralities[0].first, g );

    for ( int candidate = 0; candidate < degree_sorted.size(); candidate++ ) {
        if ( !IsIn ( existing_candidates, degree_sorted[candidate].first ) ) {
            result.push_back ( degree_sorted[candidate].first );
        }
    }

    return result;
}

bool GraphOperations::AreNeigbours ( Graph g, int starting_node, int ending_node )
{
    Graph::adjacency_iterator neighbourIt, neighbourEnd;
    tie ( neighbourIt, neighbourEnd ) = adjacent_vertices ( starting_node, g );
    for ( ; neighbourIt != neighbourEnd; ++neighbourIt ) {
        int v_current_neighbor = *neighbourIt;
        if ( v_current_neighbor == ending_node ) {
            return true;
        }
    }
    return false;
}

Graph GraphOperations::ExtractSubgraph ( Graph g, TreeVertex* branch, int max_depth )
{
    // write the children in the list
    std::vector<TreeVertex*> current_children_list;
    for ( std::vector<TreeVertex>::iterator it = branch->GetChildren()->begin(); it != branch->GetChildren()->end(); ++it ) {
        current_children_list.push_back ( & ( *it ) );
    }

    std::vector<int> subtree_vertices;
    subtree_vertices.push_back ( ( int ) branch->GetId() );
    int current_depth = 0;

    // find all chilldren and build a list
    while ( current_depth < max_depth ) {
        std::vector<TreeVertex*> next_children_list;
        for ( std::vector<TreeVertex*>::iterator cc_i = current_children_list.begin(); cc_i != current_children_list.end(); ++cc_i ) {
            subtree_vertices.push_back ( ( int ) ( *cc_i )->GetId() );
            std::vector<TreeVertex>* current_childrens_children = ( *cc_i )->GetChildren();
            for ( std::vector<TreeVertex>::iterator c_i = current_childrens_children->begin(); c_i != current_childrens_children->end(); ++c_i ) {
                next_children_list.push_back ( & ( *c_i ) );
            }
        }
        current_depth++;
        current_children_list = next_children_list;
    }

    Graph subgraph = ConstructSubgraph ( g, subtree_vertices );
    return subgraph;
}

/*********************************************************************
***** Tree related opreations
**********************************************************************/

std::string GraphOperations::CreateBalancedForest ( std::string text, int num_partitions )
{
    Graph g;
    dynamic_properties dp;

    const std::string vn = "vertex_name";
    dp.property ( vn,get ( vertex_name,g ) );

    // convert string to char array
    char *graph_xml = new char[text.size() +1];
    graph_xml[text.size()]=0;
    memcpy ( graph_xml,text.c_str(),text.size() );

    OpenFromString ( g, dp, graph_xml );
    //OpenFromXML(g, dp, "../samples/graph-li.xml");
    //OpenFromXML(g, dp, "../samples/temp.xml");

    //---
    // Create data for Dijkstra
    std::vector<Vertex> predecessors ( boost::num_vertices ( g ) ); 	// To store parents
    std::vector<Weight> distances ( boost::num_edges ( g ) ); 		// To store distances

    IndexMap indexMap = boost::get ( boost::vertex_index, g );
    NameMap nameMap = boost::get ( boost::vertex_name, g );

    PredecessorMap predecessorMap ( &predecessors[0], indexMap );
    DistanceMap distanceMap ( &distances[0], indexMap );

    SetWeights ( g, distanceMap, 1.0f );
    //PrintGraphProperties(g, nameMap, distanceMap);

    std::vector< std::pair<int, float> > centralities = GetCentralities ( g, nameMap, indexMap );



    if ( centralities.size() < num_partitions ) {
        std::cout<<"You know... the number of partitions is bigger than you have vertices! (HELLOO??ERROR!!!)" <<std::endl;
        return NULL;
    }

    std::vector<TreeVertex*> subtrees;
    std::vector<TreeVertex*> current_level_nodes;
    std::vector<Vertex> taken_vertices;

    // select least central nodes as starting point for partitioning
    /*
    for(int i = centralities.size() - 1 ; i >= (centralities.size() - num_partitions); i--){
      std::cout<<i<<". Least central node: " << centralities[i].first << ", " << centralities[i].second <<std::endl;
      subtrees.push_back ( new TreeVertex ( centralities[i].first, 0 ) );
      taken_vertices.push_back ( centralities[i].first );
    }
    */

    // select most central nodes as starting point for partitioning
    
    for(int i = 0 ; i < centralities.size(); i++){
      std::cout<<i<<". Most central node: " << centralities[i].first << ", " << centralities[i].second <<std::endl;
    }
    

    // TODO: from the main graph, remove duplicates?
    // ConstructSubgraph
    std::vector<int> vertex_range;
    for ( int i = 0; i < 20; i++ ) {
        vertex_range.push_back ( i );
    }
    g = ConstructSubgraph ( g, vertex_range ); //-- same?

    std::vector<int> candidates = NonNeigbourVertices ( g, centralities );
    candidates = SortedByDegree ( g, candidates );

    /*
    BOOST_FOREACH ( int c, candidates ) {
        std::cout<< "Candidates final: " << c << std::endl;
    }
    */

    //subtrees.push_back ( new TreeVertex ( 7, 0 ) );
    //subtrees.push_back ( new TreeVertex ( 3, 0 ) );
    //taken_vertices.push_back ( 7 );
    //taken_vertices.push_back ( 3 );

    //taken_vertices.push_back(8);

    for ( int i = candidates.size() - 1 ; i >= ( candidates.size() - num_partitions ); i-- ) {
        subtrees.push_back ( new TreeVertex ( candidates[i], 0 ) );
        taken_vertices.push_back ( candidates[i] );
    }

    BOOST_FOREACH ( TreeVertex* branch, subtrees )
    current_level_nodes.push_back ( branch );


    bool increase_depth = false;
    int d = 0;
    int depth = 5; // HARDCODED !!!!

    // TODO: IDEA, maybe on limit the number of children for each node?
    // TODO: Maybe the candidate nodes should be at least not neighbours? or 1 distant?
    while ( d < depth ) {
        // check to move to next depth level
        if ( increase_depth ) {
            std::vector<TreeVertex*> next_level_nodes;
            BOOST_FOREACH ( TreeVertex* current_vertex, current_level_nodes ) {
                std::vector<TreeVertex>* children = current_vertex->GetChildren();
                next_level_nodes = zipit ( next_level_nodes, children, d );
            }
            current_level_nodes = next_level_nodes;
            increase_depth = false;
        }
        if ( current_level_nodes.empty() ) {
            break;
        }
        // add children non-greedy
        int empty_set_counter = current_level_nodes.size();
        BOOST_FOREACH ( TreeVertex* current_vertex, current_level_nodes ) {
            Vertex neigbour = GetFreeNeighbor ( g, vertex ( current_vertex->GetId(), g ), taken_vertices );
            if ( neigbour != BAD_OUTPUT ) {
                taken_vertices.push_back ( neigbour );
                TreeVertex* new_child = new TreeVertex ( neigbour, d + 1 );
                new_child->SetParent ( current_vertex->GetId() );
                current_vertex->SetChild ( new_child );
                //cout<< current_vertex->GetId() << " takes " << new_child->GetId() << std::endl;
            } else {
                empty_set_counter -= 1;
                if ( empty_set_counter == 0 ) {
                    d++;
                    increase_depth = true;
                }
            }
        }
    }


    // For each tree, print it
    std::string final_output = "";
    std::vector<Graph> partitions;
    BOOST_FOREACH ( TreeVertex* current_vertex, subtrees ) {
        Graph partition = ExtractSubgraph ( g, current_vertex, depth );
        partitions.push_back ( partition );
        // print
        dynamic_properties dp;
        dp.property ( "node_id", get ( vertex_name, partition ) );
	Graph tree = ConstructTreeFromGraph(partition);
	
	final_output += WriteGraphToString ( tree, dp );
	std::cout<< "Partition: " << final_output << std::endl;
        std::cout<<"Partition: " << WriteGraphToDotString ( tree, dp ) <<std::endl;
    }

    //dynamic_properties dp2;
    //dp2.property ( "node_id", get ( vertex_name, partitions[0] ) );
    //return WriteGraphToString ( partitions[0], dp2 );
    
    return final_output;
}

std::string GraphOperations::CreateTree ( std::string text )
{

    Graph g;
    dynamic_properties dp;

    const std::string vn = "vertex_name";
    dp.property ( vn,get ( vertex_name,g ) );

    // convert string to char array
    char *a = new char[text.size() +1];
    a[text.size()]=0;
    memcpy ( a,text.c_str(),text.size() );

    //OpenFromString(g, dp, a);
    OpenFromXML ( g, dp, "../samples/graph-li.xml" );

    
    Graph tree = ConstructTreeFromGraph(g);

    dynamic_properties dp2;
    dp2.property ( "node_id", get ( vertex_name, tree ) );
    // WriteGraphToFile ( tree, "tree.dot" );
    return WriteGraphToString ( g, dp2 );
    //return text;
}

Graph GraphOperations::ConstructTreeFromGraph(Graph g)
{
    // Create data for Dijkstra
    std::vector<Vertex> predecessors ( boost::num_vertices ( g ) ); 	// To store parents
    std::vector<Weight> distances ( boost::num_edges ( g ) ); 		// To store distances

    IndexMap indexMap = boost::get ( boost::vertex_index, g );
    NameMap nameMap = boost::get ( boost::vertex_name, g );

    PredecessorMap predecessorMap ( &predecessors[0], indexMap );
    DistanceMap distanceMap ( &distances[0], indexMap );

    // set names and weights
    //SetNames(g, nameMap);
    SetWeights ( g, distanceMap, 1.0f );
    //PrintGraphProperties(g, nameMap, distanceMap);
    
      // floyd warshall
    std::vector< std::pair<int, float> > centralities = GetCentralities ( g, nameMap, indexMap );

    // dijkstra
    Vertex v0 = vertex ( centralities[0].first, g ); // < change
    boost::dijkstra_shortest_paths ( g, v0, boost::distance_map ( distanceMap ).predecessor_map ( predecessorMap ) );

    Graph tree;
    boost::copy_graph ( g, tree );
    RemoveEdges ( tree );

    typedef std::vector<Graph::edge_descriptor> PathType;

    for ( int i = 0; i < num_vertices ( g ); i++ ) {
        PathType path;
        Vertex v3= vertex ( i, g );
        Vertex v = v3; // We want to start at the destination and work our way back to the source
        for ( Vertex u = predecessorMap[v];  u != v; v = u, u = predecessorMap[v] ) {
            std::pair<Graph::edge_descriptor, bool> edgePair = boost::edge ( u, v, g );
            Graph::edge_descriptor edge = edgePair.first;
            path.push_back ( edge );
            if ( boost::edge ( u,v, tree ).second == false ) {
                add_edge ( u, v, 1, tree );    // source, destination, weight, tree
            }
        }
    }
    return tree;
}