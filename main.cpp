#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

#define ROOT 1

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
  aux.at(v-1).visited = true;
  aux.at(v-1).low = aux.at(v-1).num = counter++; // Rule 1
  auto& row = graph.at(v-1);
  for_each( row.begin(), row.end(), [&](int w) {
      if(! aux.at(w-1).visited ) { // forward edge
	aux.at(w-1).parent = v;
	FindArt( w, graph, aux, ArtPoints, counter );
	if( ArtPoints.end() == find( ArtPoints.begin(), ArtPoints.end(), v ) ) { // check if already found that it is an articulation point
	  if( ( (aux.at(v-1).parent == -1) && (row.size() > 1) ) || // root check
	      ( (aux.at(v-1).parent != -1) && aux.at(w-1).low >= aux.at(v-1).num ) ) { // otherwise
	    ArtPoints.push_back( v );
	  }
	}
	aux.at(v-1).low = min( aux.at(v-1).low, aux.at(w-1).low ); // Rule 3
      } else {
	if( aux.at(v-1).parent != w  ) { // back edge
	  aux.at(v-1).low = min( aux.at(v-1).low, aux.at(w-1).num ); // Rule 2
	}
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
  graph.push_back( vector<int>(0) );
  vector<vector<int> > full_graph;
  full_graph.reserve( graph.size() );

  for( size_t v = 0; v < graph.size(); ++v ) {
    auto row = graph.at(v);
    vector<int> adj_list;
    for_each( row.begin(), row.end(), [&](int n) { if( n != 0 ) { adj_list.push_back(n); } } );
    for( int i = 0; i < static_cast<int>(v); ++i ) {
      row = graph.at(i);
      if( any_of( row.begin(), row.end(), [&](int n) { return (n == static_cast<int>(v+1)); } ) ) {
	adj_list.push_back(i+1);
      }
    }
    sort( adj_list.begin(), adj_list.end() );
    full_graph.push_back( adj_list );
  }

  vector<Vertex> aux;
  for( size_t i = 0; i < full_graph.size(); ++i ) {
    Vertex v = { false, -1, -1, -1 };
    aux.push_back( v );
  }

  vector<int> ArtPoints;
  int counter = 1;

  for( int i = 1; i <= static_cast<int>( full_graph.size() ); ++i ) {
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
  while( ifs.good() ) {
    getline( ifs, line );
    string val;
    stringstream ss;
    vector<int> vertex;
    ss << line; // put the line into the string stream so we can tokenize
    while( getline( ss, val, ',' ) ) {
      int v = stoi( val );
      if( v == -1 ) {
	solve( graph );
	graph.clear(); // reset for the next graph in the file
	break;
      } else {
	if( v > 0 ) {
	  vertex.push_back( v );
	}
      }
    }
    graph.push_back( vertex );
  }
  
  //
  // Need to call again because previous call only
  // applies to multiple graphs in a file.
  //
  solve( graph );
  
  return 0;
}
