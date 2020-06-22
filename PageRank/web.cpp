#include "web.h"

Web::Web() 
{ 

}

Web::~Web() 
{
	for (int i = 0; i < pages.size(); i++) {
        delete pages[i];
    }
}

bool Web::read_graph(const char *filename) 
{ 
	fstream ifile(filename);
	if(ifile.fail()) return false;
	ifile >> total;
	if(ifile.fail()) return false;
	int id, num;
	string url, line;
	double rank;
	vector<int> links;

	for (int i = 0; i < total; i++) {
		vector<int> links;
		ifile >> id >> url >> rank;
		getline(ifile, line);
		getline(ifile, line);
		stringstream ss(line);
		while(ss >> num) {
			links.push_back(num);
		}
        
		Page* p = new Page(id, url, rank, links);
		pages.push_back(p);
	}
	return true;
}

bool Web::write_graph(const char *filename)
{
	ofstream ofile(filename);
	if(ofile.fail()) return false;
	ofile << total << endl;

	for(int i = 0; i < total; i++) {
		ofile << pages[i]->get_id() << endl;
		ofile << '\t' << pages[i]->get_url() << endl;
		ofile << '\t' << pages[i]->get_rank() << endl;
		ofile << '\t';

		for (int j = 0; j < pages[i]->get_links().size(); j++) {
			ofile << pages[i]->get_links()[j] << " ";
		}
		ofile << endl;
	}
	return true;
}

void Web::calculate_rank(int S, int N)
{
	int num = N / pages.size();
	int *walkers = new int[pages.size()];
	int *moves = new int[pages.size()];
	for (int i = 0; i < pages.size(); i++) {
		walkers[i] = num;
		moves[i] = num;
	}

	for (int i = 0; i < S; i++) {
		for (int j = 0; j < pages.size(); j++) {
			while (walkers[j] > 0) {
                if (pages[j]->get_links().size() != 0) {
                    int r = rand() % pages[j]->get_links().size();
                    r = pages[j]->get_links()[r];
                    moves[r]++;
                    moves[j]--;
                }
                walkers[j]--;
			}
		}
		for (int j = 0; j < pages.size(); j++) {
			walkers[j] = moves[j];
		}
	}

	for (int i = 0; i < pages.size(); i++) {
		pages[i]->set_rank((double)walkers[i] / N);
	}

	delete[] walkers;
	delete[] moves;
}