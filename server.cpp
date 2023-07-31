#include <iostream>
#include <tuple>
#include <SQLiteCpp/SQLiteCpp.h>
#include "crow_all.h"

int serve(void) {

  SQLite::Database db("tfidf.db");
  crow::SimpleApp app;

  // Serve a simple interface
  CROW_ROUTE(app, "/")
    .methods("GET"_method)
    ([](const crow::request& req, crow::response& res) {
      res.set_static_file_info("index.html");
      res.end();
    });

  // handle the query
  CROW_ROUTE(app, "/")
    .methods("POST"_method)
    ([&db](const crow::request& req, crow::response& res) {
      //crow::json::wvalue response;

      SQLite::Statement query(db, "SELECT * FROM tfidf WHERE TERM = ? ORDER BY score DESC LIMIT 10");
      const auto& body = req.body;
      std::vector<std::string> docs;
      std::vector<std::string> abs_paths;
      std::vector<double> scores;
      query.bind(1, body);

      // Execute the query and iterate through the rows returned
      while (query.executeStep()) {
	// Retrieve the values from the current row
	std::string abs_path = query.getColumn(0).getString();
	std::string doc = query.getColumn(1).getString();
	std::string term = query.getColumn(2).getString();
	double score = query.getColumn(3).getDouble();

	// Do something with the retrieved values, e.g. print them to the console
	std::cout << doc << " " << term << " " << score << std::endl;
	docs.push_back(doc);
	scores.push_back(score);
	abs_paths.push_back(abs_path);
      }
      std::string response = "<ul>";
      for (int i = 0; i < docs.size(); i++) {
	response += "<li>"+ docs[i] + " " + abs_paths[i] +"</li>\n";
      }

      response  += "</ul>";
      res.code = 200;
      std::cout << response << std::endl;

      res.set_header("Content-Type", "text/html");
      res.body = response;
      res.end();
    });

  // Set CORS headers
  // app.after_handle([](const crow::request& req, crow::response& res) {
  //     res.set_header("Access-Control-Allow-Origin", "*");
  //     res.set_header("Access-Control-Allow-Headers", "Origin, X-Requested-With, Content-Type, Accept");
  // });

  // Start the server
  app.port(8080).run();
  return 0;
}
