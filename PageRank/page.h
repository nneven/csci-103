#ifndef PAGE_H
#define PAGE_H
#include <string>
using std::string;
#include<vector>
using std::vector;

class Page { 
public:
	Page(int id, string url, double rank, vector<int> links);
	//setters and getters
	int get_id();
	string get_url();
	double get_rank();
	vector<int> get_links();
	void set_rank(double rank);

private:
	//Data members: id, url, pagerank, links , .. etc
	int id;
	string url;
	double rank;
	vector<int> links;
};  
#endif