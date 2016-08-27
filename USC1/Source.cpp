#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <utility>
#include <string.h>
#include <list>
#include "queueHeader.h"
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <sstream>
#include <iterator>

using namespace std;
ofstream outfile("output.txt");

struct vertexNode
{
	int data;
	int length;
	int cost;
	int offsetNumber;
	vertexNode *next;
};


class Graph
{
public:
	Graph(int V);
	int noOfVertices;
	struct vertexNode **node;
	string source;
	string destination;
	string operation;
	string * vertexIndex;
	void setEdges(int s, int d, int c, int o);
	int mapVertexNameToIndex(string s, string d, string m);
	int getIndex(string data);
	bool goalTest(int vertex);
	map<string, string> offsetMap;
	int time;
	string BFS();
	string DFS();
	string UCS();
	string itos(int i);
	pipeQueue * bfsQueuingFunc(pipeQueue *pipes, int nodeData, int parentData);
	pipeQueue * dfsQueuingFunc(pipeQueue *pipes, int nodeData, int parentData, int length);
	pipeQueue * ucsQueuingFunc(pipeQueue *pipes, int nodeData, int parentData, int length, string *vertexIndex);
	bool checkIfChildrenInExplored(pipeQueue *exploredPipe, int info);
	struct vertexNode * getNewChildNode(pipeQueue *exploredPipe, int info);
	bool isPipeActive(int time, string s, string d);
	bool checkIfNoChildren(int info);

};

Graph::Graph(int V)
{
	noOfVertices = V;
	struct vertexNode *N;
	vertexIndex = new string[noOfVertices];
	node = new vertexNode *[noOfVertices];
	for (int i = 0; i < noOfVertices; i++) {
		N = (struct vertexNode*) malloc(sizeof(vertexNode));
		node[i] = (vertexNode*)N;
		node[i]->next = NULL;
		node[i]->data = 0;
		node[i]->length = 0;
	}
}


void Graph::setEdges(int s, int d, int c, int o)
{
	struct vertexNode *tempNode = new vertexNode();
	struct vertexNode *first = new vertexNode();
	struct vertexNode *temp = new vertexNode();
	tempNode->data = d;
	tempNode->next = NULL;
	tempNode->cost = c;
	tempNode->length = 0;
	tempNode->offsetNumber = o;
	first = node[s];
	temp = node[s];
	while (first->next != NULL)
	{

		if (vertexIndex[first->next->data].compare(vertexIndex[d]) < 0) {
			//cout << "FIRST->data " << vertexIndex[first->next->data] << " Compared data " << vertexIndex[d];
			temp = first;
			first = first->next;
		}
		else {
			//cout << "ELSE condition FIRST->data " << vertexIndex[first->next->data] << " Compared data " << vertexIndex[d];

			temp = first;
			first = first->next;
			temp->next = tempNode;
			tempNode->next = first;

			return;
		}


	}

	first->next = tempNode;
	//cout << "FIRST TIME " << first->next->data;

}


int Graph::mapVertexNameToIndex(string source, string destination, string middleNodes)
{
	//cout << "Destination : " << destination;
	int i = 1;
	vertexIndex[0] = source;
	//cout << vertexIndex[0];

	string delimiter = " ";
	size_t pos = 0;
	string token;
	while ((pos = middleNodes.find(delimiter)) != std::string::npos) {
		token = middleNodes.substr(0, pos);
		vertexIndex[i] = token;
		//cout << " " << i << vertexIndex[i];
		middleNodes.erase(0, pos + delimiter.length());
		i++;
	}
	vertexIndex[i] = middleNodes;
	//cout << " " << i << vertexIndex[i];
	i++;

	while ((pos = destination.find(delimiter)) != std::string::npos) {
		token = destination.substr(0, pos);
		vertexIndex[i] = token;
		//cout << " " << i << vertexIndex[i];
		destination.erase(0, pos + delimiter.length());
		i++;
	}
	vertexIndex[i] = destination;
	//cout << " " << i << vertexIndex[i];
	i++;

	//cout << vertexIndex[0] << vertexIndex[1] << vertexIndex[2] << vertexIndex[3];

	return i;
}

int Graph::getIndex(string Element)
{
	//cout << "Get index";
	for (int i = 0; i < noOfVertices; i++)
	{
		if (Element.compare(vertexIndex[i]) == 0)
		{
			return i;
			//cout << i << " ";
		}
	}
	return -1;
}


int calculatePath(int node, pipeQueue *exploredPipe) {

	int length = 1;
	//cout << "Calculate path node = " << node << " ";

	while (node != 0) {

		//cout << exploredPipe->searchQueue(node) << " ";
		node = exploredPipe->searchQueue(node);
		length++;
	}

	//cout << "LENGTH of path is " << length;
	return length;
}

