#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <climits>

using namespace std;

namespace TarjansAlgorithm {
  /*
   * Simple structure that is used in the
   * aux vector to keep track of the additional
   * information needed at each vertex
   */
  struct Vertex {
    bool visited;
    int num;
    int low;
    int parent;
    Vertex( bool v, int n, int l, int p ) : visited(v), num(n), low(l), parent(p) {}
  };

  /*
   * Returns the articulation points in a graph
   * in the vector ArtPoints.
   * 
   * Nota Bene: This method is recursive so might fail on very
   * large graphs.
   *
   * graph: the adjcency lists
   * aux: holds extra information about each vertex in the graph
   * ArtPoints: the articulation points found so far
   * counter: the current number to label the vertex
   * 
   * Algorithm based on one from Data Structures and Algorithm Analysis in C++ 3rd Ed. (Weiss) p.385
   */
  void FindArt( int v, const vector<vector<int> >& graph, vector<Vertex>& aux, vector<int>& ArtPoints, int& counter ) {
    aux.at(v-1).visited = true; // mark the vertex as visited
    aux.at(v-1).low = aux.at(v-1).num = counter++; // Rule 1
    auto& row = graph.at(v-1);
    for_each( row.begin(), row.end(), [&](int w) { // for each adjacent vertex w...
	if(! aux.at(w-1).visited ) {               // forward edge
	  aux.at(w-1).parent = v;                  // set w's parent to be v
	  FindArt( w, graph, aux, ArtPoints, counter );
	  if( ArtPoints.end() == find( ArtPoints.begin(), ArtPoints.end(), v ) ) { // check if already found that it is an articulation point
	    if( aux.at(w-1).low >= aux.at(v-1).num ) { // Is an Articulation Point?
	      if( aux.at(v-1).parent == -1 ) {         // if root (because it has no parent)
		// count how many vertices have it as a parent, iff has more than
		// 1 child, then it is an Articulation Point
		if( 1 < count_if( aux.begin(), aux.end(), [&](Vertex vv) { return( vv.parent == v ); } ) ) {
		  ArtPoints.push_back( v );
		}
	      } else { // not root, so it is an Articulation Point
		ArtPoints.push_back( v );
	      }
	    }
	  }
	  aux.at(v-1).low = min( aux.at(v-1).low, aux.at(w-1).low ); // Rule 3
	} else if( aux.at(v-1).parent != w  ) { // back edge (not the one just travelled on)
	  aux.at(v-1).low = min( aux.at(v-1).low, aux.at(w-1).num ); // Rule 2
	}
      });
  }

  /*
   * Takes a directed graph and converts it to an
   * undirected graph, then finds the Articulation Points
   * in the graph.
   * 
   * An articulation point is a vertex in the graph that
   * if it were to be removed would disconnect the graph.
   *
   * id est: a node in a network s.t. if the node went down,
   * a portion of the network (other than that node) would
   * also go down.
   */
  void solve( vector<vector<int> >& graph ) {
    static int graph_num = 1;
    graph.push_back( vector<int>(0) ); // just for convience, fill in the last vertex
    const int graph_sz = static_cast<int>( graph.size() );
    vector<vector<int> > full_graph;
    full_graph.reserve( graph_sz );

    //
    // Since the graph is undirected, it is more efficient to one time,
    // fill in the missing values, i.e. if a is adjacent to b, then add
    // b adjacent to a
    //
    for( int v = 0; v < graph_sz; ++v ) {
      auto row = graph.at(v);
      vector<int> adj_list;
      for_each( row.begin(), row.end(), [&](int n) { if( n != 0 ) { adj_list.push_back(n); } } );
      int count = 1;
      for( auto& it : graph ) {
	if( any_of( it.begin(), it.end(), [&](int n) { return (n == static_cast<int>(v+1)); } ) ) {
	  adj_list.push_back(count);
	}
	count++;
      }
      full_graph.push_back( adj_list );
    }

    //
    // Create the auxillary information vector that is needed for each vertex
    //
    vector<Vertex> aux;
    for( int i = 0; i < graph_sz; ++i ) {
      aux.emplace_back( false, INT_MAX, INT_MAX, -1 );
    }

    vector<int> ArtPoints;
    int counter = 1; // since passing by reference, have to have an actual variable
    //
    // if the graph is unconnected, then need to run several times with different
    // roots
    //
    for( int i = 1; i <= graph_sz; ++i ) {
      if(! aux.at(i-1).visited ) {
	FindArt( i, full_graph, aux, ArtPoints, counter );
      }
    }

    //
    // Output Results
    //
    cout << "Graph #" << graph_num++ << endl;
    if(! ArtPoints.empty() ) {
      sort( ArtPoints.begin(), ArtPoints.end() );
      for( auto& it : ArtPoints ) {
	cout << it << " is an articulation point." << endl;
      }
      cout << endl;
    } else {
      cout << "No articulation points (the graph is bi-connected)." << endl;
      cout << endl;
    }
  }
}

int main( int argc, char **argv ) {
  //
  // Check if an argument was passed, if so, use it to open
  // the input file
  //
  if( argc < 2 ) {
    cerr << "Usage: " << argv[0] << " [FILE]" << endl;
    return 0;
  }
  ifstream ifs( argv[1], ifstream::in );
  if(! ifs.good() ) {
    cerr << "Unable to open input file: " << argv[1] << endl;
  }

  //
  // The input file should be adjacency lists separated by
  // commas (',').  So, this gets the full line then tokenizes
  // it using a stringstream.
  //
  string line;
  vector< vector<int> > graph;
  bool skip = false;
  while( ifs.good() ) {
    getline( ifs, line );

    string val;
    stringstream ss;
    vector<int> vertex;
    ss << line; // put the line into the string stream so we can tokenize
    while( getline( ss, val, ',' ) ) {
      int v = stoi( val );
      if( v == -1 ) {
	TarjansAlgorithm::solve( graph );
	graph.clear(); // reset for the next graph in the file
	skip = true;
	break;
      } else {
	if( v > 0 ) {
	  vertex.push_back( v );
	}
      }
    }
    if(! skip ) { // make sure to not add empty vertex after breaking from while loop
      graph.push_back( vertex );
    } else {
      skip = false;
    }
  }
  
  //
  // Need to call again because previous call only
  // applies to multiple graphs in a file.
  //
  graph.pop_back(); // an extra vertex always gets pushed onto the back that shouldn't be there
  TarjansAlgorithm::solve( graph );
  
  return 0;
}
