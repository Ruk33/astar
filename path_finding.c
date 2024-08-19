int find_path(float *result, float *start, float *end);

int point_is_walkable(float *point);

int point_has_obstacle(float *point);

int points_are_equal(float *a, float *b);

float points_distance(float *a, float *b);

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
int find_path(float *result, float *start, float *end)
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
    int max_tries = 4096;
    if (!start)
        return 0;
    if (!end)
        return 0;
    // add the start point to the open list.
    int nodes_count = 1;
    nodes[0] = (struct node) {0};
    nodes[0].position[0] = start[0];
    nodes[0].position[1] = start[1];
    for (int tries = 0; tries < max_tries; tries++) {
        struct node *current = 0;
        // find non-visited node with lowest f.
        for (int i = 0; i < nodes_count; i++) {
            if (nodes[i].visited)
                continue;
            if (!current) {
                current = nodes + i;
                continue;
            }
            float current_f = current->g + current->h;
            float node_f = nodes[i].g + nodes[i].h;
            if (node_f < current_f)
                current = nodes + i;
        }
        // nothing more to process. the path couldn't be found.
        if (!current)
            return 0;
        // mark the node as visited so we won't check it again.
        current->visited = 1;
        // found path!
        if (points_are_equal(current->position, end)) {
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
            { current->position[0] - celld, current->position[1] },
            // right.
            { current->position[0] + celld, current->position[1] },
            // up.
            { current->position[0], current->position[1] - celld },
            // down.
            { current->position[0], current->position[1] + celld },
        };
        int neighbors_count = 4;
        for (int j = 0; j < neighbors_count; j++) {
            int already_checked = 0;
            // check if position already visited.
            for (int k = nodes_count - 1; k >= 0; k--) {
                if (nodes[k].position[0] == neighbor[j][0] &&
                    nodes[k].position[1] == neighbor[j][1] &&
                    nodes[k].visited) {
                    already_checked = 1;
                    break;
                }
            }
            // skip if already checked.
            if (already_checked)
                continue;
            // don't add neighbor if it's already in the list
            // and it has a lower g
            int already_included_with_lower_g = 0;
            for (int k = nodes_count - 1; k >= 0; k--) {
                if (nodes[k].position[0] == neighbor[j][0] &&
                    nodes[k].position[1] == neighbor[j][1] &&
                    !nodes[k].visited &&
                    nodes[k].g < current->g + celld) {
                    already_included_with_lower_g = 1;
                    break;
                }
            }
            if (already_included_with_lower_g)
                continue;
            // check if the cell can be walked.
            if (!point_is_walkable(neighbor[j]))
                continue;
            // check if there is an obstacle in the cell.
            if (point_has_obstacle(neighbor[j]))
                continue;
            // add neighbor.
            nodes[nodes_count] = (struct node) {0};
            // distance between child and parent.
            nodes[nodes_count].g = current->g + celld;
            nodes[nodes_count].h = points_distance(neighbor[j], end);
            nodes[nodes_count].position[0] = neighbor[j][0];
            nodes[nodes_count].position[1] = neighbor[j][1];
            nodes[nodes_count].parent = current;
            nodes_count++;
        }
    }
    return 0;
}

int point_is_walkable(float *point)
{
    // write your implementation.
    return 1;
}

int point_has_obstacle(float *point)
{
    // write your implementation.
    return 0;
}

int points_are_equal(float *a, float *b)
{
    // write your implementation.
    // this is just an example.
    float e = 0.01f;
    float dx = a[0] - b[0];
    float dy = a[1] - b[1];
    int result = dx <= e && dy <= e;
    return result;
}

float points_distance(float *a, float *b)
{
    float dx = b[0] - a[0];
    float dy = b[1] - a[1];
    float result = dx * dx + dy * dy;
    return result;
}
