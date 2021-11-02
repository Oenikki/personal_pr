#include <vector>
#include <iostream>
using namespace std;
/* Keep left closed and right opened.*/

namespace hh69 {
class Solution {
    int mySqrt(int x) {
        int l = 1, r = x, mid = 0, temp = 0;
        while (l <= r) {
            mid = l + (r - l) / 2;
            temp = x / mid;
            if (temp = mid) {
                return mid;
            } else if (temp > mid) {
                l = mid + 1;
            } else {
                r = mid - 1;
            }
        }
        return r;
    }
};
}

namespace hh34 {
class Solution {
public:
    vector<int> searchRange(vector<int>& nums, int target) {
        int lower = findLeftBound(nums, target);
        int upper = findRightBound(nums, target);
        if (lower >= nums.size() || nums[lower] != target) {
            return { -1, -1 };
        }
        return { lower, upper };
    }

private:
    int findLeftBound(vector<int>& nums, int target) {
        int l = 0, r = nums.size();
        while (l < r) {
            int mid = l + (r - l) / 2;
            if (nums[mid] < target) {
                l = mid + 1;
            } else {
                r = mid;
            }
        }
        cout << "l: " << l << " " << "r: " << r << endl;
        return l;
    }

    int findRightBound(vector<int>& nums, int target) {
        int l = 0, r = nums.size();
        while (l < r) {
            int mid = l + (r - l) / 2;
            if (nums[mid] <= target) {
                l = mid + 1;
            } else {
                r = mid;
            }
        }
        cout << "l: " << l << " " << "r: " << r << endl;
        return l - 1;
    }
};
}

namespace hh81 {
class Solution {
public:
    bool search(vector<int>& nums, int target) {
        int l = 0, r = nums.size() - 1;
        while (l <= r) {
            int mid = l + (r - l) / 2;
            if (nums[mid] == target) {
                return true;
            }
            if (nums[l] == nums[mid]) { //can not infer sequential interval
                l++;
            } else if (nums[l] < nums[mid]) { //interval [l, mid] is ordered
                if (nums[l] <= target && target < nums[mid]) {
                    r = mid - 1;
                } else {
                    l = mid + 1;
                }
            } else { //interval [mid, r] is ordered
                if (nums[mid] < target && target <= nums[r]) {
                    l = mid + 1;
                } else {
                    r = mid - 1;
                }
            }
        }
        return false;
    }
};
}

int main() {
    std::vector<int> nums = {5, 7, 7, 8, 8, 10};
    int target = 8;
    hh34::Solution().searchRange(nums, target);
    return 0;
}
