#include <bits/stdc++.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#define debug(x) printf("%s(%d): %d\n", __func__, __LINE__, x)
using namespace std;

/* run this program using the console pauser or add your own getch, system("pause") or input loop */

class AVL_Node					//Class of AVL Node that defines the parameters of a node in the AVL Tree.
{ 
	private: 
		int key; 						//key -> the value that is stored in the node.
		int bf;							//bf -> the balance factor of a node 
						   				// bf= height of left subtree - height of right subtree.  
		AVL_Node *left_child;			//pointer to the left child of a node.
		AVL_Node *right_child;         //pointer to the right child of the node.
	public:
		AVL_Node()                     //default constructor of AVL_node class.
		{
		}
		AVL_Node(int x)					// parameterised constructor of AVL_node class which will initialise the default value of parameters of the node.
		{
			key=x;						//initialise the key of the node with the value of x.
			bf=0;						// initialise balance factor of the node as 0.
			left_child=NULL;			// initialise the left pointer of the node as NULL.
			right_child= NULL;			// initialise the right pointer of the node as NULL.
		}
		~AVL_Node()						//destruction which will delete the pointers after use.
		{
			delete right_child;
			delete left_child;
		}
		friend class AVL_Tree;			//To access the private members of the AVL node class.
}; 


class AVL_Tree						//Class of AVL Node that defines the operations to be performed in the AVL Tree.
{ 
	private: 
		AVL_Node *root;				//root is a dummy node that is created at the beginning of all the operations in the AVL tree.
	public: 
		AVL_Tree()                       	//AVL tree default constructor to initialise the parameters of the tree
		{
			root=new AVL_Node(INT_MAX);			//Initialisation of dummy node with the maximum value of integer.
			root->left_child=NULL;				//Initialise left child of the dummy node as NULL.
		 } 
		AVL_Tree(const AVL_Tree &T);             		 // copy constructor
		AVL_Tree &operator=(const AVL_Tree &T);  		// operator overloading

		AVL_Node* link(int add, AVL_Node *balance_point);        	//Function to initialise the value of add which represents from where the node is being deleted or inserted.
		void AVL_Insert(int x);										//Function to insert an element in the avl tree 
		void AVL_Delete(int k); 									//Function to delete an element in the avl tree
		bool AVL_Search(int k); 									//Function to search an element in the avl tree
		void write_into_file(AVL_Node *root, ofstream &my_file);	//Function to write the avl tree in teh file
		void AVL_Print(const char*); 											//Function to print the AVL tree.

		~AVL_Tree()									//destructor to delete the dummy node created.
		{
			delete root;
		}
};

