#!/usr/bin/env python

import sys

#a better algorithm exist
def four_sum(nums, target):
    solutions = []
    two_sum = {}
    if len(nums) < 4:
        return []
    #calcul all the 2sums in hash, indexed by sum and containing indexes
    #O(n^2) in time
    for i in xrange(0, len(nums)):
        for j in xrange(i + 1, len(nums)):
            res = nums[i] + nums[j]
            if res in two_sum:
               two_sum[res].append((i, j))
            else:
               two_sum[res] = [(i, j)]
    #looking for target
    keys = two_sum.keys()
    keys.sort()
    #O(n^3) in time O(n^2) in space
    for i in xrange(0, len(keys)):
        ki = keys[i]
        if ki > 0:
            break
        if target - ki not in two_sum:
            continue
        for k in two_sum[ki]:
            for l in two_sum[target - ki]:
                #remove doubles indexes
                if len(set(k) & set(l)) > 0:
                    continue
                #check if already in solutions
                sol = sorted([nums[k[0]], nums[k[1]], nums[l[0]], nums[l[1]]])
                add = True
                for s in solutions:
                    if s == sol:
                        add = False
                        break
                if add:
                    solutions.append(sol)
    return solutions

def test_case(nums, target):
    print "============================="
    print "input : " + str((nums, target))
    print four_sum(nums, target)

if __name__ == '__main__':
    test_case([1, 0, -1, 0, -2, 2], 0)
    test_case([0, 0, 0, 0], 0)
    test_case([-3, -1, 0, 2, 4, 5], 0)
    test_case([1,4,-3,0,0,0,5,0], 0)
    test_case([-1,0,1,2,-1,-4], -1)
    test_case([-3,-1,0,2,4,5], 1)
