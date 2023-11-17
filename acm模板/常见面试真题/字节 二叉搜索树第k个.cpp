#include <iostream>
#include <stack>

using namespace std;


struct TreeNode {

    int val;
    TreeNode *left = nullptr;
    TreeNode *right = nullptr;

    explicit TreeNode(int val) : val(val) {}
};

int res = -1;
int cur = 1;

void dfs(TreeNode *node, int k) {
    if (k <= 0) {
        res = -1;
        return;
    }

    if (!node) return;
    if (node->left) dfs(node->left, k);

    //当前节点处理
    if (cur == k) {
        res = node->val;
    }
    cur++;
    if (node->right) dfs(node->right, k);
}

void dfs2(TreeNode *node, int k, int &cur2, int &res2) {
    if (k <= 0) {
        res2 = -1;
        return;
    }

    if (!node) return;
    dfs2(node->left, k, cur2, res2);

    //当前节点处理
    if (cur2 == k) {
        res2 = node->val;
    }
    cur2++;
    dfs2(node->right, k, cur2, res2);
}


int dfs3(TreeNode *node, int k) {
    if (!node) return -1;
    if (k <= 0) return -1;
    stack<TreeNode *> stack;
    while (node || !stack.empty()) {
        while (node) {
            stack.push(node);
            node = node->left;
        }
        node = stack.top();
        stack.pop();
        --k;
        if (k == 0) {
            break;
        }
        node = node->right;
    }
    if (k > 0)return -1;

    return node->val;
}


int dfs4(TreeNode *node, int k) {
    if (!node) return -1;
    if (k <= 0) return -1;
    int temp = dfs4(node->left, k);
    if(temp != -1){
        return temp;
    }

    static int count = 1;
    if(count==k){
        return node->val;
    }
    count++;

    temp= dfs4(node->right, k);
    if(temp!=-1){
        return temp;
    }

    return -1;
}

void printTree(TreeNode *node) {
    if (!node) return;
    stack<TreeNode *> st;
    while (true) {
        while (node) {
            st.push(node);
            node = node->left;
        }
        if (st.empty()) break;
        node = st.top();
        st.pop();
        cout << node->val << " ";
        node = node->right;
    }
}


int main() {
    TreeNode *node2 = new TreeNode{2};
    TreeNode *node3 = new TreeNode{3};
    TreeNode *node4 = new TreeNode{4};
    TreeNode *node5 = new TreeNode{5};
    TreeNode *node6 = new TreeNode{6};
    TreeNode *node7 = new TreeNode{7};
    TreeNode *node8 = new TreeNode{8};
    node5->left = node3;
    node5->right = node7;
    node3->left = node2;
    node3->right = node4;
    node7->left = node6;
    node7->right = node8;

    int k = 6;

    dfs(node5, k);
    cout << res << endl;
    int res2 = -1;
    int cur2 = 1;
    dfs2(node5, k, cur2, res2);
    cout << res2 << endl;
    cout << dfs3(node5, k) << endl;
    cout<<dfs4(node5,k)<<endl;
//    printTree(node5);
    return 0;
}