AVL_Node* AVL_Tree::link(int add, AVL_Node *balance_point)
{
	if(add==1)
		return balance_point->left_child;						//returns the pointer of the left sub tree 
	else
		return balance_point->right_child;						//returns the pointer to the right sub tree
}
void AVL_Tree::AVL_Insert(int x)
{
	// parent_bal_point denotes the pointer to the parent of the balance node where balancing is to be done.
	// balance point denotes the node where the balancing is to be done.
	// iterator is a pointer that traverse to the point after which insertion is to be performed.
	// ptr_q pointer to the place of insertion.

	AVL_Node *parent_bal_point=root;								
	AVL_Node *balance_point=root->right_child;
	AVL_Node *iterator=root->right_child;
	AVL_Node *ptr_q,*ptr_r;
	
	AVL_Node *temp=new AVL_Node(x);				//temp -> temporary node which is to be inserted in the avl tree.

	//CASE 1: When tree is empty and insertion is to be performed.
	// we just point the right pointer of dummy node to the temporary node.

	if(iterator== NULL)                       
	{
		root->right_child=temp;		
		return;
	}

	//Code to search the point where the node is to be inserted.
	while(1)
	{
		//when element to be inserted is greater than the iterator move ptr_q to the right sub tree.
		if(x>iterator->key)								
		{
			
			ptr_q=iterator->right_child;
			
			//Check whether the place is empty or not.
			if(ptr_q==NULL)							//Place is empty
			{
				iterator->right_child=temp;			// point the right child of iterator to the temporary node.
				ptr_q=iterator->right_child;		// also point ptr_q to the same position.
				break;
			}
			//Case where the iterator needs to traverse more in downward direction to find the empty space for insertion
			else
			{
				if(ptr_q->bf != 0)							
				{
					parent_bal_point=iterator;				// Move the pointer so as to find the appropriate place for balancing.
					balance_point=ptr_q;
					
				}
				iterator=ptr_q;
			}
		}
		
		//when element to be inserted is smaller than the iterator move ptr_q to the left sub tree.
		else if(x<iterator->key)
		{
			ptr_q=iterator->left_child;

			//Check whether the place is empty or not.
			if(ptr_q==NULL)									//Place is empty					
			{
				iterator->left_child=temp;					// point the left child of iterator to the temporary node.
				ptr_q=iterator->left_child;					// also point ptr_q to the same position.
				
				break;
			}
			//Case where the iterator needs to traverse more in downward direction to find the empty space for insertion
			else
			{
				if(ptr_q->bf != 0)
				{
					parent_bal_point=iterator;				// Move the pointer so as to find the appropriate place for balancing.
					balance_point=ptr_q;
					
				}
				iterator=ptr_q;
			}	
		}

		// Case where duplicate element is inserted in the tree.
		else 
		{
			cout<<"Element to be inserted is Already Present!!!  "<<endl;
			break;
		}	
	}
	
	int add; 				// add-> to which sub tree the node is inserted.          

	if(x>balance_point->key)
	{
		add=-1;				//Element is inserted on the right sub tree
	}
	else
	{
		add=1;				//Element is inserted on the right sub tree
	}
	
	// Balancing the balance factors of all the nodes between iterator and pointer r.
	ptr_r=link(add,balance_point);
	iterator=link(add,balance_point);

	while(iterator!=ptr_q)
	{
		if(x>iterator->key)
		{
			iterator->bf=-1;						// If the insertion is in the right suv tree then make the balance factor of the node as -1;
			iterator=iterator->right_child;			// move iterator to its right child
		}
		else if(x<iterator->key)
		{
			iterator->bf=1;							// If the insertion is in the left sub tree then make the balance factor of the node as 1;						
			iterator=iterator->left_child;			// move iterator to its left child
		}
	}
	// Checking for Rotation: 
	
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
	// Case where SINGLE ROTATION is required.
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
		
		// Case where DOUBLE ROTATION is required.
		
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
void AVL_Tree::AVL_Delete(int k)
{
	AVL_Node* iterator;
	iterator=root->right_child;

	//Checking whether the tree is empty or not. If tree is empty throw an exception.
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

	//When tree is not empty.

	stack<AVL_Node*> explored_nodes;			//	Stack containing avl nodes that depicts the path that is followed.
	explored_nodes.push(root);					//	Initially push root to the stack.	
	
	int flag=0;						// variable to check whether the element to be deleted is in the avl tree or not.

	//Searching for the node to be deleted.
	while(iterator != NULL)
	{
		if(iterator->key < k)							//if node is less than the key element than push the 
		{												//node in the stack and continue to search in the right sub tree.
			explored_nodes.push(iterator);
			iterator=iterator->right_child;
		}
		else if(iterator->key > k)						//if node is less than the key element than push 
		{												//the node in the stack and continue to search in the left sub tree.
			explored_nodes.push(iterator);
			iterator=iterator->left_child;
		}
		else if(iterator->key == k)						//when the node is found break the loop and put value of flag=1;
		{
			flag=1;
			break;
		}
	}

	try
	{
		//Case if the elememt to be deleted is not present in the tree then throw an exception.
		if(flag ==0)
		{
			throw "The element to be deleted doesn't exist in the AVL tree.";
		}
		
		// CASE-1: Node to be deleted is the leaf node (i.e node has no children) or Node with single child (either left child or right child)
		if(iterator->left_child==NULL || iterator->right_child==NULL)
		{
			AVL_Node *temp_node, *parent_node;
			parent_node=explored_nodes.top();

			if(iterator->left_child == NULL)
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
			explored_nodes.push(iterator);					// Push the node into the stack so that later while popping the balancing can be performed.
			AVL_Node *successor;							// Successor node pointer to the successor of the node to be deleted.
			successor = iterator->right_child;				

			// Find the inorder successor of the node to be deleted.
			while (successor->left_child != NULL)
			{
				explored_nodes.push(successor);				
				successor=successor->left_child;
			}
			
			iterator->key=successor->key;					// Replace the node to be deleted with its successor node.
		
			k=successor->key;								// Now change the node to be deleted as the succesor node.

			AVL_Node *temp_node;				// points to the right or left child of the successor node
			AVL_Node *parent_node;				// points to the parent of successor node.

			// Temp_node points to the left child of successor as it is not null.
			if(successor->left_child ==NULL)
			{
				temp_node=successor->right_child;				
			}
			// Temp_node points to the right child of successor as it is not null.
			else
			{
				temp_node=successor->left_child;
			}

			parent_node=explored_nodes.top();
			
			// Deletion of the successor node.
			// When the left or right child of the successor node is NULL.
			if(temp_node==NULL)
			{
				temp_node=successor;				// Initialise temp node to the successor
				
				// check whether the successor is the right child or left child of the parent node.
				// When found put the pinter to that child as NULL.
				if(successor==parent_node->left_child)
				{
					
					parent_node->left_child = NULL;
					
				}
				else if(successor==parent_node->right_child)
				{
					parent_node->right_child = NULL;
				}
				successor=NULL;
			}
			// Deletion of the successor node.
			// When the left or right child of the successor node is  not NULL.
			// replace every parameter of successor node with the parameters of the temp node.
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
	// Balance the balance factor of all the nodes that are stored in the stack.
	AVL_Node *parent_bal_point;
	AVL_Node *balance_point;
	AVL_Node *ptr_r;

	while(explored_nodes.top() != root)
	{
		balance_point=explored_nodes.top();
	
		int add;
		if(k >= balance_point->key)
		{
			add=-1;			// node to be deleted is from the right subtree.
		}
		else
		{
			add=1;			// node to be deleted is from the right subtree.
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
			

			// Case of Single Rotation
			if(ptr_r->bf==0 || ptr_r==NULL)
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
			// Case of Single Rotation
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
			// Case of Double Rotation
			else if(ptr_r->bf == add)
			{
				if(add == 1)
				{
					iterator=ptr_r->left_child;
					ptr_r->left_child=iterator->right_child;
					iterator->right_child=ptr_r;
					balance_point->right_child=iterator->left_child;
					iterator->left_child=balance_point;

					if(iterator->bf==0 || iterator->bf==1)
					{
						balance_point->bf=0;
					}
					else
					{
						balance_point->bf=1;
					}


					if(iterator->bf==0 || iterator->bf==-1)
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
			//Case where after rotation the balance factor does not change and remain the same then
			//there is no need for further checking of the above nodes and we can return
			if(iterator->bf == 1 || iterator->bf == -1)
			{
			
				if(balance_point == parent_bal_point->right_child) 
						parent_bal_point->right_child = iterator;
				else 
						parent_bal_point->left_child = iterator;
				return;
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
	AVL_Node *ptr=root->right_child;     	// pointer to traverse through the avl tree. 
	while(ptr != NULL)
	{
		if(ptr->key == k)			// when you get the element to be serached return true.
			return true;
		else if(ptr->key < k)			
			ptr=ptr->right_child;		//search in right subtree if ptr value is less than x.
		else
			ptr=ptr->left_child;		//search in left subtree if ptr value is greater than x.
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
 


void AVL_Tree::AVL_Print(const char* file_name)
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

    string cmd = "dot -Tpng graph.gv -o "+ string(file_name)+ ".png";
    system((char*)cmd.c_str());

	cout<<"The AVL Tree is printed.  Please have a watch!! "<<endl;
}

int main(int argc, char** argv) {

	AVL_Tree *obj=new AVL_Tree();       //Create object for the AVL_Tree Class
	cout<<endl<<endl;
	cout<<"-------LIST OF OPERATION IN AVL TREE---------"<<endl;
	cout<<"Operation 1: \n \t Insertion in AVL Tree."<<endl;
	cout<<"Operation 2: \n \t Deletion in AVL Tree."<<endl;
	cout<<"Operation 3: \n \t Search in AVL Tree."<<endl;
	cout<<"Operation 4: \n \t Print the AVL Tree."<<endl<<endl;
	
	int choice,element;
	string file;
	
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
					cout<<"Enter the file name: ";
					cin>>file;
					cout<<endl;
					obj->AVL_Print(file.c_str());
					cout<<endl;
					break;
			case 5:
					exit(0);
					break;
			default:
					cout<<"Wrong operation !!\n Please enter the valid operation.\n";
					break;
		}
		if(choice>5)
		{
			break;
		}
		}
		delete obj;
	
	return 0;
}
