//solution to 208 leetcode problem
struct node
{
    char ch;
    bool is_end = false;
    std::vector<node*> childs;
    node(char ch = '\0')
    {
        this->ch = ch;
    }
};
void insert(node* root, std::string &word, size_t idx)
{
    if (idx < word.size())
    {
        node* parent = nullptr;
        
        for (auto& child : root->childs)
        {
            if (child->ch == word[idx])
            {
                parent = child;
                break;
            }
        }
        if (parent == nullptr)
        {
            auto n = new node(word[idx]);
            root->childs.push_back(n);
            insert(n, word, idx + 1);
        }
        else
        {
            insert(parent, word, idx + 1);
        }
    }else root->is_end = true;
}
std::pair<bool, node*> prefix(node* root, std::string &word, size_t idx)
{
    std::pair<bool, node*> ans = {true, root};
    if (idx < word.size())
    {
        node* parent = nullptr;
        for (auto& child : root->childs)
        {
            if (child->ch == word[idx])
            {
                parent = child;
                break;
            }
        }
        if (parent)
        {
            ans = prefix(parent, word, idx + 1);
        }
        else return {false, nullptr};
    }
    return ans;
}
class Trie {
public:
    /** Initialize your data structure here. */
    Trie() {
        root = new node();
    }
    
    /** Inserts a word into the trie. */
    void insert(string word) {
        ::insert(root, word, 0);
    }
    
    /** Returns if the word is in the trie. */
    bool search(string word) {
        auto [f, node] = ::prefix(root, word, 0);
        if(f)
        {
            return node->is_end;
        }
        return false;
    }
    
    /** Returns if there is any word in the trie that starts with the given prefix. */
    bool startsWith(string prefix) {
        auto [f, node] = ::prefix(root, prefix, 0);
        return f;
    }
    node *root = nullptr;
};
