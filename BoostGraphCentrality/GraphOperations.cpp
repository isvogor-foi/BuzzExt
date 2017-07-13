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

/*******************************************************
 *  Graph operations
 */

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
    //graph_traits<Graph>::vertex_iterator i, end;
    //for(tie(i, end) = vertices(g); i != end; ++i) {
    //	cout << "Name: " << get(nameMap, *i) << endl;
    //}

    for ( int i = 0; i < num_vertices ( g ); i++ ) {
        cout << "Vertex: " << nameMap[i] << endl;
    }

    for ( int i = 0; i < num_edges ( g ); i++ ) {
        cout << "Weight: " << distanceMap[i] << endl;
    }

    /*
    boost::property_map <Graph, boost::edge_weight_t >::type EdgeWeightMap = get(boost::edge_weight, g);
    boost::graph_traits< Graph >::edge_iterator e_it, e_end;
    for(tie(e_it, e_end) = boost::edges(g); e_it != e_end; ++e_it)
    {
      std::cout<<"Weight: " << EdgeWeightMap[*e_it] << std::endl;
    }
    */
}

Vertex GraphOperations::GetFreeNeighbor ( Graph& g, Vertex vertex, std::vector<Vertex> taken )
{
    /*
    Graph::vertex_iterator vertexIt, vertexEnd;
    tie(vertexIt, vertexEnd) = vertices(g);
    for(; vertexIt != vertexEnd; ++vertexIt){
    	Vertex v_current = *vertexIt;
    	if(v_current == vertex){
    		std::cout<<"Name: " << v_current <<std::endl;
    	}
    }
    */

    Graph::adjacency_iterator neighbourIt, neighbourEnd;
    tie ( neighbourIt, neighbourEnd ) = adjacent_vertices ( vertex, g );
    for ( ; neighbourIt != neighbourEnd; ++neighbourIt ) {
        Vertex v_current_neighbor = *neighbourIt;
        //std::cout<< "Neighbour: " << v_current_neighbor <<std::endl;
        if ( std::find ( taken.begin(), taken.end(), v_current_neighbor ) == taken.end() ) {
            //std::cout << "Found it ! " << v_current_neighbor << std::endl;
            return v_current_neighbor;
        }
        //std::cout<< "Neighbour: " << v_current_neighbor <<std::endl;
    }
    //std::cout<< " Returning... " <<std::endl;
    return BAD_OUTPUT;

}

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

