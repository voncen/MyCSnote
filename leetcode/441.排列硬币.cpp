/*
 * @lc app=leetcode.cn id=441 lang=cpp
 *
 * [441] 排列硬币
 */

// @lc code=start
class Solution {
public:
    int arrangeCoins(int n) {
        int j=1;
        long i=(sqrt( (long long) 8*n+1)-1)/2+1;
        while(i*(i+1)/2>n)
        {
            i=i-1;
        }
        return i;
    }
};
// @lc code=end
//sqrt((long long) n )

