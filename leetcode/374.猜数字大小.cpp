/*
 * @lc app=leetcode.cn id=374 lang=cpp
 *
 * [374] 猜数字大小
 */

// @lc code=start
/** 
 * Forward declaration of guess API.
 * @param  num   your guess
 * @return 	     -1 if num is lower than the guess number
 *			      1 if num is higher than the guess number
 *               otherwise return 0
 * int guess(int num);
 */

class Solution {
public:
    int guessNumber(int n) {
        long left=1,right=n,c=(left+right+1)/2;
        int g=guess(c);
        while(g!=0)
        {
            if(g==1)
            {
                left=c+1;
                c=(left+right+1)/2;
            }
            else if(g==-1)
            {
                right=c-1;
                c=(left+right+1)/2;
            }
            g=guess(c);
        }
        return c;
    }
};
// @lc code=end

