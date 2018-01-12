void world_check_adjacent(int * ts, int variant, int x_relative, int y_relative, int z) {
    // Get the surrounding tiles
    int a = 0;
    int b = 0;
    int c = 0;
    int d = 0;
    int e = 0;
    int f = 0;
    int g = 0;
    int h = 0;

    for (int i = 0; i < MAX_ENTITYS; i++) {
        if (entity_exists(i) && entity_get_type(i) == TILE) {
            if      (x_relative-1 == entity_get_x(i)/(2*TILESIZE) && y_relative-1 == entity_get_y(i)/(2*TILESIZE))  a = entity_get_variant(i) != variant;
            else if (x_relative == entity_get_x(i)/(2*TILESIZE) && y_relative-1 == entity_get_y(i)/(2*TILESIZE))    b = entity_get_variant(i) != variant;
            else if (x_relative+1 == entity_get_x(i)/(2*TILESIZE) && y_relative-1 == entity_get_y(i)/(2*TILESIZE))  c = entity_get_variant(i) != variant;
            else if (x_relative-1 == entity_get_x(i)/(2*TILESIZE) && y_relative == entity_get_y(i)/(2*TILESIZE))    d = entity_get_variant(i) != variant;
            else if (x_relative+1 == entity_get_x(i)/(2*TILESIZE) && y_relative == entity_get_y(i)/(2*TILESIZE))    e = entity_get_variant(i) != variant;
            else if (x_relative-1 == entity_get_x(i)/(2*TILESIZE) && y_relative+1 == entity_get_y(i)/(2*TILESIZE))  f = entity_get_variant(i) != variant;
            else if (x_relative == entity_get_x(i)/(2*TILESIZE) && y_relative+1 == entity_get_y(i)/(2*TILESIZE))    g = entity_get_variant(i) != variant;
            else if (x_relative+1 == entity_get_x(i)/(2*TILESIZE) && y_relative+1 == entity_get_y(i)/(2*TILESIZE))  h = entity_get_variant(i) != variant;
        }
    }

    // Top left
    ts[0] = a*1 + b*2 + d*4;
    if (ts[0] == 6) ts[0] = 7;
    if (ts[0] == 2) ts[0] = 3;
    if (ts[0] == 4) ts[0] = 5;

    // Top right
    ts[1] = b*1 + c*2 + e*8;
    if (ts[1] == 9) ts[1] = 11;
    if (ts[1] == 8) ts[1] = 10;
    if (ts[1] == 1) ts[1] = 3;

    // Bot left
    ts[2] = d*1 + f*4 + g*8;
    if (ts[2] == 9) ts[2] = 13;
    if (ts[2] == 1) ts[2] = 5;
    if (ts[2] == 8) ts[2] = 12;

    // Bot right
    ts[3] = e*2 + g*4 + h*8;
    if (ts[3] == 6) ts[3] = 14;
    if (ts[3] == 4) ts[3] = 12;
    if (ts[3] == 2) ts[3] = 10;
}

void world_update_tile(int id) {
    int type = entity_get_variant(id);
    switch (type) {
        case DIRT: {
            int ts[4];
            int x = entity_get_x(id)/(2 * TILESIZE);
            int y = entity_get_y(id)/(2 * TILESIZE);
            int z = entity_get_z(id)/(2 * TILESIZE);
            world_check_adjacent(ts, type, x, y, z);

            for (int i = 0; i < 4; i++) {
                if (ts[i] != 0) {
                    entity_update_view_variation(id, i, ts[i]+22);
                    entity_update_view_type(id, i, 2);
                }
            }
            break;
        }
        case STONE: {
            for (int i = 0; i < 4; i++) {

            }
            break;
        }
        case SAND: {
            for (int i = 0; i < 4; i++) {

            }
            break;
        }
        case DARK_SAND: {
            for (int i = 0; i < 4; i++) {

            }
            break;
        }
    }
}
