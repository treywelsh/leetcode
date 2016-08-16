#!/usr/bin/env python

#Use the binary decomposition of b and property : a^(2^k) * a^(2^k) = a^(2^(k+1))
#O(log2(b))
class Solution(object):
    def myPow(self, a, b):
        res = 1
        ap2 = a
        b_abs = abs(b)
        while b_abs > 0:
            if b_abs & 1 > 0:
                res *= ap2
            b_abs >>= 1
            ap2 *= ap2
        if b < 0:
            return 1.0/res
        return res

def test_case(a, b):
    s = Solution()
    print str(a) + "^" + str(b) + "=" + str(s.myPow(a,b))

if __name__ == '__main__':
    test_case(2, 0)
    test_case(2, 2)
    test_case(2, -2)
    test_case(2, 10)
