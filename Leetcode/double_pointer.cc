#include <vector>
#include <string>
#include <math.h>
using namespace std;

struct ListNode {
    int val;
    ListNode *next;
    ListNode(int x) : val(x), next(nullptr) {}
};

namespace hh167 {
class Solution{
public:
    vector<int> twoSum(vector<int>& numbers, int target) {
        int r = 0, l = numbers.size() - 1;
        int sum = 0;
        while (r < l) {
            sum = numbers[l] + numbers[r];
            if (sum == target) {
                return vector<int> {r + 1, l + 1};
            } else if (sum > target) {
                l--;
            } else {
                r++;
            }
        }
        return {};
    }
};
}

namespace hh88 {
class Solution {
public:
    void merge(vector<int>& nums1, int m, vector<int>& nums2, int n) {
        int pos = m-- + n-- - 1;
        while (m >= 0 && n >= 0) {
            nums1[pos--] = nums1[m] > nums2[n] ? nums1[m--] : nums2[n--];
        }
        while (n >= 0) {
            nums1[pos--] = nums2[n--];
        }
    }
};
}

namespace hh142 {
class Solution{
public:
    ListNode *detectCycle(ListNode *head) {
        ListNode *fast = head;
        ListNode *slow = head;
        do {
            if (!fast || !fast->next) {
                return nullptr;
            }
            slow = slow->next;
            fast = fast->next->next;
        }
        while (fast != slow);
        fast = head;
        while (fast != slow) {
            slow = slow->next;
            fast = fast->next;
        }
        return fast;
    }
};
}

namespace hh76 {
class Solution{
public:
    string minWindow(string s, string t) {
        vector<int> char_num(128, 0);
        vector<bool> char_flag(128, false);
        //get the features of pattern "t"
        for (const char c : t) {
            ++char_num[c];
            char_flag[c] = true;
        }
        //current sliding window [l, r]
        int l = 0;
        int min_l = 0;
        int count = 0;
        int min_size = INT_MAX;
        for (int r = 0; r < s.size(); ++r) {
            if (char_flag[s[r]]) {
                if (--char_num[s[r]] >= 0) {
                    ++count;
                }
                //current window has owns all character of string "t"
                while (count == t.size()) {
                    if (r - l + 1 < min_size) {
                        min_size = r - l + 1;
                        min_l = l;
                    }
                    if (char_flag[s[l]] && ++char_num[s[l]] > 0) {
                        --count;
                    }
                    ++l;
                }

            }
        }
        return min_size > s.size() ? "" : s.substr(min_l, min_size);
    }
};
}

namespace hh633 {
class Solution{
public:
    bool judgeSquareSum(int c) {
        long l = 0;
        long r = (int)sqrt(c);
        /*Attention here
        declare long, not int, otherwise overflow.*/
        while (l <= r) {
            int sum = l * l + r * r;
            if (sum == c) {
                return true;
            } else if (sum < c) {
                l++;
            } else {
                r--;
            }
        }
        return false;
    }
};
}

namespace hh680 {
class Solution{
public:
    bool validPalindrome(string s) {
        int l = 0;
        int r = s.size() - 1;
        for (; l < r && s[l] == s[r]; ++l, --r);
        return isPanlindrome(s, l, r - 1) || isPanlindrome(s, l + 1, r);
    }
private:
    bool isPanlindrome(std::string str, int i, int j) {
        for (; i < j && str[i] == str[j]; ++i, --j);
        return i >= j;
    }
};
}

namespace hh524 {
class Solution{
public:
    string findLongestWord(string s, vector<string>& dictionary) {
        string ans;
        for (const auto& str : dictionary) {
            int i = 0;
            int j = 0;
            while (i < s.length() && j < str.length()) {
                if (s[i] == str[j]) {
                    ++j;
                }
                ++i;
            }
            if (j == str.size()) {
                if (str.length() > ans.length() ||
                    (str.length() == ans.lenght() && str < ans)) {
                    ans = str;
                }
            }
        }
        return ans;
    }
};
}

