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

namespace hh154 {
class Solution{
    int findMin(vector<int>& nums) {
        int l = 0, r = nums.size() - 1;
        while (l < r) {
            int mid = l + (r - l) / 2;
            if (nums[mid] < nums[r]) {
                r = mid;
            } else if (nums[mid] > nums[r]) {
                l = mid + 1;
            } else {
                --r;
            }
        }
        return nums[l];
    }

    int findMin2(vector<int>& nums) {
        int l = 0;
        int r = nums.size() - 1;
        if (nums[r] > nums[l]) {
            return nums[l];
        }
        while (l <= r) {
            if (nums[r] > nums[l]) {
                return nums[l];
            }
            int mid = l + (r - l) / 2;
            if (nums[l] < nums[mid]) {
                l = mid + 1;
            } else if (nums[l] > nums[mid]) {
                r = mid;
                l++;
            } else {
                l++;
            }
        }
        return nums[mid];
    }
};
}

int main() {
    std::vector<int> nums = {2, 2, 2, 0, 1};
    int target = 8;
    hh154::Solution().findMin2(nums);
    return 0;
}
