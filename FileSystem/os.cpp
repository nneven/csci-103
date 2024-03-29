#include <iostream>
#include "os.h"
using namespace std;
//------------------------------------------------------------------------------
//IMPORTANT: You are not allowed to remove or add any parameters to any functions.
//------------------------------------------------------------------------------

/*
Constructor
	Dynamically allocate root data.
	set isFolder = true.
	dynamically allocate Mlist for our root data as well (since it's a directory type).
    push our newly created data object pointer to wd and dataStack from the top.
*/
OS::OS() {
	root_data = new Data;
	root_data->name = "root";
	root_data->isFolder = true;
	root_data->childList = new MList;
	wd.push_top(root_data);
	dataStack.push_top(root_data);
}

/*
Destructor to clean-up memory, i will leave this for you to figure it out.
*/
OS::~OS(){
	while (dataStack.top() != NULL) {
		if (dataStack.top()->nodeData->isFolder) delete dataStack.top()->nodeData->childList;
		delete dataStack.top()->nodeData;
		dataStack.pop_top();
	}
}

/*
Search a node in the current directory
	If one is found, return a pointer to that node
	If one is not found, return NULL
*/
Node* OS::search_item(string fname) {

	return wd.top()->nodeData->childList->search(wd.top()->nodeData->childList->top(), fname);
}

/*
Delete a node in the current directory
	Note: this function only deletes (permanently) the node, not the Data obj the node points to
	If the item you want to delete doesn't exist in the current directly, do

		cout<<"Error: cannot find file or directory '"<<fname<<"'"<<endl;
*/
void OS::del(string fname) {
	if (search_item(fname) == NULL) {
		cout << "Error: cannot find file or directory '" << fname << "'" << endl;
	} else {
		wd.top()->nodeData->childList->deleteNode(search_item(fname));
	}
}

/*
Create a file or a folder, use boolean isFolder to tell (true if folder, false if file)
Things to keep in mind:
	1). Don't forget to search for a node in the current directory with the same name first.
		if the name already exists, do:

				cout<<"Directory or file name '"<<fname<<"' already exists"<<endl;

	2). If you are creating a folder, make sure to allocate a memory for its MList object
		and set the boolean isFolder appropriately.
    3). At this point you should initialize the size of file and folder to 0
	4). Once the data object is created, add the pointer to that to dataStack from the "top" 
		and add the node to the current directory list from the "bottom".
	5). Once added, sort the current directory list by name.
*/
void OS::create_item(string fname, bool isFolder) {
	if (search_item(fname)) {
		cout << "Directory of file name '" << fname << "' already exists" << endl;
	} else {
		Data* data = new Data;
		data->name = fname;
		data->isFolder = isFolder;
		data->size = 0;
		if (isFolder) data->childList = new MList;
		wd.top()->nodeData->childList->push_bottom(data);
		dataStack.push_top(data);
		wd.top()->nodeData->childList->sortByNameInsertion();
	}
}

/*
Read or write a file according to the boolean isRead (true = read, false = write)
Things to keep in mind:
	1). make sure that a file "fname" exists in our current directly, if not

			cout<<"Error: cannot find file '"<<fname<<"'"<<endl;

	2). if it exists, make sure that it is a file type, not a folder type. If it is a folder type,

			cout<<"Error: '"<<fname<<"' is not a file"<<endl;

	3). read mode is simply just cout<<...text...<<endl;
	4). for write mode you need to allow user input, use these 3 lines below:

                cout<<"$> ";
				string input;
				getline(cin,input);

		then simply just set text to the input string.
	5). the size of the file should be based on the length of the input string.
*/
void OS::file(string fname, bool isRead) {
	if (search_item(fname) == NULL) {
		cout << "Error: cannot find file '" << fname << "'" << endl;
	} else if (search_item(fname)->nodeData->isFolder) {
		cout << "Error: '" << fname << "' is not a file" << endl;
	}  else if (isRead) {
		cout << search_item(fname)->nodeData->text << endl;
	} else {
		cout << "$> ";
		string input;
		getline(cin, input);
		search_item(fname)->nodeData->text = input;
		search_item(fname)->nodeData->size = input.length();
	}
}


//Change directory
void OS::cd_dir(string fname) {
	if(fname == "..") {
		//this option takes you up one directory (move the directory back one directory)
		//you should not be able to go back anymore if you are currently at root.
        if (wd.top()->nodeData != root_data) wd.pop_top();

	} else if(fname == "~") {
		//this option takes you directly to the home directory (i.e., root).
        while (wd.top()->nodeData != root_data) wd.pop_top();

	} else {
		/*
			This option means that you are trying to access (go into) a directory
			1). check whether there exists a node with that name, if you cannot find it:

					cout<<"Error: cannot find directory '"<<fname<<"'"<<endl;

			2). if it does exist, check whether it is a folder type. If it is a file type:

					cout<<"Error: '"<<fname<<"' is not a directory"<<endl;
			
			3). after checking both things, you can safely move to another directory
		*/
		if (search_item(fname) == NULL) {
			cout << "Error: cannot find directory '" << fname << "'" << endl;
		} else if (!search_item(fname)->nodeData->isFolder) {
			cout << "Error: '" << fname << "' is not a directory" << endl;
		} else {
			wd.push_top(search_item(fname)->nodeData);
		}
	}
}

//Print list of item in the current directory, the way you print it will be according to the passed-in option
void OS::ls(string option) {
	if (option == "-d") {
		//Default option - print the list of items in the current directory from top to bottom.
		//use a single space as delimiter.
		wd.top()->nodeData->childList->printTtB(" ");
	} else if(option == "-sort=name") {
		//Use Insertion Sort to sort items in the current directory and print from top to bottom (alphabetically A-Za-z).
		//use a single space as delimiter.
		wd.top()->nodeData->childList->sortByNameInsertion();
		wd.top()->nodeData->childList->printTtB(" ");
	} else if(option == "-r") {
		//Reverse print the list of items in the current directory (i.e., print form bottom to top).
		//use single space as delimiter.
		wd.top()->nodeData->childList->printBtT(" ");
	} else if(option == "-sort=size") {
		//Sort list by size and print the list of items in the current directory from top to bottom.
		//use single space as delimiter.
        wd.top()->nodeData->childList->sortBySizeSelection();
        wd.top()->nodeData->childList->printTtB(" ");
	} else {
		cout<<"usage: ls [optional: -r for reverse list, -sort=size to sort list by file size, -sort=name to soft list by name]";
	}
}

//Priting path from root to your current directory.
//use slash "/" as our delimiter.
//Example output: root/cs103/usc/viterbi/
void OS::present_working_dir() {
	wd.printBtT("/");
}