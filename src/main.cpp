#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <algorithm>

///////////////////// data structure ///////////////////

typedef std::string element_t;

typedef std::vector<element_t> group_t;

typedef std::vector<group_t> version_t;

// An edit distance between two groups
struct sEditDist
{
    int iv1;  // zero-based index to group in first version
    int iv2;  // zero-based index to group in second version
    int dist; // count of insertions and deletion needs to reach 2nd group from first

    void display();
};

typedef std::vector<sEditDist> vEdits_t;

//////////////////////// Globals //////////////////////////

// the versions
version_t v1, v2;

// the optimal connections
vEdits_t vConnections;

////////////////////// methods //////////////////////////////

/// @brief generate example from stackoverflow question
/// @return
/// https://stackoverflow.com/q/79258933/16582
void gen1()
{
    // g0_0={a,b,c}, g0_1={d,e,f,g,h}, g0_2={i}, and g0_3={j}.
    v1 = version_t{
        {"a", "b", "c"},
        {"d", "e", "f", "g", "h"},
        {"i"},
        {"j"}};

    // g1_0={a,b,c,d}, g1_1={e,f,g,h,i,k}, g1_2={j,l}
    v2 = version_t{
        {"a", "b", "c", "d"},
        {"e", "f", "g", "h", "i", "k"},
        {"j", "l"}};
}
/// @brief edit distance between two groups
/// @param g1
/// @param g2
/// @return
int editDistance(
    const group_t &g1,
    const group_t &g2)
{
    /* Do a linear search through a possible mapping between version 1 and 2 groups
    calculating the edit distance ( number of insertions and deletions reuired ) between them.

    This is the bottleneck.  It is blindingly fast for a few elements 
    O(N) where N is the number of elements ( element present in both versions counts for 2 )
    When there are more than ~100,000 elements this might drag a bit.

    TODO: This can be optimised by sorting the members of each group and doing a binary search

    */
    int dist = 0;
    for (auto &e : g1)
    {
        auto it = std::find(g2.begin(), g2.end(), e);
        if (it == g2.end())
            dist++; // element needs to be added to first group
    }
    for (auto &e : g2)
    {
        auto it = std::find(g1.begin(), g1.end(), e);
        if (it == g1.end())
            dist++; // element needs to be deleted from first group
    }
    return dist;
}

/// @brief connect v1 groups to v2 groups, mimimizing the total edit distance
void Connect()
{
    sEditDist ed;
    for (ed.iv1 = 0; ed.iv1 < v1.size(); ed.iv1++)
    {
        int bestv2 = 0;
        int bestDist = INT_MAX;
        for (ed.iv2 = 0; ed.iv2 < v2.size(); ed.iv2++)
        {
            // calculate edit distance
            ed.dist = editDistance(
                v1[ed.iv1],
                v2[ed.iv2]);
            ed.display();

            // keep the smallest edit distance
            if (ed.dist < bestDist)
            {
                bestDist = ed.dist;
                bestv2 = ed.iv2;
            }
        }
        // add connection with smallest edit distance from g1
        ed.iv2 = bestv2;
        ed.dist = bestDist;
        vConnections.push_back(ed);
    }
}
void ConnectDisplay()
{
    std::cout << "\nOptimal mapping between versions\n";
    for (auto &d : vConnections)
        d.display();
}

void sEditDist::display()
{
    std::cout << iv1 << " -> " << iv2 << ", dist = " << dist << "\n";
}

/////////////// Main //////////////////////

main()
{
    // generate problem
    gen1();

    // make connections
    Connect();

    // display result
    ConnectDisplay();
    return 0;
}
