#include <vector>
using namespace std;

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

namespace hh
