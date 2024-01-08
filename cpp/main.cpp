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
    vector<vector<int>> combinationSum(vector<int> const& candidates, int const target) {
        vector<vector<int>> result{};

        if (!candidates.empty()) {
            if (1 == candidates.size()) {
                if (candidates[0] == target) {
                    result = {candidates};
                }
            } else {
                using combo_t = std::string;
                std::unordered_set<combo_t> combos{};

//                size_t candidateToIdx[41]; // Candidate value to its corresponding index in 'candidates'.
//                for (size_t i = 0; candidates.size() > i; ++i) { candidateToIdx[candidates[i]] = i; }

                auto const minCandidate = *std::min_element(candidates.begin(), candidates.end());
                auto const minCandidateIsValid = 2 <= minCandidate && 40 >= minCandidate;
                assert(minCandidateIsValid);
                if (minCandidateIsValid) {
                    auto const maxFactorCount = target / minCandidate + (0 == target % minCandidate ? 0 : 1);
                    std::vector<size_t> factors{}; //!< Indexes into candidates (not the candidates themselves).

                    while (true) {
                        // Calculate the next permutation of factors (candidates).
                        size_t rolloverCount = 0;
                        for (auto& factor : factors) {
                            factor = (factor + 1) % candidates.size();
                            if (0 != factor) { break; }
                            ++rolloverCount;
                        }

                        // Detect when all factor permutations have been processed.
                        auto const fullRollover = factors.size() == rolloverCount;
                        if (fullRollover) {
                            // Stop processing when all possible factors have been processed.
                            auto const allFactorPermutationsEvaluated = factors.size() + 1 >= maxFactorCount;
                            if (allFactorPermutationsEvaluated) { break; }

                            // Add the next factor.
                            factors.push_back(0);
                        }

                        // Evaluate sum of all factors.
                        std::remove_cv_t<decltype(target)> sum{};
                        for (auto const factor : factors) {
                            auto const candidate = candidates[factor];
                            if (target - sum < candidate) { sum = 0; break; }
                            sum = sum + candidate;
                        }
                        if (target == sum) {
                            // Record result.
                            combo_t result{};
//
//!\todo TODO: The following is insufficient because two results are 
//             considered identical when the frequency of identical factors
//             in each result are the same!  So there has to be some way to
//             compare results, e.g. by sorting them _before_ recording them.
//             Is there a faster solution than sorting?  Frequency counting?
//
                            std::transform(factors.begin(), factors.end(), std::back_inserter(result), [&](auto const factor){ return candidates[factor]; });
std::sort(result.begin(), result.end());
                            combos.insert(result);
                        }
                    }
                }

                // Convert results into final result and return it.
                for (auto const& value : combos) {
                    vector<int> asVec(value.size());
                    std::copy(value.begin(), value.end(), asVec.begin());
                    result.emplace_back(std::move(asVec));
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
    auto const candidates = std::vector<int>{2,3,6,7};
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

/*
    End of "main.cpp"
*/
