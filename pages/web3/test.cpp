#include <sys/stat.h>
#include <iostream>

using namespace std;

bool isDirectory(const string& path) {
  struct stat statbuf;

  // Attempt to get file information using stat()
  if (stat(path.c_str(), &statbuf) == -1) {
    cerr << "Error: stat() failed for path: " << path << endl;
    return false;
  }

  // Use S_ISDIR macro to check for directory type
  return S_ISDIR(statbuf.st_mode);
}

int main() {
  string path;

  cout << "Enter a path: ";
  getline(cin, path);

  if (isDirectory(path)) {
    cout << path << " is a directory." << endl;
  } else {
    cout << path << " is not a directory." << endl;
  }

  return 0;
}