std::string GraphOperations::CreateBalancedForest ( std::string text )
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

    // set names and weights
    //SetNames(g, nameMap);
    //dynamic_properties dp2;
    SetWeights ( g, distanceMap, 1.0f );
    //PrintGraphProperties(g, nameMap, distanceMap);

    /*
    std::vector<Vertex> taken_vertices;

    for(int i = 0; i < 10; i++) {
    	Vertex neighbour = GetFreeNeighbor(g, vertex(15, g), taken_vertices);
    	if(neighbour != BAD_OUTPUT){
    		taken_vertices.push_back(neighbour);
    	}
    }

    std::cout<<"Size: " << taken_vertices.size() << std::endl;
    */


    std::vector<TreeVertex*> subtrees;
    std::vector<TreeVertex*> current_level_nodes;
    std::vector<Vertex> taken_vertices;

    subtrees.push_back ( new TreeVertex ( 7, 0 ) );
    subtrees.push_back ( new TreeVertex ( 3, 0 ) );
    //subtree.push_back(new TreeVertex(8, 0));

    taken_vertices.push_back ( 7 );
    taken_vertices.push_back ( 3 );
    //taken_vertices.push_back(8);

    BOOST_FOREACH(TreeVertex* branch, subtrees)
      current_level_nodes.push_back(branch);


    bool increase_depth = false;
    int d = 0;
    int depth = 5;



    while ( d < depth ) {
        // check to move to next depth level
	std::cout << subtrees[0]->GetId() << " TEST: " << subtrees[0]->GetChildren()->size() << std::endl;
        if ( increase_depth ) {
            std::vector<TreeVertex*> next_level_nodes;
            BOOST_FOREACH ( TreeVertex* current_vertex, current_level_nodes ) {
                std::vector<TreeVertex>* children = current_vertex->GetChildren();
                next_level_nodes = zipit ( next_level_nodes, children, d );
		//for(std::vector<TreeVertex>::iterator it = children->begin(); it != children->end(); ++it){
		//  next_level_nodes.push_back(&(*it));
		//}
            }
            current_level_nodes = next_level_nodes;
            //current_level_nodes.clear();// = next_level_nodes;
	    /*
	    BOOST_FOREACH(TreeVertex* branch, next_level_nodes)
	      current_level_nodes.push_back(branch);
	      //TODO: Here is the issue i think... Zippit function kills parenthood...
	    /*
            std::cout<<" ------------------------ "<<std::endl;
            BOOST_FOREACH(TreeVertex* current_vertex, current_level_nodes){
	      std::cout<<"Current level: " << current_vertex->GetId() << std::endl;
            }*/
            increase_depth = false;
        }
        if ( current_level_nodes.empty() ) {
            break;
        }
        // add children non-greedy
        int empty_set_counter = current_level_nodes.size();
        /*
        std::cout<<"Current level size: " << empty_set_counter << std::endl;
	for(std::vector<TreeVertex*>::iterator it = current_level_nodes.begin(); it != current_level_nodes.end(); ++it){
	    std::cout<< "Hello: " << (*it)->GetId() << std::endl;
            Vertex neigbour = GetFreeNeighbor ( g, vertex ( (*it)->GetId(), g ), taken_vertices );
            if ( neigbour != BAD_OUTPUT ) {
                taken_vertices.push_back ( neigbour );
                TreeVertex* new_child = new TreeVertex ( neigbour, d + 1 );
                new_child->SetParent ( (*it)->GetId() );
                (*it)->SetChild ( new_child );
                cout<< (*it)->GetId() << " takes " << new_child->GetId() << std::endl;
		cout<< (*it)->GetId() << " has: " << (*it)->GetChildren()->size() << std::endl;

            } else {
                empty_set_counter -= 1;
                if ( empty_set_counter == 0 ) {
                    d++;
                    increase_depth = true;
                }
	    }
	}
	*/
        BOOST_FOREACH ( TreeVertex* current_vertex, current_level_nodes ) {
            std::cout<< "Hello: " << current_vertex->GetId() << std::endl;
            Vertex neigbour = GetFreeNeighbor ( g, vertex ( current_vertex->GetId(), g ), taken_vertices );
            if ( neigbour != BAD_OUTPUT ) {
                taken_vertices.push_back ( neigbour );
                TreeVertex* new_child = new TreeVertex ( neigbour, d + 1 );
                new_child->SetParent ( current_vertex->GetId() );
                current_vertex->SetChild ( new_child );
                cout<< current_vertex->GetId() << " takes " << new_child->GetId() << std::endl;
            } else {
                empty_set_counter -= 1;
                if ( empty_set_counter == 0 ) {
                    d++;
                    increase_depth = true;
                }
            }
        }
        
        std::cout<<"Level: " << d << std::endl;
    }


    // printout
    BOOST_FOREACH ( TreeVertex* current_vertex, subtrees ) {
      /*
        std::vector<TreeVertex>* current_children_list = current_vertex->GetChildren();
        std::vector<int> subtree_vertices;
        subtree_vertices.push_back ( current_vertex->GetId() );
        int current_depth = 0;
        while ( current_depth < depth ) {
	  std::vector<TreeVertex*> next_level_nodes;
	  for( std::vector<TreeVertex>::iterator current_child = current_children_list->begin(); current_child != current_children_list->end(); ++current_child ){
	    std::cout<<"Current child: " << (*current_child).GetId() << ", children: " << (*current_child).GetChildren()->size() <<std::endl;
	  }
	  current_depth++;
	}
	std::cout<<" ---- " << std::endl;
	*/
      GetSubgraph(g, current_vertex, depth);
    }

    // just print out
    

    dynamic_properties dp2;
    dp2.property ( "node_id", get ( vertex_name, g ) );
    return WriteGraphToString ( g, dp2 );
}

