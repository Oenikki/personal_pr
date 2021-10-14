//Leetcode 104 & 110
#include <algorithm> //std::max
#include <cstdlib> //abs

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right)
        : val(x), left(left), right(right) {}
};

namespace hh104 {
class Solution {
public:
    int maxDepth(TreeNode *root) {
        return root ? std::max(maxDepth(root->right), maxDepth(root->left)) + 1 : 0;
    }
};
}

namespace hh110 {
class Solution{
public:
    bool isBalanced(TreeNode *root) {
        if (!root) {
            return true;
        }
        return abs(height(root->left) - height(root->right)) <= 1 && isBalanced(root->left) && isBalanced(root->right);
    }
private:
    int height(TreeNode *node) {
        return node ? std::max(height(node->left), height(node->right)) + 1 : 0;
    }
};

class Solution2 {
public:
    bool isBalanced(TreeNode *root) {
        return helper(root) != -1;
    }
private:
    int helper(TreeNode *node) {
        if (!node) {
            return 0;
        }
        int left = helper(node->left);
        int right = helper(node->right);
        if(left == -1 || right == -1 || abs(left - right) > 1) {
            return -1;
        }
        return 1 + std::max(left, right);
    }
};
}
