#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <utility>
#include <string.h>
#include <list>

using namespace std;

struct pipeNode {
	int info;
	int parentInfo;
	int length;
	struct pipeNode *next;
};

class pipeQueue {


public:
	pipeNode *rear;
	pipeNode *front;
	pipeQueue();
	void insertQueue(int nodeData, int parentData);
	void insertFrontQueue(int nodeData, int parentData, int length);
	void insertUCSQueue(int nodeData, int parentData, int length, string * vertexIndex);
	//int removeQueue();
	struct pipeNode * removeQueue();
	bool isEmpty();
	void display();
	bool checkIfNodeExists(int node);
	bool checkIfGreaterNodeExists(int nodeData, int nodeLength);
	void replaceFrontier(int nodeData, int length, string *vertexIndex);
	struct pipeNode * firstElement();
	int searchQueue(int node);
};

pipeQueue::pipeQueue() {
	rear = NULL;
	front = NULL;
}

void pipeQueue::insertQueue(int nodeData, int parentData) {
	//pipeNode *temp = new pipeNode;
	//struct pipeNode *temp;
	//cout << "parameters" << nodeData << " " << parentData;
	struct pipeNode *temp = new pipeNode();
	temp->info = nodeData;
	temp->parentInfo = parentData;
	temp->next = NULL;
	//cout << "parameters" << nodeData << " " << parentData << " ";

	//cout << "TEMP1" << " " << temp->info << " " << temp->parentInfo;
	if (front == NULL)
	{
		front = temp;
		rear = temp;
		//cout << "TEMP2" << temp->info << " " << temp->parentInfo;
		//cout << "Enter insert queue";
	}
	else
	{
		rear->next = temp;
		rear = temp;
		rear->next = NULL;
	}

}

struct pipeNode * pipeQueue::firstElement() {

	struct pipeNode *temp = new pipeNode();
	temp = front;
	return temp;

}


void pipeQueue::insertFrontQueue(int nodeData, int parentData, int length) {
	//pipeNode *temp = new pipeNode;
	//struct pipeNode *temp;
	struct pipeNode *temp1 = new pipeNode;
	struct pipeNode *temp2 = new pipeNode;
	temp1->info = nodeData;
	temp1->parentInfo = parentData;
	temp1->length = length;
	temp1->next = NULL;

	if (front == NULL)
	{
		front = temp1;
		rear = temp1;
	}
	else
	{
		temp1->next = front;
		front = temp1;
		/*temp2 = front->next;
		front->next = temp1;
		temp1->next = temp2;*/

	}

}

void pipeQueue::insertUCSQueue(int nodeData, int parentData, int length, string *vertexIndex) {

	struct pipeNode *temp = new pipeNode();
	temp->info = nodeData;
	temp->parentInfo = parentData;
	temp->length = length;

	struct pipeNode *first = new pipeNode();
	struct pipeNode *second = new pipeNode();

	//cout << "UCS parameter" << nodeData << " " << parentData << " " << length;

	if (front == NULL)
	{
		front = temp;
		rear = temp;
	}
	else {

		if (front->next == NULL) {
			if (front->length < length) {
				front->next = temp;
				return;
			}
			else if (front->length == length) {
				if (vertexIndex[front->info].compare(vertexIndex[nodeData]) > 0) {
					temp->next = front;
					front = temp;
					return;
				}
				else {
					front->next = temp;
					return;
				}
			}
			else {
				temp->next = front;
				front = temp;
				return;
			}
		}
		else {
			first = front;
			second = front->next;
			//cout << "First element is " << first->info << "And its length" << first->length;
			//cout << "\nSecond element is " << second->info << "And its length" << second->length;

			if (first->length > length) {
				temp->next = front;
				front = temp;
				return;
			}

			else if (first->length == length) {
				//cout << "First Length is equal " << length << "and " << vertexIndex[first->info].compare(vertexIndex[nodeData]);
				if (vertexIndex[first->info].compare(vertexIndex[nodeData]) > 0) {
					temp->next = front;
					front = temp;
					return;
				}
				while (second != NULL && second->length == length) {
					if ((vertexIndex[first->info].compare(vertexIndex[nodeData]) < 0)) {
						first = first->next;
						second = second->next;
					}

				}

				first->next = temp;
				temp->next = second;
				return;

			}
			else {
				while (second != NULL) {
					if (second->length > length) {
						first->next = temp;
						temp->next = second;
						return;
					}
					else if (second->length == length) {
						if (vertexIndex[second->info].compare(vertexIndex[nodeData]) > 0) {
							//length is before sec->length
							first->next = temp;
							temp->next = second;
							return;
						}
						else {
							if (second->next != NULL) {
								first = second;
								second = second->next;
								first->next = temp;
								temp->next = second;
								return;
							}
							else {
								second->next = temp;
								return;
							}
						}

					}// end of comparison for equals
					else
					{
						first = second;
						second = second->next;
					}
				}

				first->next = temp;
			}

		}

	}
}


