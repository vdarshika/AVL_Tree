#include <bits/stdc++.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <cmath>
//#define debug(x) printf("%s(%d): %d\n", __func__, __LINE__, x)
using namespace std;

/* run this program using the console pauser or add your own getch, system("pause") or input loop */

class AVL_Node
{ 
	private: 
		int key; 
		int bf;  
		AVL_Node *left_child, *right_child; 
	public:
		AVL_Node()
		{
		}
		AVL_Node(int x)
		{
			key=x;
			bf=0;
			left_child=NULL;
			right_child= NULL;
		}
		~AVL_Node()
		{
			delete right_child;
			delete left_child;
		}
		friend class AVL_Tree;
}; 
class AVL_Tree{ 
	private: 
		AVL_Node *root;
	public: 
		AVL_Tree()
		{
			root=new AVL_Node(INT_MAX);
		//	root->left_child=NULL;
		 } 
		//AVL_Tree(const AVL_Tree &T);             //parameterised constructor
		//AVL_Tree &operator=(const AVL_Tree &T);  //copy constructor
		AVL_Node* link(int add, AVL_Node *balance_point);
		void AVL_Insert(int x); 
		void AVL_Delete(int k); 
		bool AVL_Search(int k); 
		void write_into_file(AVL_Node *root, ofstream &my_file);
		void AVL_Print(); 
		~AVL_Tree()
		{
			delete root;
		}
};

