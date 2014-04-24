#include <iostream>
#include <fstream>
#include <string>
#include <vertex>

#define N 100

using namespace std;

void solve() {
  static int counter = 0;
  cout << counter++ << endl;
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
  int graph[N][N];
  int count = 0;

  while( ifs >> line ) {
    if( stoi( line ) == -1 ) {
      solve();
    } else {
      ++count;
    }
  }
  solve();
  
  return 0;
}
