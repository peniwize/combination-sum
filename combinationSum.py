import time

"""
candidates = [2,3,6,7]
target = 7
WARNING: Input must be sorted for this method to work!

    *           *
2222222|3333|66|7
 222233  336  6
  223 3   3
   2


candidates = [2,3,5]
target = 8
WARNING: Input must be sorted for this method to work!

   *    *      *
22222222222|3333|55
 2222223355  335  5
  22335 3 5   3
   2 3


comboSum():
    combo += values[valuesIdx]
    sum += values[valuesIdx]
    if target > sum:
        while len(values) > valuesIdx:
            comboSum(values, valuesIdx, combo)
            sum -= combo.pop()
            valuesIdx += 1
    elif target <= sum:
        if target == sum:
            result += combo

        sum -= combo.pop()
"""

"""
candidates = [2,3,5]
target = 8

   *    *      *
22222222222|3333|55
 2222223355  335  5
  22335 3 5   3
   2 3
"""
def comboSumRecursive(values, target, result = None, valuesIdx = None, combo = None, sum = None):
    if None == result: result = []
    if None == valuesIdx: valuesIdx = 0
    if None == combo: combo = []
    if None == sum: sum = 0
    while len(values) > valuesIdx and target - sum >= values[valuesIdx]:
        if target - sum == values[valuesIdx]:
            combo.append(values[valuesIdx])
            result.append(combo[:])
            combo.pop()
        else:
            combo.append(values[valuesIdx])
            sum += values[valuesIdx]
            result, sum = comboSumRecursive(values, target, result, valuesIdx, combo, sum)
            sum -= combo.pop()
        valuesIdx += 1
    return result, sum

def comboSum(values, target):
    result, sum = comboSumRecursive(sorted(values), target)
    return result

if "__main__" == __name__:
    values = [2, 3, 6, 7] # Produces: [[2,2,3],[7]]
    target = 7
    print("comboSum({}, {})".format(values, target))
    print("[")
    start = time.time()
    result = comboSum(values, target)
    end = time.time()
    for combo in result:
        print("  {}".format(combo))
    print("]")
    print("Elapsed: {}".format(end - start))

    values = [2, 3, 5] # Produces: [[2,2,2,2],[2,3,3],[3,5]]
    target = 8
    print("comboSum({}, {})".format(values, target))
    print("[")
    start = time.time()
    result = comboSum(values, target)
    end = time.time()
    for combo in result:
        print("  {}".format(combo))
    print("]")
    print("Elapsed: {}".format(end - start))

    values = [2] # Produces: []
    target = 1
    print("comboSum({}, {})".format(values, target))
    print("[")
    start = time.time()
    result = comboSum(values, target)
    end = time.time()
    for combo in result:
        print("  {}".format(combo))
    print("]")
    print("Elapsed: {}".format(end - start))

    values = [4,8,11,10,9,3,12,7,6]
    target = 25
    print("comboSum({}, {})".format(values, target))
    print("[")
    start = time.time()
    result = comboSum(values, target)
    end = time.time()
    for combo in result:
        print("  {}".format(combo))
    print("]")
    print("Elapsed: {}".format(end - start))
