/*
 * @lc app=leetcode.cn id=326 lang=cpp
 *
 * [326] 3的幂
 */

// @lc code=start
class Solution {
public:
    bool isPowerOfThree(int n) {
        long a=n;
        if(a>0)
        {
            while(a%3==0)
            {
                a=a/3;
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