Graph GraphOperations::GetSubgraph ( Graph g, TreeVertex* branch, int max_depth )
{
    //std::cout<< branch->GetId() << " Children: " << branch->GetChildren()->size() << std::endl;
    // write the children in the list 
    std::vector<TreeVertex*> current_children_list;
    for ( std::vector<TreeVertex>::iterator it = branch->GetChildren()->begin(); it != branch->GetChildren()->end(); ++it ) {
      current_children_list.push_back(&(*it));
    }
    
    std::vector<int> subtree_vertices;
    subtree_vertices.push_back ( ( int ) branch->GetId() );
    int current_depth = 0;

    // find all chilldren and build a list 
    while(current_depth < max_depth)
    {
      std::vector<TreeVertex*> next_children_list;
      for(std::vector<TreeVertex*>::iterator cc_i = current_children_list.begin(); cc_i != current_children_list.end(); ++cc_i){
	subtree_vertices.push_back((int)(*cc_i)->GetId());
	std::vector<TreeVertex>* current_childrens_children = (*cc_i)->GetChildren();
	for(std::vector<TreeVertex>::iterator c_i = current_childrens_children->begin(); c_i != current_childrens_children->end(); ++c_i){
	  next_children_list.push_back(&(*c_i));
	}

      }
      current_depth++;
      current_children_list = next_children_list;
    }
    // copy a graph and remove extra vertices
    Graph subgraph;
    boost::copy_graph ( g, subgraph );
    RemoveEdges ( subgraph );
    graph_traits<Graph>::vertex_iterator i, end;
    for ( tie ( i, end ) = vertices ( g ); i != end; ++i ) {
	int current = (*i);
	if(!IsIn(subtree_vertices, current)){
	  cout << "Removing: " << current << endl;
	  remove_vertex(*i, subgraph);

	}
        //clear_vertex ( *i, g );
	//remove_vertex(*i, subgraph);
    }
    
    dynamic_properties dp2;
    dp2.property ( "node_id", get ( vertex_name, g ) );
    std::cout<< "Graph: " << WriteGraphToDotString(subgraph, dp2);

    /*
    std::cout<< " Constructing subgraph " <<std::endl;
    while ( current_depth < max_depth ) {
        std::vector<TreeVertex>* next_children_list;
        std::cout<<"Size: "<<current_children_list->size() <<std::endl;
        for ( std::vector<TreeVertex>::iterator it = current_children_list->begin(); it != current_children_list->end(); ++it ) {
                subtree_vertices.push_back ( ( int ) ( *it ).GetId() );
                std::cout<< ( *it ).GetId() <<" - Children: " << ( *it ).GetChildren()->size() <<std::endl;
                for ( std::vector<TreeVertex>::iterator c_it = ( *it ).GetChildren()->begin(); c_it != ( *it ).GetChildren()->end(); ++c_it ) {
                    //subtree_vertices.push_back((int)(*it).GetId());
                std::cout<<"Pushing: "<< (*c_it).GetId() <<std::endl;
                    next_children_list->push_back ( ( *c_it ) );
                }
            }
            
        std::cout<<"Going next level..." << current_depth <<std::endl;
        current_depth++;
        std::cout<<"Before: "<<current_children_list->size() <<std::endl;
        current_children_list = next_children_list;
        std::cout<<"After: "<<current_children_list->size() <<std::endl;
        std::cout<<"Real: "<<next_children_list->size() <<std::endl;

    }
    */

    // print members of the graph
    BOOST_FOREACH ( int member, subtree_vertices ) {
        std::cout<<"Member: "<<member<<std::endl;
    }

    return g;
}

bool GraphOperations::IsIn(std::vector<int> elements, int element){
  BOOST_FOREACH(int current, elements){
    if(current == element){
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
	children_list.push_back(&(*it));
      
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
	  zipped_list.push_back((*nln_iterator));
            ++nln_iterator;
        }
        if ( cld_iterator != children_list.end() ) {
            //std::cout << "Added(2) " << (*cld_iterator)->GetId() << std::endl;
            //zipped_list.push_back ( new TreeVertex ( ( *cld_iterator )->GetId(), depth ) );
	    zipped_list.push_back((*cld_iterator));
            ++cld_iterator;
        }
    }

    std::cout << "Returning: " << zipped_list.size() << std::endl;
    BOOST_FOREACH ( TreeVertex* current_vertex, zipped_list ) {
        std::cout<<"Current: " << current_vertex->GetId() << " ";
    }

    //delete children_list;
    return zipped_list;

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



    dynamic_properties dp2;
    dp2.property ( "node_id", get ( vertex_name, tree ) );
    WriteGraphToFile ( tree, "tree.dot" );
    return WriteGraphToString ( g, dp2 );
    //return text;
}

std::string GraphOperations::SayHello()
{
    return "Hello";
}

