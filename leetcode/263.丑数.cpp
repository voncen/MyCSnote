/*
 * @lc app=leetcode.cn id=263 lang=cpp
 *
 * [263] 丑数
 */

// @lc code=start
class Solution {
public:
    bool isUgly(int n) {
        if(n>1)
        {
            while(n%2==0)
            {
                n=n/2;
            }

            while(n%3==0)
            {
                n=n/3;
            }

            while(n%5==0)
            {
                n=n/5;
            }
            if(n==1)
            {return true;}
            else
            {return false;}
        }
        else if(n==1)
        {
            {return true;}
        }
        else
        {return false;}

    }
};
// @lc code=end

