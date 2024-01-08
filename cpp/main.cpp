/*!
    \file "main.cpp"

    Author: Matt Ervin <matt@impsoftware.org>
    Formatting: 4 spaces/tab (spaces only; no tabs), 120 columns.
    Doc-tool: Doxygen (http://www.doxygen.com/)

    https://leetcode.com/problems/combination-sum/
*/

//!\sa https://github.com/doctest/doctest/blob/master/doc/markdown/main.md
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include <algorithm>
#include <array>
#include <cassert>
#include <chrono>
#include <deque>
#include <doctest/doctest.h> //!\sa https://github.com/doctest/doctest/blob/master/doc/markdown/tutorial.md
#include <iterator>
#include <memory>
#include <queue>
#include <unordered_set>
#include <vector>
#include <set>
#include <span>
#include <stack>

using namespace std;

// [----------------(120 columns)---------------> Module Code Delimiter <---------------(120 columns)----------------]

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
    virtual ~TreeNode() {
        if (left) {
            delete left;
        }
        if (right) {
            delete right;
        }
    }
};

class Solution {
public:
    using freqCombo_t = std::array<size_t, 41>;

    struct freqComboHash_t {
        std::size_t operator()(freqCombo_t const& value) const noexcept {
            // https://algs4.cs.princeton.edu/34hash/#:~:text=The%20most%20commonly%20used%20method,between%200%20and%20M%2D1.
            size_t result{};
            for (auto const element : value) {
                result = static_cast<size_t>(31 * result + element);
            }
            return result;
        }
    };

    /*
        Sort the candidates.
        When the sum exceeds 'target' then there is no way that even more 
        factors will produce a valid sum, so abandon remaining combinations
        produced from the stem that produced the first invalid sum.
        a
        a b
        a b c > sum!  Stop evaluating from c on, i.e. skip those.
        a c
        ...
    */
    vector<vector<int>> combinationSum(vector<int>& candidates, int const target) {
        vector<vector<int>> result{};

        if (!candidates.empty()) {
            if (1 == candidates.size()) {
                if (candidates[0] == target) {
                    result = {candidates};
                }
            } else {
                std::sort(candidates.begin(), candidates.end());

                std::unordered_set<freqCombo_t, freqComboHash_t> freqCombos{};

                std::deque<int> factorStack{};
                int sum = 0;
                size_t factor = 0;
                while (true) {
                    bool moveToNextParentFactor = true;

                    auto const sumWithinTargetRange = target - sum >= candidates[factor];
                    if (sumWithinTargetRange) {
                        auto const targetFound = target - sum == candidates[factor];
                        if (targetFound) {
                            // Create and record factor frequencies for this combination of factors.
                            freqCombo_t freqCombo{};
                            for (auto const stackedFactor : factorStack) { ++freqCombo[stackedFactor]; }
                            ++freqCombo[factor];
                            freqCombos.insert(freqCombo); // Records the same value only once.
                        } else {
                            factorStack.push_back(factor);
                            sum += candidates[factor];
                            factor = 0; // Restart sequence.
                            moveToNextParentFactor = false;
                        }
                    }
                    
                    if (moveToNextParentFactor) {
                        if (factorStack.empty()) {
                            factor = (factor + 1) % candidates.size();
                        } else {
                            while (!factorStack.empty()) {
                                factor = (factorStack.back() + 1) % candidates.size();
                                sum -= candidates[factorStack.back()];
                                factorStack.pop_back();
                                if (0 != factor) { break; }
                            }
                        }
                    }
                    
                    auto const allCombinationsVisited = 0 == factor && factorStack.empty();
                    if (allCombinationsVisited) { break; }
                }

                // Convert factor combination frequencies into candidate combinations.
                for (auto const& freqCombo : freqCombos) {
                    vector<int> combo{};
                    // For each factor in the combination:
                    for (size_t factor = 0; freqCombo.size() > factor; ++factor) {
                        // Emit "frequency count" candidates.
                        for (auto count = freqCombo[factor]; count; --count) {
                            combo.push_back(candidates[factor]);
                        }
                    }
                    result.emplace_back(std::move(combo));
                }
            }
        }

        return result;
    }
};

// [----------------(120 columns)---------------> Module Code Delimiter <---------------(120 columns)----------------]

struct elapsed_time_t
{
    std::chrono::steady_clock::time_point start{};
    std::chrono::steady_clock::time_point end{};
    
    elapsed_time_t(
        std::chrono::steady_clock::time_point start
        , std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now()
    ) : start{std::move(start)}, end{std::move(end)} {}
};

std::ostream&
operator<<(std::ostream& os, elapsed_time_t const& et)
{
    auto const elapsed_time = et.end - et.start;
    os << std::chrono::duration_cast<std::chrono::microseconds>(elapsed_time).count()
       << '.' << (std::chrono::duration_cast<std::chrono::microseconds>(elapsed_time).count() % 1000)
       << " ms";
    return os;
}

