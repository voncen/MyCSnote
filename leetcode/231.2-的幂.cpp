/*
 * @lc app=leetcode.cn id=231 lang=cpp
 *
 * [231] 2 的幂
 */

// @lc code=start
class Solution {
public:
    bool isPowerOfTwo(int n) {
        if(n<1)
        {
            return false;
        }
        else
        {
            while (n%2==0)
            {
                n=n/2; 
            }
            if (n==1)
            {
                return true;
            }
            
            else
            {
                return false;
            }
        }
    }
};
// @lc code=end

