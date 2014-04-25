#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

void solve( vector<vector<int> >& graph ) {
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

  for( auto it : full_graph ) {
    for( auto itt : it ) {
      cout << itt << " ";
    }
    cout << endl;
  }
}

int main( int argc, char **argv ) {
  if( argc < 2 ) {
    cerr << "Usage: " << argv[0] << " [FILE]" << endl;
    return 0;
  }

  ifstream ifs( argv[1], ifstream::in );
  if(! ifs.good() ) {
    cerr << "Unable to open input file: " << argv[1] << endl;
  }

  string line;
  vector< vector<int> > graph;

  while( ifs.good() ) {
    getline( ifs, line );
    
    string val;
    stringstream ss;
    vector<int> vertex;
    ss << line;
    while( getline( ss, val, ',' ) ) {
      int v = stoi( val );
      if( v == -1 ) {
	solve( graph );
	graph.clear();
	break;
      } else {
	vertex.push_back( v );
      }
    }
    graph.push_back( vertex );
  }

  //solve( graph );
  
  return 0;
}
