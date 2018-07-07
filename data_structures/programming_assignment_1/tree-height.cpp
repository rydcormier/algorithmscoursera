#include <algorithm>
#include <iostream>
#include <vector>
#include <queue>
#if defined(__unix__) || defined(__APPLE__)
#include <sys/resource.h>
#endif

class Node;

class Node {
public:
    int key;
    Node *parent;
    std::vector<Node *> children;
    
    Node() {
        this->parent = NULL;
    }
    
    void setParent(Node *theParent) {
        parent = theParent;
        parent->children.push_back(this);
    }
};

int treeHeight(Node *root) {
    if (root == NULL) return 0;
    int height = 0;
    // use a queue for level traversal
    std::queue<Node *> q;
    q.push(root);
    
    while (true) {
        // count the nodes on this level
        int count = q.size();
        // no nodes means we're done;
        if (count == 0) return height;
        
        ++height;
        
        // Dequeue nodes and enque children
        while (count > 0) {
            Node *node = q.front();
            q.pop();
            for (int i = 0; i < node->children.size(); ++i) {
                q.push(node->children[i]);
            }
            --count;
        }
    }
}


int main_with_large_stack_space() {
    std::ios_base::sync_with_stdio(0);
    int n;
    std::cin >> n;
    
    std::vector<Node> nodes;
    nodes.resize(n);
    
    // keep the index for the root to use with treeHeight
    int root_index;
    for (int child_index = 0; child_index < n; child_index++) {
        int parent_index;
        std::cin >> parent_index;
        if (parent_index >= 0) {
            nodes[child_index].setParent(&nodes[parent_index]);
        } else {    // parent index of -1 means root
            root_index = child_index;
        }
        nodes[child_index].key = child_index;
    }
    
    // Replace this code with a faster implementation
//    int maxHeight = 0;
//    for (int leaf_index = 0; leaf_index < n; leaf_index++) {
//        int height = 0;
//        for (Node *v = &nodes[leaf_index]; v != NULL; v = v->parent)
//            height++;
//        maxHeight = std::max(maxHeight, height);
//    }
    
    int maxHeight = treeHeight(&nodes[root_index]);
    
    std::cout << maxHeight << std::endl;
    return 0;
}


int main (int argc, char **argv)
{
#if defined(__unix__) || defined(__APPLE__)
    // Allow larger stack space
    const rlim_t kStackSize = 16 * 1024 * 1024;   // min stack size = 16 MB
    struct rlimit rl;
    int result;
    
    result = getrlimit(RLIMIT_STACK, &rl);
    if (result == 0)
    {
        if (rl.rlim_cur < kStackSize)
        {
            rl.rlim_cur = kStackSize;
            result = setrlimit(RLIMIT_STACK, &rl);
            if (result != 0)
            {
                std::cerr << "setrlimit returned result = " << result << std::endl;
            }
        }
    }
    
#endif
    return main_with_large_stack_space();
}
