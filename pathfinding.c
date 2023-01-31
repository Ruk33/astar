int pointsareequal
(float *a, float *b);

int pointiswalkable
(float *point);

float pointsdistance
(float *a, float *b);

int pointhasobstacle
(float *point);

// find path from start to end.
// store the next step in result.
// 
// example, if we have a map/grid like:
// [s] [x] [x] [x] [e]
// where s = start and e = end, the result
// of the first call of this function would
// be (r):
// [s] [r] [x] [x] [e]
// now, you can call this function again from
// r to get closer and closer:
// [x] [s] [x] [x] [e]
// resulting in:
// [x] [s] [r] [x] [e]
// 
// result can be NULL (useful if you only want to know if a path is possible)
// on success, 1 is returned.
// on failure, 0 is returned.
int findpath
(float *result, float *start, float *end)
{
    // this struct probably won't be used outside this function so
    // we may as well just define it in here and that's it.
    struct node {
        int visited;
        float g;
        float h;
        float position[2];
        struct node *parent;
    };
    // increase or decrease as needed.
    // default: 8192
    static struct node nodes[8192] = {0};
    // prevent infinite loops.
    // default: 4096
    int maxtries = 4096;
    if (!start)
        return 0;
    if (!end)
        return 0;
    // add the start point to the open list.
    int nodescount = 1;
    nodes[0] = (struct node) {0};
    nodes[0].position[0] = start[0];
    nodes[0].position[1] = start[1];
    for (int tries = 0; tries < maxtries; tries++) {
        struct node *current = 0;
        // find non-visited node with lowest f.
        for (int i = 0; i < nodescount; i++) {
            if (nodes[i].visited)
                continue;
            if (!current) {
                current = nodes + i;
                continue;
            }
            float currentf = current->g + current->h;
            float nodef = nodes[i].g + nodes[i].h;
            if (nodef < currentf)
                current = nodes + i;
        }
        // nothing more to process. the path couldn't be found.
        if (!current)
            return 0;
        // mark the node as visited so we won't check it again.
        current->visited = 1;
        // found path!
        if (pointsareequal(current->position, end)) {
            if (result) {
                // find the next step from start.
                while (current->parent) {
                    result[0] = current->position[0];
                    result[1] = current->position[1];
                    current = current->parent;
                }
            }
            return 1;
        }
        // cell dimension. i'm assuming you have a map defined by a grid.
        // cell dimension refers to each cell in that grid.
        // default: 64
        float celld = 64;
        // cells close to the current node.
        float neighbor[][2] = {
            // left.
            {current->position[0] - celld, current->position[1]},
            // right.
            {current->position[0] + celld, current->position[1]},
            // up.
            {current->position[0], current->position[1] - celld},
            // down.
            {current->position[0], current->position[1] + celld},
        };
        int neighborscount = 4;
        for (int j = 0; j < neighborscount; j++) {
            int alreadychecked = 0;
            // check if position already visited.
            for (int k = nodescount - 1; k >= 0; k--) {
                if (nodes[k].position[0] == neighbor[j][0] &&
                    nodes[k].position[1] == neighbor[j][1] &&
                    nodes[k].visited) {
                    alreadychecked = 1;
                    break;
                }
            }
            // skip if already checked.
            if (alreadychecked)
                continue;
            // don't add neighbor if it's already in the list
            // and it has a lower g
            int alreadyincludedwithlowerg = 0;
            for (int k = nodescount - 1; k >= 0; k--) {
                if (nodes[k].position[0] == neighbor[j][0] &&
                    nodes[k].position[1] == neighbor[j][1] &&
                    !nodes[k].visited &&
                    nodes[k].g < current->g + celld) {
                    alreadyincludedwithlowerg = 1;
                    break;
                }
            }
            if (alreadyincludedwithlowerg)
                continue;
            // check if the cell can be walked.
            if (!pointiswalkable(neighbor[j]))
                continue;
            // check if there is an obstacle in the cell.
            if (pointhasobstacle(neighbor[j]))
                continue;
            // add neighbor.
            nodes[nodescount] = (struct node) {0};
            // distance between child and parent.
            nodes[nodescount].g = current->g + celld;
            nodes[nodescount].h = pointsdistance(neighbor[j], end);
            nodes[nodescount].position[0] = neighbor[j][0];
            nodes[nodescount].position[1] = neighbor[j][1];
            nodes[nodescount].parent = current;
            nodescount++;
        }
    }
    return 0;
}

int pointsareequal
(float *a, float *b)
{
    // write your implementation.
    // this is just an example.
    float e = 0.01f;
    float dx = a[0] - b[0];
    float dy = a[1] - b[1];
    int result = dx <= e && dy <= e;
    return result;
}

int pointiswalkable
(float *point)
{
    // write your implementation.
    return 1;
}

float pointsdistance
(float *a, float *b)
{
    if (!a)
        return 0;
    if (!b)
        return 0;
    float dx = b[0] - a[0];
    float dy = b[1] - a[1];
    float result = dx * dx + dy * dy;
    // optional.
    // result = sqrtf(result);
    return result;
}

int pointhasobstacle
(float *point)
{
    // write your implementation.
    return 0;
}
