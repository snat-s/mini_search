#include "index.cpp"
#include "server.cpp"
#include <iostream>
#include <cstring>

/*
  Create a simple crawler. Currently The only
  thing that we do is serve the directory, but I want
  to be able to serve recursively a directory.
 */

int main(int argc, char* argv[]) {
 
  if (argc == 1) {
    std::cout << "run with a command. If there is a need for help, run mini_se help" << std::endl;
    return 0;
  }

  if (!std::strcmp(argv[1], "index")) {
    std::cout << "Running index function on: " << argv[2] << std::endl;
    index(argv[2]);
    std::cout << "Done." << std::endl;
  } else if (!std::strcmp(argv[1], "serve")) {
    std::cout << "Running a server in port 8080" << std::endl;
    serve();
    std::cout << "Done." << std::endl;
  } else if (!std::strcmp(argv[1], "help")) {
	  std::cout << "mini_se index <path/to/directory> to index a directory" << std::endl;
	  std::cout << "mini_se serve to run webpage" << std::endl;
	  std::cout << "mini_se help to display this message" << std::endl;
  }

  return 0;
}