AVL_Node* AVL_Tree::link(int add, AVL_Node *balance_point)
{
	if(add==1)
		return balance_point->left_child;
	else
		return balance_point->right_child;
}
void AVL_Tree::AVL_Insert(int x)
{
	
	AVL_Node *parent_bal_point=root;
	AVL_Node *balance_point=root->right_child;
	AVL_Node *iterator=root->right_child;
	AVL_Node *ptr_q,*ptr_r;
	
	AVL_Node *temp=new AVL_Node(x);
	if(iterator== NULL)                       
	{
		root->right_child=temp;
		return;
	}
	while(1)
	{
		if(x>iterator->key)
		{
			
			ptr_q=iterator->right_child;
			if(ptr_q==NULL)
			{
				iterator->right_child=temp;
				ptr_q=iterator->right_child;
				break;
			}
			else
			{
				if(ptr_q->bf != 0)
				{
					parent_bal_point=iterator;
					balance_point=ptr_q;
					
				}
				iterator=ptr_q;
			}
		}
		else if(x<iterator->key)
		{
			ptr_q=iterator->left_child;
			if(ptr_q==NULL)
			{
				iterator->left_child=temp;
				ptr_q=iterator->left_child;
				
				break;
			}
			else
			{
				if(ptr_q->bf != 0)
				{
					parent_bal_point=iterator;
					balance_point=ptr_q;
					
				}
				iterator=ptr_q;
			}	
		}
		else 
		{
			cout<<"Element to be inserted is Already Present!!!  "<<endl;
			break;
		}	
	}
	//debug(ptr_q->key);
	int add;
	if(x>balance_point->key)
	{
		add=-1;
	}
	else
	{
		add=1;
	}
	
	ptr_r=link(add,balance_point);
	iterator=link(add,balance_point);

	while(iterator!=ptr_q)
	{
		if(x>iterator->key)
		{
			iterator->bf=-1;
			iterator=iterator->right_child;
		}
		else if(x<iterator->key)
		{
			iterator->bf=1;
			iterator=iterator->left_child;
		}
	}

	//debug(balance_point->key);
	//debug(ptr_r->key);
	//debug(iterator->key);
	
	if(balance_point->bf==(-1*add))
	{
		balance_point->bf=0;
		return;
	}
	else if(balance_point->bf==0)
	{
		balance_point->bf=add;
		return;
	}
	
	
	//SINGLE ROTATION---
	else
	{
	if(ptr_r->bf == add)
	{
		if(add==-1)
		{
			iterator=ptr_r;
			balance_point->right_child=ptr_r->left_child;
			ptr_r->left_child=balance_point;
			balance_point->bf=0;
			ptr_r->bf=0;
		} 
		else if(add==1)
		{
			iterator=ptr_r;
			balance_point->left_child=ptr_r->right_child;
			ptr_r->right_child=balance_point;
			balance_point->bf=0;
			ptr_r->bf=0;
		}
	}
	
	//DOUBLE ROTATION-------
	
	else if(ptr_r->bf == -1*add)
	{
		 if(add == -1)
		 {
			iterator=ptr_r->left_child;
			ptr_r->left_child=iterator->right_child;
			iterator->right_child=ptr_r;
			balance_point->right_child=iterator->left_child;
			iterator->left_child= balance_point;
			
			if(iterator->bf==0 || iterator->bf==-1)
			{
				ptr_r->bf=0;
			}
			else
			{
				ptr_r->bf=-1;
			}
			
			if(iterator->bf==0 || iterator->bf==1)
			{
				balance_point->bf=0;
			}
			else
			{
				balance_point->bf=1;
			}
			
			
			iterator->bf=0;
		}
		else if(add==1)
		{
			iterator=ptr_r->right_child;
			ptr_r->right_child=iterator->left_child;
			iterator->left_child=ptr_r;
			balance_point->left_child=iterator->right_child;
			iterator->right_child= balance_point;
			
			if(iterator->bf==0 || iterator->bf==1)
			{
				ptr_r->bf=0;
			}
			else
			{
				ptr_r->bf=1;
			}
			
			if(iterator->bf==0 || iterator->bf==-1)
			{
				balance_point->bf=0;
			}
			else
			{
				balance_point->bf=-1;
			}

			iterator->bf=0;	
		}
	}
	}
	
	if(balance_point==parent_bal_point->right_child)
	{
		parent_bal_point->right_child=iterator;
	}
	else
	{
		parent_bal_point->left_child=iterator;
	}
}
void AVL_Tree::AVL_Delete(int k)
{
	AVL_Node* iterator;
	iterator=root->right_child;

	//Checking whether the tree is empty or not

	try
	{
		if(iterator==NULL)
		{
			throw "There is no element to delete : AVL Tree is Empty.";
		}
	}
	catch(const char* e)
	{
		cout<<e<<endl;
		return;
	}

	stack<AVL_Node*> explored_nodes;
	explored_nodes.push(root);
	
	int flag=0;
	while(iterator != NULL)
	{
		if(iterator->key < k)
		{
			explored_nodes.push(iterator);
			iterator=iterator->right_child;
		}
		else if(iterator->key > k)
		{
			explored_nodes.push(iterator);
			iterator=iterator->left_child;
		}
		else if(iterator->key == k)
		{
			flag=1;
			break;
		}
	}

	try
	{
		if(flag ==0)
		{
			throw "The element to be deleted doesn't exist in the AVL tree.";
		}
		
		// CASE-1: Node to be deleted is the leaf node (i.e node has no children) or Node with single child (either left child or right child)
		if(iterator->left_child==NULL || iterator->right_child==NULL)
		{
			
			AVL_Node *temp_node, *parent_node;
			parent_node=explored_nodes.top();

			if(iterator->right_child != NULL)
			{
				temp_node=iterator->right_child;
			}
			else
			{
				temp_node=iterator->left_child;
			}

			//	NODE with no child
			if(temp_node==NULL)
			{
				temp_node=iterator;
				if(iterator == parent_node->right_child)
				{
					parent_node->right_child = NULL;
				}
				else if(iterator == parent_node->left_child)
				{
					parent_node->left_child = NULL;
				}
				iterator=NULL;
				
			}
			// Node with one child(either left child or left child)
			else
			{
				iterator->key=temp_node->key;
				iterator->bf=temp_node->bf;
				iterator->right_child=temp_node->right_child;
				iterator->left_child=temp_node->left_child;
			}
			delete temp_node;
		}
		// CASE-2 Node to be deleted has both the children
		else
		{
			explored_nodes.push(iterator);
			AVL_Node *successor;
			successor = iterator->right_child;

			// Find the inorder successor of the node to be deleted.
			while (successor->left_child != NULL)
			{
				explored_nodes.push(successor);
				successor=successor->left_child;
			}
			
			iterator->key=successor->key;
		
			k=successor->key;

			AVL_Node *temp_node,*parent_node;
			if(successor->left_child !=NULL)
			{
				temp_node=successor->left_child;
			}
			else
			{
				temp_node=successor->right_child;
			}
			parent_node=explored_nodes.top();
			
			if(temp_node==NULL)
			{
				temp_node=successor;
				
				if(successor==parent_node->left_child)
				{
					parent_node->left_child==NULL;
					
				}
				else if(successor==parent_node->right_child)
				{
					parent_node->right_child=NULL;
				}
				successor=NULL;
				temp_node=NULL;
			}
			else
			{
				successor->key=temp_node->key;
				successor->bf=temp_node->bf;
				
				successor->left_child=temp_node->left_child;
				successor->right_child=temp_node->right_child;
			}
			
			delete temp_node;
		}

	}
	catch(const char* e)
	{
		cout<<e<<endl;
		return;
	}
	
	AVL_Node *parent_bal_point;
	AVL_Node *balance_point;
	AVL_Node *ptr_r;

	while(explored_nodes.top() != root)
	{
		balance_point=explored_nodes.top();
		int add;
		if(k > balance_point->key)
		{
			add=-1;
		}
		else
		{
			add=1;
		}
		explored_nodes.pop();
		parent_bal_point=explored_nodes.top();

		if(balance_point->bf == add)
		{
			balance_point->bf =0;
			continue;
		}
		else if(balance_point->bf == 0)
		{
			balance_point->bf = -1*add;
			return;
		}
		else
		{
			ptr_r=link(balance_point->bf,balance_point);

			if(ptr_r->bf==0)
			{
				if(add==1)
				{
					iterator=ptr_r;
					balance_point->right_child=ptr_r->left_child;
					ptr_r->left_child=balance_point;
					ptr_r->bf=add;
				}
				else if(add==-1)
				{
					iterator=ptr_r;
					balance_point->left_child=ptr_r->right_child;
					ptr_r->right_child=balance_point;
					ptr_r->bf=add;
				}
			}
			else if(ptr_r->bf == (-1*add))
			{
				if(add ==-1)
				{
					iterator=ptr_r;
					balance_point->left_child=ptr_r->right_child;
					ptr_r->right_child=balance_point;
					balance_point->bf=0;
					ptr_r->bf=0;
				}
				else if(add ==1)
				{
					iterator=ptr_r;
					balance_point->right_child=ptr_r->left_child;
					ptr_r->left_child=balance_point;
					balance_point->bf=0;
					ptr_r->bf=0;
				}
			}
			else if(ptr_r->bf == add)
			{
				if(add == 1)
				{
					iterator=ptr_r->left_child;
					ptr_r->left_child=iterator->right_child;
					iterator->right_child=ptr_r;
					balance_point->right_child=iterator->left_child;
					iterator->left_child=balance_point;

					if(iterator->bf==0 || iterator->bf==-1)
					{
						balance_point->bf=0;
					}
					else
					{
						balance_point->bf=1;
					}


					if(iterator->bf==0 || iterator->bf==1)
					{
						ptr_r->bf=0;
					}
					else
					{
						ptr_r->bf=-1;
					}

					iterator->bf=0;
				}
				else if( add == -1)
				{
					iterator=ptr_r->right_child;
					ptr_r->right_child=iterator->left_child;
					iterator->left_child=ptr_r;
					balance_point->left_child=iterator->right_child;
					iterator->right_child=balance_point;

					if(iterator->bf==0 || iterator->bf==-1)
					{
						balance_point->bf=0;
					}
					else
					{
						balance_point->bf=-1;
					}


					if(iterator->bf==0 || iterator->bf==1)
					{
						ptr_r->bf=0;
					}
					else
					{
						ptr_r->bf=1;
					}

					iterator->bf=0;
					
				}
			}
			if(balance_point==parent_bal_point->right_child)
			{
				parent_bal_point->right_child=iterator;
			}
			else
			{
				parent_bal_point->left_child=iterator;
			}
		}
	}	
}
bool AVL_Tree::AVL_Search(int k)
{
	AVL_Node *ptr=root->right_child;
	while(ptr != NULL)
	{
		if(ptr->key == k)
			return true;
		else if(ptr->key < k)
			ptr=ptr->right_child;
		else
			ptr=ptr->left_child;
	}
	return false;
}
void AVL_Tree::write_into_file(AVL_Node *root, ofstream &my_file)
{
 	if(root == NULL) return;
 	my_file << "node" << root->key << " [label = \"<f0> | <f1> " << root->key << " | <f2> " << root->bf << " | <f3>\"];\n";
    if(root->left_child) {
    	write_into_file(root->left_child, my_file);
    	my_file << "node" << root->left_child->key << " [label = \"<f0> | <f1> " << root->left_child->key << " | <f2> " << root->left_child->bf << " | <f3>\"];\n";
    	my_file << "\"node" << root->key << "\":f0 -> \"node" << root->left_child->key << "\":f1;\n";
    }
    if(root->right_child) {
    	write_into_file(root->right_child, my_file);
    	my_file << "node" << root->right_child->key << " [label = \"<f0> | <f1> " << root->right_child->key << " | <f2> " << root->right_child->bf << " | <f3>\"];\n";
    	my_file << "\"node" << root->key << "\":f3 -> \"node" << root->right_child->key << "\":f1;\n";
    }   	
}



