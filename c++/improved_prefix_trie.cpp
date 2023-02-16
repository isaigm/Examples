//solution to 677 leetcode problem
#include <iostream>
struct trie_node
{
    bool is_key;
    char c;
    int value;
    trie_node *childs[26];
    trie_node(char cc = '\0')
    {
        c = cc;
        is_key = false;
        for (int i = 0; i < 26; i++)
        {
            childs[i] = nullptr;
        }
    }
};
void insert(trie_node *root, std::string key, int value)
{
    auto cursor = root;
    for (size_t i = 0; i < key.size(); i++)
    {
        int idx = key[i] - 'a';
        if (cursor->childs[idx] == nullptr)
        {
            cursor->childs[idx] = new trie_node(key[i]);
        }
        cursor = cursor->childs[idx];
    }
    cursor->value = value;
    cursor->is_key = true;
}
void dfs(trie_node *node, int &s)
{
    if (node)
    {
        if (node->is_key)
        {
            s += node->value;
        }
        for (int i = 0; i < 26; i++)
        {
            dfs(node->childs[i], s);
        }
    }
}
int prefix_sum(trie_node *root, std::string prefix)
{
    int s = 0;
    auto cursor = root;
    for (size_t i = 0; i < prefix.size(); i++)
    {
        if (cursor->childs[prefix[i] - 'a'] == nullptr)
        {
            return 0;
        }
        cursor = cursor->childs[prefix[i] - 'a'];
    }
    dfs(cursor, s);
    return s;
}
class MapSum {
public:
    /** Initialize your data structure here. */
    MapSum() {
        root = new trie_node();
    }
    
    void insert(string key, int val) {
        ::insert(root, key, val);
    }
    
    int sum(string prefix) {
        return prefix_sum(root, prefix);
    }
    trie_node *root = nullptr;
};
