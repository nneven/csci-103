#include "page.h"

Page::Page(int id, string url, double rank, vector<int> links) 
{
	this->id = id;
	this->url = url;
	this->rank = rank;
	this->links = links;
}

int Page::get_id()
{
	return id;
}

string Page::get_url()
{
	return url;
}

double Page::get_rank()
{
	return rank;
}

vector<int> Page::get_links()
{
	return links;
}

void Page::set_rank(double rank)
{
	this->rank = rank;
}