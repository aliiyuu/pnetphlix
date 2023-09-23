#ifndef TREEMULTIMAP_INCLUDED
#define TREEMULTIMAP_INCLUDED

template <typename KeyType, typename ValueType>
class TreeMultimap
{
  public:
    class Iterator
    {
      public:
        Iterator()
        {}
        
        Iterator(std::vector<ValueType>& contents) : it(contents.begin()), m_begin(contents.begin()), m_end(contents.end())
        {
            m_hasContents = true; 
        }

        ValueType& get_value() const
        {
            return *it;
        }

        bool is_valid() const
        {
            return (m_hasContents && it != m_end); 
        }

        void advance()
        {
            if (is_valid()) it++; 
        }

      private:
        bool m_hasContents = false;
        typename std::vector<ValueType>::iterator it;
        typename std::vector<ValueType>::iterator m_begin;
        typename std::vector<ValueType>::iterator m_end;
    };

    TreeMultimap()
    {
        m_root = nullptr;
    }

    ~TreeMultimap()
    {
        deleteNodes(m_root);
    }

    void insert(const KeyType& key, const ValueType& value)
    {
        if (m_root == nullptr)
        {
            m_root = new Node(key, value);
        }
        Node* p = m_root;
        for(;;)
        {
            if (key == p->key) // if the key already exists
            {
                p->values.push_back(value); 
                return;
            }
            if (key < p->key)
            {
                if (p->left != nullptr)
                    p = p->left;
                else
                {
                    p->left = new Node(key, value);
                    return;
                }
            }
            else if (key > p->key)
            {
                if (p->right != nullptr)
                    p = p->right;
                else
                {
                    p->right = new Node(key, value);
                    return;
                }
            }
        }
    }

    Iterator find(const KeyType& key) const
    {
        Node* p = m_root;
        
        while (p != nullptr)
        {
        
            if (key == p->key)
                break;
            else if (key < p->key)
                p = p->left;
            else
                p = p->right;
        }
        
        if (p == nullptr)
            return Iterator(); // didn't find the value
        
        Iterator it = Iterator(p->values);
        
        return it;
    }

  private:
    struct Node
    {
        Node() : left(nullptr), right(nullptr) {}
        Node(KeyType k, ValueType v) : left(nullptr), right(nullptr), key(k)
        {
            values.push_back(v);
        }
        KeyType key;
        std::vector<ValueType> values;
        Node *left, *right;
    };
    Node* m_root;
    void deleteNodes(Node* p)
    {
        if (p != nullptr)
        {
            deleteNodes(p->left);
            deleteNodes(p->right);
            delete p;
        }
    }
};

#endif // TREEMULTIMAP_INCLUDED
