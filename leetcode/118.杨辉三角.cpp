/*
 * @lc app=leetcode.cn id=118 lang=cpp
 *
 * [118] 杨辉三角
 */

// @lc code=start
class Solution {
public:
    vector<vector<int>> generate(int numRows) {
        list<list> yanghui={};
        list<int> a={1};
        list<int> b={1};
        for(i=1;i<numRows+1;i++)
        {
            
            for(j=2;j<i+1;j++)
            {
                b.add()
            }
            yanghui[i]=a;
        }
        return yanghui;
    }

};
// @lc code=end