string Graph::itos(int i) {

	stringstream s;
	s << i;
	return s.str();
}

string Graph::BFS() {

	pipeQueue *frontierPipe = new pipeQueue();
	pipeQueue *exploredPipe = new pipeQueue();
	struct vertexNode *p = new vertexNode();
	struct pipeNode *removedNode = new pipeNode();
	bool endOfSearch = false;
	int lengthPath = 0;
	//check if the first node is the solution
	frontierPipe->insertQueue(0, 0);

	do
	{
		if (frontierPipe->isEmpty()) {
			string out = "None";
			outfile << out << std::endl;
			return out;
		}

		removedNode = frontierPipe->removeQueue();
		exploredPipe->insertQueue(removedNode->info, removedNode->parentInfo);
		if (node[removedNode->info]->next != NULL) {
			p = node[removedNode->info]->next;

			while (p != NULL) {
				if (!exploredPipe->checkIfNodeExists(p->data) && !frontierPipe->checkIfNodeExists(p->data)) {
					if (goalTest(p->data)) {
						time = time + calculatePath(removedNode->info, exploredPipe);
						//cout << "\n Yayyy BFS" << vertexIndex[p->data] << "is the vertex and its parent is " << removedNode->info << "and the time taken is " << time << "\n";
						int totalTime = time % 24;
						outfile << vertexIndex[p->data] << " " << totalTime << std::endl;


						//istringstream iss;
						//iss >> totalTime;
						string out = vertexIndex[p->data] + " " + itos(totalTime) + "\n";
						//string out = vertexIndex[p->data] + "\n";
						return out;
					}
					frontierPipe = bfsQueuingFunc(frontierPipe, p->data, removedNode->info);
				}
				p = p->next;
			}
		}
		lengthPath = lengthPath++;

	} while (true);

	delete p;
	delete frontierPipe;
}

pipeQueue * Graph::bfsQueuingFunc(pipeQueue *pipes, int nodeData, int parentData) {

	pipes->insertQueue(nodeData, parentData);
	return pipes;
}

bool Graph::checkIfNoChildren(int info) {

	if (node[info]->next == NULL) {
		//cout << "No children";
		return true;
	}
	else
		return false;
}

bool Graph::checkIfChildrenInExplored(pipeQueue *exploredPipe, int info) {

	if (exploredPipe->front == NULL) {
		return false;
	}
	struct vertexNode *p = new vertexNode();
	struct pipeNode *temp = new pipeNode;
	temp = exploredPipe->front;
	p = node[info]->next;

	while (p != NULL) {
		if (!exploredPipe->checkIfNodeExists(p->data))
			return false;
		p = p->next;
	}

	//cout << "Children in explored set";
	return true;

}

struct vertexNode * Graph::getNewChildNode(pipeQueue *exploredPipe, int info) {

	struct vertexNode *p = new vertexNode();
	struct pipeNode *temp = new pipeNode;
	p = node[info]->next;
	temp = exploredPipe->front;

	while (p != NULL) {

		if (exploredPipe->checkIfNodeExists(p->data)) {
			p = p->next;
		}
		else {
			//cout << "In get child function " << p->data;
			return p;
		}
	}
}

string Graph::DFS() {

	pipeQueue *frontierPipe = new pipeQueue();
	pipeQueue *exploredPipe = new pipeQueue();
	struct vertexNode *p = new vertexNode();
	struct pipeNode *removedNode = new pipeNode();
	struct pipeNode *topNode = new pipeNode();
	int nodeValue;

	frontierPipe->insertFrontQueue(0, 0, 0);
	//cout << "After inserting 1st element";

	do {
		if (frontierPipe->isEmpty()) {
			//cout << "Fail!!!!";
			string out = "None";
			outfile << out << std::endl;
			return out;
		}

		topNode = frontierPipe->firstElement();
		//cout << "\n\nTop Frontier Node = " << topNode->info << "\n";
		if (goalTest(topNode->info)) {
			time = (time + topNode->length) % 24;
			//cout << "Yayyyy  " << vertexIndex[topNode->info] << "and time is " << time;
			string out = vertexIndex[topNode->info];
			outfile << vertexIndex[topNode->info] << " " << time << std::endl;
			return out;
		}

		if (checkIfNoChildren(topNode->info) || checkIfChildrenInExplored(exploredPipe, topNode->info)) {
			//cout << "does it enter";
			removedNode = frontierPipe->removeQueue();
			//cout << "\n Removed node " << removedNode->info << "\n";
			exploredPipe->insertQueue(removedNode->info, removedNode->parentInfo);
			//check for the goal //if yes break else //add this to the explored set
		}
		else {
			p = getNewChildNode(exploredPipe, topNode->info);
			p->length = topNode->length + 1;
			//cout << "\n Element added to the queue " << p->data << "\n";
			if (!exploredPipe->checkIfNodeExists(p->data) && !frontierPipe->checkIfNodeExists(p->data)) {
				frontierPipe = dfsQueuingFunc(frontierPipe, p->data, removedNode->info, p->length);
			}
			else {
				removedNode = frontierPipe->removeQueue();
				exploredPipe->insertQueue(removedNode->info, removedNode->parentInfo);
			}
		}
	} while (true);

}


