/*
 * @lc app=leetcode.cn id=69 lang=cpp
 *
 * [69] Sqrt(x)
 */

// @lc code=start
class Solution {
public:
    int mySqrt(int x) {
        double a=1+x/2;
        while(a*a>x+0.1)
        {
            a=(a+x/a)/2;
        }
        return int(a);
    }
};
// @lc code=end
//计算物理牛顿迭代法

