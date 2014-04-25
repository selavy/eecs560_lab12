#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

void solve( vector< vector<int> >& graph ) {
  size_t counter = 1;
  queue<int> Q;
  vector<bool> visited; // maybe use bitset?
  graph.push_back( vector<int>(0) );
  vector<int> lows;
  lows.reserve( graph.size() );
  visited.reserve( graph.size() );
  for( size_t i = 0; i < graph.size(); ++i ) {
    visited.push_back( false );
    lows.push_back( 0 );
  }

  Q.push( 1 );
  cout << "Graph size = " << static_cast<int>( graph.size() ) << endl;
  cout << "Visited size = " << static_cast<int>( visited.size() ) << endl;

  while(! Q.empty() ) {
    int v = Q.front();
    Q.pop();
    if( v == 0 ) continue;
    --v;
    if( visited.at(v) ) continue;
    visited.at(v) = true;
    lows.at(v) = counter++;

    int j = 0;
    for( auto it = graph.begin(); (it != graph.end()) && (j <= v); ++j, ++it ) {
      if( j == v ) {
	auto& it = graph.at(v);
	for_each( it.begin(), it.end(), [&](int i) { 
	    if( (i > 0) && !visited.at(i-1) ) {
	      Q.push(i);
	    }
	  });
      }
      else if( any_of( it->begin(), it->end(), [&](int i) { return(i == (v+1)); } ) ) {
	if(! visited.at(j) ) {
	  Q.push(j+1);
	}
      }
    }
  }

  int i = 1;
  for( auto& it : lows ) {
    cout << i << " : " << it << endl;
    ++i;
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