pipeQueue * Graph::dfsQueuingFunc(pipeQueue *pipes, int nodeData, int parentData, int length) {

	pipes->insertFrontQueue(nodeData, parentData, length);
	return pipes;
}

string Graph::UCS() {

	pipeQueue *frontierPipe = new pipeQueue();
	pipeQueue *exploredPipe = new pipeQueue();
	struct vertexNode *p = new vertexNode();
	struct pipeNode *removedNode = new pipeNode();
	bool endOfSearch = false;
	int lengthPath = 0;
	int presentTime = 0;
	//check if the first node is the solution
	frontierPipe->insertUCSQueue(0, 0, 0, vertexIndex);

	do
	{
		if (frontierPipe->isEmpty()) {
			//cout << "Fail!!!!";
			string out = "None";
			outfile << out << std::endl;
			return out;
		}

		removedNode = frontierPipe->removeQueue();
		//cout << "\nRemoved node is " << vertexIndex[removedNode->info] << "and its length is " << removedNode->length << "\n";
		if (removedNode == NULL) {
			//cout << "ITS NULL";
			string out = "None";
			outfile << out << std::endl;
			return out;
		}
		if (goalTest(removedNode->info)) {
			//cout << "Yayyy UCS" << vertexIndex[removedNode->info] << "is the vertex and its parent is " << removedNode->parentInfo << "  The time is " << (removedNode->length + time) % 24;
			//calculatePath(removedNode->info, exploredPipe);

			int totalTime = (removedNode->length + time) % 24;
			outfile << vertexIndex[removedNode->info] << " " << totalTime << std::endl;
			//istringstream iss;
			//iss >> totalTime;
			string out = vertexIndex[removedNode->info] + " " + itos(totalTime) + "\n";
			//string out = vertexIndex[removedNode->info] + "\n";
			return out;
		}
		exploredPipe->insertQueue(removedNode->info, removedNode->parentInfo);
		if (node[removedNode->info]->next != NULL) {
			p = node[removedNode->info]->next;
			p->length = removedNode->length + p->cost;
			//cout << "Data = " << vertexIndex[p->data] << "Cost = " << p->cost << "\n parent length = " << removedNode->length << "\n chid length = " << p->length << "\n";

			while (p != NULL) {

				p->length = removedNode->length + p->cost;
				presentTime = (removedNode->length + time) % 24;
				//presentTime = (p->length + time) % 24;
				//cout << "\nFor pipe " << p->data << " presentTime = " << presentTime << "and offsetnumber is " << p->offsetNumber;
				if (p->offsetNumber == 0 || isPipeActive(presentTime, vertexIndex[removedNode->info], vertexIndex[p->data])) {
					if (!exploredPipe->checkIfNodeExists(p->data) && !frontierPipe->checkIfNodeExists(p->data)) {
						//cout << "\n Inserted node " << vertexIndex[p->data] << "and its length is " << p->length << "\n";
						frontierPipe = ucsQueuingFunc(frontierPipe, p->data, removedNode->info, p->length, vertexIndex);
						//cout << "\n\nThe frontier queue is \n";
						frontierPipe->display();
					}
					else if (frontierPipe->checkIfGreaterNodeExists(p->data, p->length)) {
						//cout << "comeeee";
						frontierPipe->display();
						frontierPipe->replaceFrontier(p->data, p->length, vertexIndex);
						//cout << "\n\nThe frontier queue is \n";
						frontierPipe->display();
					}
				}

				p = p->next;

			}
		}

	} while (true);

	delete p;
	delete frontierPipe;
}

pipeQueue * Graph::ucsQueuingFunc(pipeQueue *pipes, int nodeData, int parentData, int length, string * vertexIndex) {

	pipes->insertUCSQueue(nodeData, parentData, length, vertexIndex);
	return pipes;
}