TreeNode* createBst(std::vector<int> values) {
    TreeNode* root = nullptr;

    for (auto value : values) {
        // Search for parent node, if any.
        TreeNode* parent{};
        for (TreeNode* iter = root; iter; ) {
            parent = iter;
            iter = value < iter->val ? iter->left : iter->right;
        }
        
        // Find parent pointer.
        TreeNode** parentPtr{};
        if (parent) {
            if (value < parent->val) {
                parentPtr = &parent->left;
            } else {
                parentPtr = &parent->right;
            }
        } else {
            parentPtr = &root;
        }
        
        // Allocate new node.
        if (parentPtr) {
            *parentPtr = new TreeNode{std::move(value)};

            if (!root) {
                root = *parentPtr;
            }
        }
    }

    return root;
}

std::vector<int> inorder(TreeNode const* root) {
    std::vector<int> result{};

    if (root) {
        auto const leftResult = inorder(root->left);
        std::copy(leftResult.begin(), leftResult.end(), std::back_inserter(result));

        result.push_back(root->val);

        auto const rightResult = inorder(root->right);
        std::copy(rightResult.begin(), rightResult.end(), std::back_inserter(result));
    }

    return result;
}

std::vector<int> preorder(TreeNode const* root) {
    std::vector<int> result{};

    if (root) {
        result.push_back(root->val);

        auto const leftResult = preorder(root->left);
        std::copy(leftResult.begin(), leftResult.end(), std::back_inserter(result));

        auto const rightResult = preorder(root->right);
        std::copy(rightResult.begin(), rightResult.end(), std::back_inserter(result));
    }

    return result;
}

constexpr auto const null = (std::numeric_limits<int>::min)();

/*!
    \brief Convert array of node values in level order to a tree of nodes.

    The array of node values is formatted like those used for leetcode problems.
    E.g. [3,9,20,null,null,15,7,null,null,null,null,30,40]
         describes:
                       03
               09              20
           --      --      15      07
         --  --  --  --  30  40  --  --
         Note that the '--'s at the end of the bottom row are excluded from the array.
*/
std::unique_ptr<TreeNode> createLevelOrderBT(std::vector<int> const& levelOrderNodes) {
    std::unique_ptr<TreeNode> result{};
    
    std::deque<TreeNode*> treeNodes{};
    size_t levelWidth = 1; // Node count in current level.
    size_t levelOrderNodesIdx = 0;
    while (levelOrderNodes.size() > levelOrderNodesIdx) {
        // Process all [child] nodes in current tree level.
        TreeNode* parent = nullptr;
        for (auto childIdx = 0
             ; levelWidth > childIdx
               && levelOrderNodes.size() > levelOrderNodesIdx
             ; ++childIdx
        ) {
            auto const firstChild = 0 == childIdx % 2;
            
            // Determine parent node for new child nodes.
            parent = !firstChild
                     ? parent // Continue using same parent.
                     : treeNodes.empty()
                       ? nullptr
                       : [&]{ auto r = treeNodes.front(); treeNodes.pop_front(); return r; }();
            
            auto const nodeValue = levelOrderNodes[levelOrderNodesIdx++];
            // Create new child node.
            treeNodes.push_back(null == nodeValue ? nullptr : new TreeNode{nodeValue});
            if (null != nodeValue) {
                if (parent) { (firstChild ? parent->left : parent->right) = treeNodes.back(); }
                if (!result) { result.reset(treeNodes.back()); }
            }
        }

        levelWidth *= 2;
    }

    return result;
}

/*!
    \brief Convert a tree of nodes to array of node values in level order.

    E.g. [3,9,20,null,null,15,7,null,null,null,null,30,40]
         describes:
                       03
               09              20
           --      --      15      07
         --  --  --  --  30  40  --  --
         Note that the '--'s at the end of the bottom row are excluded from the array.
*/
std::vector<int> btToLevelOrder(TreeNode* root) {
    std::vector<int> result{};

    std::queue<TreeNode*> levelNodes{};
    levelNodes.push(root);
    auto lastValidResultSize = result.size();
    while (!levelNodes.empty()) {
        bool atLeastOneValidNodeEnqueued = false;
        for (auto levelNodeCount = levelNodes.size(); levelNodeCount; --levelNodeCount) {
            auto node = levelNodes.front();
            levelNodes.pop();

            if (node) {
                levelNodes.push(node->left);
                levelNodes.push(node->right);
                result.push_back(node->val);
                lastValidResultSize = result.size();
                atLeastOneValidNodeEnqueued = true;
            } else {
                levelNodes.push(nullptr);
                levelNodes.push(nullptr);
                result.push_back(null);
            }
        }
        if (!atLeastOneValidNodeEnqueued) {
            result.erase(result.begin() + lastValidResultSize, result.end());
            break;
        }
    }

    return result;
}

TEST_CASE("Case 1")
{
    cerr << "Case 1" << '\n';
    auto candidates = std::vector<int>{2,3,6,7};
    auto const target = 7;
    auto const expected = [&]{
        auto result = std::vector<std::vector<int>>{
            {2,2,3}
            , {7}
        };
        for (auto& r : result) { std::sort(r.begin(), r.end()); }
        std::sort(result.begin(), result.end());
        return result;
    }();
    { // New scope.
        auto const start = std::chrono::steady_clock::now();
        auto const result = [&]{
            auto result = Solution{}.combinationSum(candidates, target);
            for (auto& r : result) { std::sort(r.begin(), r.end()); }
            std::sort(result.begin(), result.end());
            return result;
        }();
        CHECK(result == expected);
        cerr << "Elapsed time: " << elapsed_time_t{start} << '\n';
    }
    cerr << '\n';
}

