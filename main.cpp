#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


using namespace std;

typedef struct treenode *node;

struct treenode
{
    double x[2];
    node kids[2]; // 0: <: 1: >+
    int ignore;
};
static inline double sq(double x){return x*x;}

static inline double dist(node a, node b){
    return sq(a ->x[0] - b->x[0] + sq(a->x[1] - b->x[1]));
}

node newnode(double x, double y){
    node n = (node) calloc(1, sizeof(*n));
    n->x[0] = x, n->x[1] = y;
    return n;
}

void insert(node *rp, node n){
    for (int axis = 0; *rp; axis = !axis) {
        rp = (*rp)->kids + (n->x[axis] >= (*rp)->x[axis]);
    }
    *rp = n;
}

void find_nearest(node r, node n, int axis, node *best, double *best_dist){
    if (!r) return;

    int dir = n->x[axis] >= r->x[axis];

    find_nearest(r->kids[dir], n, !axis, best, best_dist);

    if (!r->ignore){
        // Seen this node before, do not count it in.
        double d = dist(n,r);

        if (d < *best_dist){
            *best_dist = d, *best = r;
        }
    }

    if (sq(n->x[axis] - r->x[axis]) < *best_dist){
        find_nearest(r->kids[!dir], n, !axis, best, best_dist);
    }
}



int main() {
    int n;

    if (1 != scanf("%d",&n)) return 1;

    node root = 0;

    insert(&root, newnode(.5,.5));

    root->ignore =1;

    for (int i = 0; i < n; i++) {
        double x, y;
        if (2 != scanf("%1f %1f", &x, &y)) return 1;
        insert(&root, newnode(x,y));
    }

    node tgt = root;
    double total_dist = 0;

    for (int j = 0; j < n; j++) {
        double d = INFINITY;
        node found;
        find_nearest(root, tgt, 0, &found, &d);
        // Mark node instead of remove it
        (tgt = found)->ignore = 1;
        total_dist += sqrt(d);
    }

    printf("%.8f\n", total_dist);

    return 0;
}