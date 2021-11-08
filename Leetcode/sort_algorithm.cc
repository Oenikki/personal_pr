#include <vector>

using namespace std;
namespace sortAlgoPrev {
//bubble sort
void bubble_sort(vector<int>& nums, int n) {
    bool sorted = true;
    for (size_t i = 0; i < n; ++i) {
        for(size_t j = i; j < n; ++j) {
            if (nums[i] > nums[j]) {
                std::swap(nums[i], nums[j]);
                sorted = false;
            }
        }
        if (sorted) {
            break;
        }
    }
}

void selection_sort(vector<int>& nums, int n) {
    int select = 0;
    for (int i = 0; i < n - 1; ++i) {
        select = i;
        for (int j = i + 1; j < n; ++j) {
            if (nums[j] < num[select]) {
                select = j;
            }
        }
        std::swap(nums[select], nums[j]);
    }
}

void insertion_sort(vector<int>& nums, int n) {

}

}

