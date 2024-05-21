#include<iostream>
#include<vector>
#include <fstream>
#include <string>
using namespace std;
class Node
{
public:
	char data;
	int freq;
	Node* left;
	Node* right;
	Node(char data, int freq) : data(data),
		freq(freq),
		left(NULL),
		right(NULL)
	{}
	~Node()
	{
		delete left;
		delete right;
	}
};

template<class T>
class minHeap
{
private:

	vector<Node*> _vector;
	void heapify(int n) {
		int largest = n;
		int left = 2 * n + 1;
		int right = 2 * n + 2;

		if (left < _vector.size() && _vector[largest]->freq > _vector[left]->freq)
			largest = left;
		if (right < _vector.size() && _vector[largest]->freq > _vector[right]->freq)
			largest = right;

		if (largest != n)
		{
			swap(largest, n);
			heapify(largest);
		}
	}
	void BubbleUp(int n)
	{
		heapify(n);
	}

	void BubbleDown(int n)
	{
		heapify(n);
	}

	void swap(int x, int y)
	{
		T store = _vector.at(x);
		_vector[x] = _vector.at(y);
		_vector[y] = store;
	}

public:
	minHeap()
	{
		;
	}

	minHeap(vector<char> &d,vector<int>f)
	{
		for (int i = 0; i < d.size(); i++)
		{
			Node* nn = new Node(d[i], f[i]);
			_vector.push_back(nn);
		}

		BuildHeap();
	}

	void BuildHeap()
	{
		for (int i = _vector.size() / 2 - 1; i >= 0; i--)
			BubbleUp(i);
	}
	void insertNode(Node* nn)
	{
		_vector.push_back(nn);

		BuildHeap();
	}
	Node* getMin()const
	{
		if (_vector.size() == 0)
			return nullptr;

		return _vector[0];
	}

	void deleteMin() //delete root Node
	{
		if (_vector.size() == 0)
			return;

		int last = _vector.size() - 1;
		swap(0, last);
		_vector.pop_back();
		BubbleDown(0);

	}
	int getSize()
	{
		return _vector.size();
	}
};

template<class T>
void swap(T& a, T& b)
{
	T store = a;
	b = a;
	a = store;
}
class Huffman_Codes
{
public:
    Node* top;
    void print_Code(Node* root, string str)
    {
        if (root == NULL)
            return;

        if (root->data == '$')
        {
            print_Code(root->left, str + "0");
            print_Code(root->right, str + "1");
        }

        if (root->data != '$')
        {
            cout << root->data << " : " << str << "\n";
            print_Code(root->left, str + "0");
            print_Code(root->right, str + "1");
        }
    }
    Huffman_Codes()
    {
        top = NULL;
    }
    ~Huffman_Codes()
    {
        delete top;
    }
    void Generate_Huffman_tree(vector<char>& data, vector<int>& freq, int size)
    {
        Node* left;
        Node* right;
        minHeap<Node*> min(data,freq);
        while (min.getSize() != 1)
        {
            left = min.getMin();
			min.deleteMin();
			right = min.getMin();
			min.deleteMin();
            top = new Node('$', left->freq + right->freq);
            top->left = left;
            top->right = right;
			min.insertNode(top);
        }
     //   print_Code(min.getMin(), "");
    }
	string search(Node* root, char t,string s ,string c)
	{
		if (root == NULL)
		{
			return "";
		}
		if (root->data == '$')
		{
			c = search(root->left,t, s + "0",c);
			if (c != "")
				return c;
			c = search(root->right,t, s + "1",c);
			if (c != "")
				return c;
		}

		if (root->data != '$')
		{
			if (root->data == t)
				return s;
			return "";
		}
		return "";

	}
	string searchIntree(char t)
	{
		return search(top, t, "","");
	}
	void encoded()
	{
		ifstream f("k.txt");
		string code;
		char temp = '\0';
		while (!(f.eof()))
		{
			f.get(temp);
			if (temp == '\n')
			{
				
				ofstream coded( "Encoded.txt",ios::app);
				coded << "\n";
				coded.close();
			}
			else
			{
				code = searchIntree(temp);
				//cout << "Code of " << temp << " = " << code << endl;
				ofstream coded("Encoded.txt",ios::app);
				coded << code;
				coded.close();
			}
			
		}
	}
	void decoded()
	{
		ifstream f("Encoded.txt");
		char temp = '\0';
		Node* root = top;
		while (!(f.eof()))
		{
			f.get(temp);
			if (temp == '\n')
			{
				ofstream decoded("decoded.txt", ios::app);
				decoded << "\n";
				decoded.close();
			}
			else if(temp == '0' || temp == '1')
			{
				if (temp == '0')
				{
					root = root->left;
					if (root->data != '$')
					{
						ofstream decoded("decoded.txt", ios::app);
						decoded << root->data;
						root = top;
					}
				}
				else
				{
					root = root->right;
					if (root->data != '$')
					{
						ofstream decoded("decoded.txt", ios::app);
						decoded << root->data;
						root = top;
					}
				}
			}

		}
	}
};

int main()
{
	// opening f and count frequency of each character in f
	string text;
	ifstream f("k.txt");
    vector<char> data;
    vector<int> freq;
	while (getline(f, text))
	{
		int len = text.length();
		for (int i = 0; i < len; i++)
		{
			
			{
				bool b = false;
				for (int a = 0; a < data.size(); a++)
				{
					if (data[a] == text[i])
					{
						freq[a]++;
	
						b = true;
					}
				}
				if (b == false)
				{
                    data.insert(data.end(), text[i]);
                    freq.insert(freq.end(), 1);
				}
			}
		}
	}
	f.close();
	for (int a = 0; a < data.size(); a++)
	{
		cout << data[a] << " ";
		cout << freq[a];
		cout << endl;
	}
    Huffman_Codes obj;
    int size = data.size();
    obj.Generate_Huffman_tree(data, freq, size);
	obj.print_Code(obj.top, "");
	obj.encoded();
	obj.decoded();
	return 0;
}