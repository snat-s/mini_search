#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <map>
#include <set>
#include <string>
#include <filesystem>
#include <algorithm>
#include <SQLiteCpp/SQLiteCpp.h>

namespace fs = std::filesystem;
using namespace std;

void calc_tf(vector<string>& doc, map<string, double>& tf);
void calc_idf(vector<vector<string>>&docs, map<string,double>&idf);
void calc_tf_idf(map<string, double>&tf, map<string, double> &idf, map<string, double> &tf_idf);
vector<string> parse_document(string filename);

int index(std::string currentPath) {
  std::set<std::string> extensions = {".html", ".org", ".md"};
  /*
    Next session:
    Implement sqlite backend.
  */
  // create the database
  SQLite::Database db("tfidf.db", SQLite::OPEN_CREATE | SQLite::OPEN_READWRITE);
  
  db.exec("CREATE TABLE IF NOT EXISTS tfidf (abs_path TEXT, doc TEXT, term TEXT, score TEXT)");

  /*
    Mini crawler.
  */

  fs::path directoryPath(currentPath);
  
  vector<vector<string>> docs;

  for (const auto& entry : fs::recursive_directory_iterator(directoryPath)) {

    if (entry.is_regular_file()) {

      std::string filename = entry.path().filename().string();

      bool is_parsing = false;

      // Get the filename and check if it ends with any extension
      for (const auto& extension : extensions) {
	if (filename.find(extension) != std::string::npos) {
	  is_parsing = true;
	  break;
	}
      }
      
      if (is_parsing) {

	vector<string> words = parse_document(entry.path());
	docs.push_back(words);
	map<string, double> tf, idf, tf_idf;
	calc_tf(words, tf);
	calc_idf(docs, idf);
	calc_tf_idf(tf, idf, tf_idf);

	try {
	  db.exec("BEGIN");
	  SQLite::Statement statement(db, "INSERT INTO tfidf (abs_path, doc, term, score) VALUES (?, ?, ?, ?)");
	  // save the tf-idf scores to this database

	  for (auto term : tf_idf) {

	    if (term.first.length() > 0) {
	      statement.bind(1, fs::absolute(entry).string());
	      statement.bind(2, entry.path().string());
	      statement.bind(3, term.first);
	      statement.bind(4, term.second);
	      statement.exec();

	      
	      // Wait for the statement to finish
	      statement.reset();

	    }
	  }
	  db.exec("COMMIT");
	} catch (const SQLite::Exception& e) {
	  std::cerr << "SQLite exception: " << e.what() << std::endl;

	}
      }
    }
  }

  cout << "Indexed: " << docs.size() << " documents."<< endl;

  return 0;
}

// Calculates the term frequency (tf) of each term in a document
void calc_tf(vector<string>& doc, map<string, double>& tf) {
  int total_words = doc.size();
  for (auto word : doc) {
    tf[word] += 1.0 / total_words;
  }
}

vector<string> parse_document(string filename) {
  ifstream in_file(filename);
  vector<string> words;
  string word;

  while (in_file >> word) {

    string parsed_string;
    for (char x : word) {
      if ((x >= 'a' && x <= 'z')) {
	parsed_string += x;
      } else if (x >= 'A' && x <= 'Z') {
	parsed_string += x + 32; // to lowercase
      }
    }
    words.push_back(parsed_string);
  }

  return words;
}

void calc_idf(vector<vector<string>>&docs, map<string,double>&idf) {

  map<string, int> term_doc_count;
  for (auto doc : docs) {
    map<string, bool> terms;
    for (auto word : doc) {
      terms[word] = true;
    }
    for (auto term : terms) {
      term_doc_count[term.first] += 1;
    }
  }
  int number_docs = docs.size();
  for (auto term : term_doc_count) {
    // cout << log((number_docs/ (double) term.second)) << endl;
    idf[term.first] = log(number_docs/ (double) term.second);
  }

}

void calc_tf_idf(map<string, double>&tf, map<string, double> &idf, map<string, double> &tf_idf) {
  for (auto term : tf) {
    // maybe there is a cleaner way of doing this
    tf_idf[term.first] = term.second * idf[term.first];
  }
}