struct pipeNode * pipeQueue::removeQueue() {
	//pipeNode *temp = new pipeNode;
	int data;
	struct pipeNode *temp = new pipeNode();
	if (front == NULL) {
		//cout << "No pipeNode to remove";
		return NULL;
	}
	else {

		temp = front;
		//cout << "Remove queue " << temp->info;
		data = temp->info;
		front = front->next;
		//delete temp;
		return temp;
	}
}

bool pipeQueue::isEmpty() {
	if (front == NULL)
		return true;
	else
		return false;
}

bool pipeQueue::checkIfNodeExists(int node) {

	pipeNode *temp = new pipeNode();
	if (front == NULL) {
		//cout << "No element exists";
		return false;
	}

	temp = front;
	while (temp != NULL) {
		if (node == temp->info)
			return true;
		else
			temp = temp->next;
	}

	return false;
}

bool pipeQueue::checkIfGreaterNodeExists(int nodeData, int length) {

	pipeNode *temp = new pipeNode();
	if (front == NULL) {
		//cout << "No element exists";
		return false;
	}

	temp = front;
	while (temp != NULL) {
		if (nodeData == temp->info) {
			if (temp->length > length) {
				return true;
			}
			else
				return false;
		}

		temp = temp->next;
	}

	return false;
}

void pipeQueue::replaceFrontier(int nodeData, int length, string *vertexIndex) {

	pipeNode *temp = new pipeNode();
	pipeNode *prev = new pipeNode();
	if (front == NULL) {
		//cout << "No element exists";
		return;
	}

	temp = front;
	if (temp->next == NULL) {
		if (nodeData == temp->info) {
			temp->length = length;
			return;
		}
	}
	else {

		if (nodeData == temp->info) {
			temp->length = length;
			front = temp->next;
			insertUCSQueue(temp->info, temp->parentInfo, temp->length, vertexIndex);
			return;
		}
		else {
			prev = front;
			temp = front->next;

			while (temp != NULL) {
				if (nodeData == temp->info) {
					temp->length = length;
					prev->next = temp->next;
					insertUCSQueue(temp->info, temp->parentInfo, temp->length, vertexIndex);
					return;
				}
				prev = temp;
				temp = temp->next;
			}

		}
	}
}

int pipeQueue::searchQueue(int node) {

	pipeNode *temp = new pipeNode();
	if (front == NULL) {
		//cout << "No element exists";
		return -1;
	}

	temp = front;
	while (temp != NULL) {
		if (node == temp->info)
			return temp->parentInfo;
		else
			temp = temp->next;
	}

	return -1;
}

void pipeQueue::display() {

	pipeNode *temp = new pipeNode();
	if (front == NULL) {
		//cout << "Ntn exists";
		return;
	}

	temp = front;
	while (temp != NULL) {
		//cout << temp->info << " ";
		temp = temp->next;

	}
}