TEST_CASE("Case 2")
{
    cerr << "Case 2" << '\n';
    auto candidates = std::vector<int>{2,3,5};
    auto const target = 8;
    auto const expected = [&]{
        auto result = std::vector<std::vector<int>>{
            {2,2,2,2}
            , {2,3,3}
            , {3,5}
        };
        for (auto& r : result) { std::sort(r.begin(), r.end()); }
        std::sort(result.begin(), result.end());
        return result;
    }();
    { // New scope.
        auto const start = std::chrono::steady_clock::now();
        auto const result = [&]{
            auto result = Solution{}.combinationSum(candidates, target);
            for (auto& r : result) { std::sort(r.begin(), r.end()); }
            std::sort(result.begin(), result.end());
            return result;
        }();
        CHECK(result == expected);
        cerr << "Elapsed time: " << elapsed_time_t{start} << '\n';
    }
    cerr << '\n';
}

TEST_CASE("Case 3")
{
    cerr << "Case 3" << '\n';
    auto candidates = std::vector<int>{2};
    auto const target = 1;
    auto const expected = [&]{
        auto result = std::vector<std::vector<int>>{};
        for (auto& r : result) { std::sort(r.begin(), r.end()); }
        std::sort(result.begin(), result.end());
        return result;
    }();
    { // New scope.
        auto const start = std::chrono::steady_clock::now();
        auto const result = [&]{
            auto result = Solution{}.combinationSum(candidates, target);
            for (auto& r : result) { std::sort(r.begin(), r.end()); }
            std::sort(result.begin(), result.end());
            return result;
        }();
        CHECK(result == expected);
        cerr << "Elapsed time: " << elapsed_time_t{start} << '\n';
    }
    cerr << '\n';
}

#include <iostream>

TEST_CASE("Case 4")
{
    cerr << "Case 4" << '\n';
    auto candidates = std::vector<int>{4,8,11,10,9,3,12,7,6};
    auto const target = 25;
    auto const expected = [&]{
        auto result = std::vector<std::vector<int>>{
            {3,3,3,3,3,3,3,4}
            , {3,3,3,3,3,3,7}
            , {3,3,3,3,3,4,6}
            , {3,3,3,3,3,10}
            , {3,3,3,3,4,9}
            , {3,3,3,3,6,7}
            , {3,3,3,4,4,4,4}
            , {3,3,3,4,4,8}
            , {3,3,3,4,6,6}
            , {3,3,3,4,12}
            , {3,3,3,6,10}
            , {3,3,3,7,9}
            , {3,3,3,8,8}
            , {3,3,4,4,4,7}
            , {3,3,4,4,11}
            , {3,3,4,6,9}
            , {3,3,4,7,8}
            , {3,3,6,6,7}
            , {3,3,7,12}
            , {3,3,8,11}
            , {3,3,9,10}
            , {3,4,4,4,4,6}
            , {3,4,4,4,10}
            , {3,4,4,6,8}
            , {3,4,4,7,7}
            , {3,4,6,6,6}
            , {3,4,6,12}
            , {3,4,7,11}
            , {3,4,8,10}
            , {3,4,9,9}
            , {3,6,6,10}
            , {3,6,7,9}
            , {3,6,8,8}
            , {3,7,7,8}
            , {3,10,12}
            , {3,11,11}
            , {4,4,4,4,9}
            , {4,4,4,6,7}
            , {4,4,6,11}
            , {4,4,7,10}
            , {4,4,8,9}
            , {4,6,6,9}
            , {4,6,7,8}
            , {4,7,7,7}
            , {4,9,12}
            , {4,10,11}
            , {6,6,6,7}
            , {6,7,12}
            , {6,8,11}
            , {6,9,10}
            , {7,7,11}
            , {7,8,10}
            , {7,9,9}
            , {8,8,9}
        };
        for (auto& r : result) { std::sort(r.begin(), r.end()); }
        std::sort(result.begin(), result.end());
        return result;
    }();
    { // New scope.
        auto const start = std::chrono::steady_clock::now();
        auto const result = [&]{
            auto result = Solution{}.combinationSum(candidates, target);
            for (auto& r : result) { std::sort(r.begin(), r.end()); }
            std::sort(result.begin(), result.end());
#if 0
for (auto const& a : result) {
    cout << '{';
    bool first = true;
    for (auto const& b : a) {
        if (first) { first = false; }
        else { cout << ','; }
        cout << b;
    }
    cout << '}' << '\n';
}
cout << std::flush;
#endif // #if 0
            return result;
        }();
        CHECK(result == expected);
        cerr << "Elapsed time: " << elapsed_time_t{start} << '\n';
    }
    cerr << '\n';
}

/*
    End of "main.cpp"
*/
