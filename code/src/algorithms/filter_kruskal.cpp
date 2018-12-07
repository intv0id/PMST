#include <stdio.h>
#include <algorithm>
#include <map>
#include <cstdlib>
#include <stdlib.h> 
#include <iostream>

#include "algorithms/filter_kruskal.hpp"
#include "algorithms/kruskal.hpp"
#include "tbb/parallel_sort.h"
#include "common.hpp"

#include "pstl/execution"
#include "pstl/algorithm"

#include <ctime>

using namespace std;

l_edge_t filter_kruskal::algorithm(Graph &g, unsigned int n_threads) {
    cout << endl << "KOIS" << endl;
    l_edge_t result;
    vector<edge*> edges {g.unique_edges.begin(), g.unique_edges.end()};
    union_find* u_find = new union_find(g.n);
    unsigned long x = 0;
    unsigned long* old_size = &x;
    return filter_kruskal_main(g, edges, u_find, old_size);
}

l_edge_t filter_kruskal_main(Graph &g, vector<edge*> &edges, union_find *u, unsigned long* old_size) {

    if (edges.size() < 50 || (*old_size) == edges.size() ) {
        tbb::parallel_sort(edges.begin(), edges.end(), compare);
        return kruskal_main(edges, u);
    }
    else {
        
        int old = *old_size;

        (*old_size) = edges.size();

        clock_t t0 = clock();

        int pivot = find_pivot(edges);
        clock_t t1 = clock();

        auto couple = partition(edges, pivot);
        clock_t t2 = clock();

        l_edge_t partial_solution = filter_kruskal_main(g, couple.first, u, old_size);
        clock_t t3 = clock();

        auto e_plus = filter(couple.second, u);
        clock_t t4 = clock();

        l_edge_t other_solution = filter_kruskal_main(g, e_plus, u, old_size);
        partial_solution.splice(partial_solution.end(), other_solution);
        // partial_solution.merge(filter_kruskal_main(g, e_plus, u, old_size));
        clock_t t5 = clock();

        if (old == 0) {
            cout << "Find pivot: " << t1 - t0 << endl;
            cout << "Partition: " << t2 - t1 << endl;
            cout << "Appel recursif: " << t3 - t2 << endl;
            cout << "Filter: " << t4 - t3 << endl;
            cout << "Merge: " << t5 - t4 << endl;
        }

        return partial_solution;
    }
}

int find_pivot(vector<edge*> &edges) {
    int n = edges.size();
    int pivot_id = rand() % n;
    // pivot_id = 10;
    return edges[pivot_id]->weight;
}

vector<edge*> filter(vector<edge*> &edges, union_find *u_find) {

    vector<edge*> filtered (edges.size());

    auto it = copy_if (pstl::execution::par,
    // auto it = copy_if (
            edges.begin(), edges.end(), filtered.begin(),
            [u_find](edge* e) {
            return u_find->find(e->source) != u_find->find(e->target);
            });

    filtered.resize(distance(filtered.begin(), it));

    return filtered;
}

vector<edge*> old_filter(vector<edge*> &edges, union_find *u_find) {
    vector<edge*> filtered;
    for (auto e : edges) {
        if (u_find->find(e->source) != u_find->find(e->target)) {
            filtered.push_back(e);
        }
    }
    return filtered;
}

pair<vector<edge*>, vector<edge*>> partition(vector<edge*> &edges, int pivot) {
    
    // edge* random_edge = edges.front();

    vector<edge*> e_minus = vector<edge*>();
    vector<edge*> e_plus = vector<edge*>();

    // copy_if (
    copy_if (pstl::execution::par,
            edges.begin(), edges.end(), back_inserter(e_minus),
            [pivot](edge* e) {return e->weight < pivot;});

    // e_minus.resize(distance(e_minus.begin(), it_minus));

    //auto it_plus = copy_if (edges.begin(), edges.end(), e_plus.begin(),
            //[pivot](edge* e) {return e->weight > pivot;});

    // copy_if (
    copy_if (pstl::execution::par,
            edges.begin(), edges.end(), back_inserter(e_plus),
            [pivot](edge* e) {return e->weight >= pivot;});
    
    // e_plus.resize(distance(e_plus.begin(), it_plus));
    //
    return make_pair(e_minus, e_plus);

}

pair<vector<edge*>, vector<edge*>> old_partition(vector<edge*> &edges, int pivot) {

    vector<edge*> e_minus;
    vector<edge*> e_plus;

    for (auto e : edges){
        if (e->weight <= pivot) {
            e_minus.push_back(e);
        }
        else {
            e_plus.push_back(e);
        }
    }

    return make_pair(e_minus, e_plus);
}
