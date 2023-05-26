#pragma once

#include <algorithm>
#include <vector>
#include "neuro_net.h"

#include <memory>
#include <limits>

struct dbg__bird_stat_t
{
    int epoch_start;
    int epoches_count = 0;
    int max_score = 0;
    int min_score = std::numeric_limits<int>::max();
    int sum_score = 0;
};

struct dbg__bird_info
{
    int dist_to_tube;
    int dist_to_bottom;
    int dist_to_hole_top;
    int dist_to_hole_bottom;
    int dist_to_hole_center;
};

class bird {
public:
//    std::unique_ptr<NeuroNet> net;
    int id;
    NeuroNet net;
    int score = 0;
    bool is_alive;
    int p = -1;
    int t = 0;
    int x;
    int y;
    double v;
    double e;

    dbg__bird_info inf;

    dbg__bird_stat_t stat;

    bird(int current_epoch = 0);

    void gen_new_id ();

    void reset_position();

    void mutate_from(const bird & b);

    void move(bool need_jump);

    bool need_to_jump(int X, int Y);

    void check_and_update_is_alive(int tube1_x, int tube1_y, int tube2_x, int tube2_y);

private:
    void update_stats ();
    void set_dead ();
};
