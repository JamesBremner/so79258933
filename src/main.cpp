#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <algorithm>

typedef std::string element_t;

typedef std::vector<element_t> group_t;

typedef std::vector<group_t> version_t;

version_t v1, v2;

struct sEditDist
{
    int iv1;
    int iv2;
    int dist;

    void display()
    {
        std::cout << iv1 << " " << iv2 << " " << dist << "\n";
    }
};

typedef std::vector<sEditDist> vEdits_t;

// optimal connections
vEdits_t vConnections;

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
    int dist = 0;
    for (auto &e : g1)
    {
        auto it = std::find(g2.begin(), g2.end(), e);
        if (it == g2.end())
            dist++;
    }
    for (auto &e : g2)
    {
        auto it = std::find(g1.begin(), g1.end(), e);
        if (it == g1.end())
            dist++;
    }
    return dist;
}
/// @brief calculate edit distances between every pair of groups in the versions
void editDistance()
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
            if( ed.dist < bestDist) {
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
void editDistanceDisplay()
{
    for (auto &d : vConnections)
        d.display();
}

main()
{
    gen1();
    editDistance();
    editDistanceDisplay();
    return 0;
}
