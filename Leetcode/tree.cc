#include <vector>
#include <algorithm> //std::max
#include <unordered_set>

using namespace std;

struct TreeNode {
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int val) : val(val), left(nullptr), right(nullptr) {}
    TreeNode(int val, TreeNode *left, TreeNode *right)
        : val(val), left(left), right(right) {}
    int val;
    TreeNode *left;
    TreeNode *right;

};

namespace hh104 {
class Solution {
public:
    int MaxDepth(TreeNode *root) {
        return root ? 1 + std::max(MaxDepth(root->left), MaxDepth(root->right)) : 0;
    }
};
}


namespace hh110 {
class Solution {
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
        if (left == -1 || right == -1 ||
            abs(left - right) > 1) {
            return -1;
        }
        return 1 + std::max(left, right);
    }
};
}

namespace hh543 {
class Solution {
public:
    int diameterOfBinaryTree(TreeNode *root) {
        int diameter = 0;
        helper(root, diameter);
        return diameter;
    }
private:
    int helper(TreeNode *node, int& diameter) {
        if (node) {
            return 0;
        }
        int left = helper(node->left, diameter);
        int right = helper(node->right, diameter);
        diameter = std::max(left + right, diameter);
        return std::max(left, right) + 1;
    }
};
}

namespace hh437 {
class Solution {
public:
    int pathSum(TreeNode *root, int targetSum) {
        return root ? pathWithRoot(root, targetSum) +
            pathSum(root->left, targetSum) + pathSum(root->right, targetSum) : 0;
    }
private:
    int pathWithRoot(TreeNode *node, int sum) {
        if (!node) {
            return 0;
        }
        int ret = node->val == sum ? 1 : 0;
        ret += pathWithRoot(node->left, sum - node->val);
        ret += pathWithRoot(node->right, sum - node->val);
        return ret;
    }
};
}

namespace hh101 {
class Solution {
public:
    bool isSymmetric(TreeNode *root) {
        return root ? isSymmetric(root->left, root->right) : true;
    }
private:
    bool isSymmetric(TreeNode *left, TreeNode *right) {
        if (!left && !right) {
            return true;
        }
        if (!left || !right) {
            return false;
        }
        if (left->val != right->val) {
            return false;
        }
        return isSymmetric(left->left, right->right) &&
            isSymmetric(left->right, right->left);
    }
};
}

namespace hh1110 {
class Solution {
public:
    vector<TreeNode*> delNodes(TreeNode *root, vector<int>& to_delete) {
        vector<TreeNode*> forest;
        unordered_set<int> dict(to_delete.begin(), to_delete.end());
        root = helper(root, dict, forest, true);
        return forest;
    }
private:
    TreeNode *helper(TreeNode *node, unordered_set<int>& dict,
                     vector<TreeNode*>& forest, bool is_root) {
        if (!node) {
            return node;
        }
        bool deleted = dict.count(node->val);
        if (is_root && !deleted) {
            forest.push_back(node);
        }
        node->left = helper(node->left, dict, forest, deleted);
        node->right = helper(node->right, dict, forest, deleted);
        return deleted ? nullptr : node
    }
};
}
