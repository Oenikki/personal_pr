#include <vector>
#include <algorithm> //std::max
#include <unordered_set>
#include <unordered_map>
#include <queue>
#include <stack>

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

namespace hh637 {
class Solution {
public:
	vector<int> averageOfLevels(TreeNode *root) {
		vector<int> res;
		if (!root) {
			return {};
		}
		queue<TreeNode*> q;
		q.push(root);
		while (!q.empty()) {
			int sz = q.size();
			double sum = .0;
			for (int i = 0; i < sz; ++i) {
				auto node = q.front();
				q.pop();
				sum += node->val;
				if (node->left) {
					q.push(node->left);
				}
				if (node->right) {
					q.push(node->right);
				}
			}
			res.push_back(sum / sz);
		}
		return res;
	}
};
}

namespace hh105 {
class Solution {
public:
    TreeNode *buildTree(vector<int>& preorder, vector<int>& inorder) {
        if (preorder.empty()) {
            return nullptr;
        }
        for (int i = 0; i < inorder.size(); ++i) {
            map.emplace(inorder[i], i);
        }
        return buildTreeHelper(preorder, inorder, 0, preorder.size() - 1, 0);
    }
private:
    TreeNode *buildTreeHelper(vector<int>& preorder, vector<int>& inorder, 
                              int inorder_s, int inorder_e, int preorder_s) {
        if (inorder_s > inorder_e) {
            return nullptr;
        }
        int mid = preorder[preorder_s];
        int index = map.at(mid);
        int left_len = index - inorder_s - 1;
        auto node = new TreeNode(mid);
        node->left = buildTreeHelper(inorder_s, index - 1, preorder_s + 1);
        node->right = buildTreeHelper(index + 1, inorder_e, preorder_s + left_len + 2);
        return node;
    }
private:
    unordered_map<int, int> map;
};
}

namespace hh144 {
class Solution {
public:
    vector<int> preorderTraversal(TreeNode *root) {
        vector<int> res;
        if (!root) {
            return res;
        }
        stack<TreeNode*> s;
        s.push(root);
        while (!s.empty()) {
            auto node = s.top();
            s.pop();
            res.push_back(node->val);
            if (node->right) {
                s.push(node->right);
            }
            if (node->left) {
                s.push(node->left);
            }
        }
        return res;
    }
};
}

namespace hh99 {
class Solution {
public:
    void recover(TreeNode *root) {
        if (!root) {
            return;
        }
        TreeNode *mark1 = nullptr;
        TreeNode *mark2 = nullptr;
        TreeNode *prev = nullptr;
        inorder(root, mark1, mark2, prev);
        if (mark1 && mark2) {
            std::swap(mark1->val, mark2->val);
        }
    }
private:
    void inorder(TreeNode *node, TreeNode *&mark1, TreeNode  *&mark2, TreeNode *&prev) {
        if (!node) {
            return;
        }
        if (node->left) {
            inorder(node->left, mark1, mark2, prev);
        }
        if (prev && node->val < prev->val ) {
            if (!mark1) {
                mark1 = prev;
                mark2 = node;
            } else {
                mark2 = node;
            }
        }
        prev = node;
        if (node->right) {
            inorder(node->right, mark1, mark2, prev);
        }
    }
};
}

namespace hh669 {
class Solution {
public:
    
}
}