void AVL_Tree::AVL_Print()
{
    ofstream my_file;
    my_file.open ("graph.gv");
    my_file << "digraph {\n";
    my_file << "node [shape = record,style=filled,height = .2];";
	my_file <<"edge [color=red];";
    AVL_Node *ptr = root->right_child;
    write_into_file(ptr, my_file);

    my_file << "}\n";
    my_file.close();

    string cmd = "dot -Tpng graph.gv -o file.png";
    system((char*)cmd.c_str());

	cout<<"The AVL Tree is printed.  Please have a watch!! "<<endl;
}

int main(int argc, char** argv) {

	AVL_Tree *obj=new AVL_Tree();       //Create object for the AVL_Tree Class
	cout<<"-------LIST OF OPERATION THAT CAN BE PERFORMED ON AVL TREE---------"<<endl;
	cout<<"Operation 1: \n \t Insertion in AVL Tree."<<endl;
	cout<<"Operation 2: \n \t Deletion in AVL Tree."<<endl;
	cout<<"Operation 3: \n \t Search in AVL Tree."<<endl;
	cout<<"Operation 4: \n \t Print the AVL Tree."<<endl<<endl;
	
	int choice,element;
	while(1)
	{
		cout<<"Enter the operation you want to perform : ";
		cin>>choice;

		switch(choice)
		{
			case 1:
					cout<<"Enter the element to be inserted : ";
					cin>>element;
					cout<<endl;
					obj->AVL_Insert(element);
					break;
			case 2:
					cout<<"Enter the element to be deleted: ";
					cin>>element;
					cout<<endl;
					obj->AVL_Delete(element);
					break;
			case 3:
					cout<<"Enter the element to be searched : ";
					cin>>element;
					cout<<endl;
					if(obj->AVL_Search(element) == true)
					{
						cout<<"The element "<<element<<" is FOUND!! "<<endl;
					}
					else
					{
						cout<<"The element "<<element<<" is NOT FOUND!! "<<endl;
					}
					break;
			case 4:
					obj->AVL_Print();
					cout<<endl;
					break;
			case 5:
					exit(0);
					break;
			default:
					cout<<"Wrong operation !!\n Please enter the valid operation.\n";
					break;
		}
	}
	return 0;
}
