
typedef struct Node Node;
struct Node
{
	void* m_data;
	Node* m_next;
	Node* m_prev;
};

struct List
{
	Node m_head;
	Node m_tail;
};
void insert( Node *_node1, Node *_node2);
void pop( Node *_node);