bool Graph::goalTest(int vertex)
{
	string node = vertexIndex[vertex];
	//cout << "Goal test Node " << node;
	size_t found = destination.find(node);
	//cout << " Destination is " << destination;
	istringstream iss(destination);
	string token;

	do {
		iss >> token;
		if (token.compare(node) == 0)
			return true;
	} while (iss);

	return false;
	//cout << "Enter goal test";
}

bool Graph::isPipeActive(int presentTime, string source, string dest) {

	//cout << "grrrrrr";
	string path = source + " " + dest;
	string offsets = offsetMap.at(path);

	string firstDelimiter = " ";
	size_t firstPos = 0;
	string token;
	string secDelimiter = "-";
	size_t secPos = 0;
	int first;
	int second;

	//cout << "Enter madi" << offsets;
	while ((firstPos = offsets.find(firstDelimiter)) != std::string::npos) {
		token = offsets.substr(0, firstPos);
		secPos = token.find(secDelimiter);
		first = atoi(token.substr(0, secPos).c_str());
		token.erase(0, secPos + secDelimiter.length());
		second = atoi(token.c_str());

		if (first - 1 < presentTime && presentTime < second + 1)
		{
			//cout << "\nIn pipe active func " << dest << "is false";
			return false;
		}

		offsets.erase(0, firstPos + firstDelimiter.length());
	}

	secPos = offsets.find(secDelimiter);
	first = atoi(offsets.substr(0, secPos).c_str());
	offsets.erase(0, secPos + secDelimiter.length());
	second = atoi(offsets.c_str());

	if (first - 1 < presentTime && presentTime < second + 1)
	{
		//cout << "\nIn pipe active func " << dest << "is false";
		return false;
	}

	//cout << "\nIn pipe active func " << dest << "is true";
	return true;

}


int main(int argc, char *argv[])
{
	ifstream inFile(argv[argc - 1]);
	string line;
	int noOfOperation;
	string operation;
	string s;
	string d;
	string m;
	int noOfEdges;
	int cost;
	string outputString;
	getline(inFile, line);
	noOfOperation = atoi(line.c_str());
	for (int i = 1; i <= noOfOperation; i++) {

		getline(inFile, line);
		operation = line;
		//cout << operation << "\n";

		getline(inFile, line);
		s = line;
		//cout << s << "\n";

		getline(inFile, line);
		d = line;
		//cout << d << "\n";

		getline(inFile, line);
		m = line;
		//cout << m << "\n";

		string nodes = s + " " + m + " " + d;
		istringstream buf(nodes);
		istream_iterator<string> beg(buf), end;
		vector<std::string> tokens(beg, end);
		int x = tokens.size();
		Graph g(x);
		g.destination = d;
		g.operation = operation;
		g.noOfVertices = g.mapVertexNameToIndex(s, d, m);
		//cout << "NO OF VERTICES" << g.noOfVertices << " ";


		getline(inFile, line);
		noOfEdges = atoi(line.c_str());
		//cout << noOfEdges << "\n";

		for (int i = 0; i < noOfEdges; i++) {
			getline(inFile, line);
			istringstream iss(line);
			string vertex[2];
			int cost;
			int offsetNumber;


			iss >> vertex[0];
			iss >> vertex[1];
			iss >> cost;
			//cout << vertex[0] << " " << vertex[1] << " " << cost << "\n";

			//cout << "Its here";
			iss >> offsetNumber;

			g.setEdges(g.getIndex(vertex[0]), g.getIndex(vertex[1]), cost, offsetNumber);


			if (operation.compare("UCS") == 0) {

				//cout << "Enters UCS";

				//vector<string> offset(offsetNumber);
				if (offsetNumber != 0)
				{
					string offset;
					string temp;
					iss >> temp;
					offset = temp;
					for (int j = 1; j < offsetNumber; j++) {
						iss >> temp;
						offset = offset + " " + temp;
					}
					//cout << "\nPRINT\n";
					//cout << offset;
					string a = vertex[0] + " " + vertex[1];
					//cout << a << "\n";
					g.offsetMap.insert(pair<string, string>(a, offset));
					//cout << "\nOkay u have GOT to work now\n";
					string uff = g.offsetMap.at(a);
					//cout << uff << "\n";
				}
			}
		}

		getline(inFile, line);
		g.time = atoi(line.c_str());
		getline(inFile, line);

		if (operation == "DFS") {
			outputString = g.DFS();
			//cout << "\nOUTPUT STRING " + outputString;
		}
		else if (operation == "BFS") {
			outputString = g.BFS();
			//cout << "\nOUTPUT STRING " + outputString;
		}
		else if (operation == "UCS") {
			//cout << "USC";
			outputString = g.UCS();

			//cout << "\nOUTPUT STRING " + outputString;
		}
	}

	outfile.close();
	return 0;

}
