/*
 * @lc app=leetcode.cn id=342 lang=cpp
 *
 * [342] 4的幂
 */

// @lc code=start
class Solution {
public:
    bool isPowerOfFour(int n) {
        long a=n;
        if(a>0)
        {
            while(a%4==0)
            {
                a=a/4;
            }
            if(a==1)
            {
                return true;
            }    
            else
            {
                return false;
            }
        }
        else
        {
            return false;
        }
    }
};
// @lc code=end

