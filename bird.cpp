#include "bird.h"
#include <fstream>


bird::bird(int current_epoch)
    : net(NeuroNet({1, 3, 1}))
{
    gen_new_id();
    stat.epoch_start = current_epoch;
    reset_position();
}

void bird::gen_new_id ()
{
    static int last_used_id = 0;
    ++last_used_id;
    id = last_used_id;
    stat = dbg__bird_stat_t();
}

double norm(double value, double min, double max) {
    return 2 * (value - min) / (max - min) - 1;
}
double norm2(double value, double min, double max) {
    return (value - min) / (max - min);
}

void bird::reset_position ()
{
    is_alive = true;
    e = 0;
    v = 0;
    x = 100;
    y = 200;
}

void bird::mutate_from(const bird & b)
{
//    int id;
//    NeuroNet net;
//    int score = 0;
//    bool is_alive;
//    int p = -1;
//    int t = 0;
//    int x;
//    int y;
//    double v;
//    double e;

    *this = b;

    gen_new_id();
    net.rel_mutation(0.025);
    is_alive = true;
    p = b.id;
    t = 0;
}

//bool bird::is_dead(int tube1_x, int tube1_y, int tube2_x, int tube2_y)
void bird::check_and_update_is_alive(int tube1_x, int tube1_y, int tube2_x, int tube2_y)
{
    if (!is_alive)
        return;

    if ((x > tube1_x - 30 && x < tube1_x + 51) && (y > 1680 + tube1_y - 30 || y < tube1_y + 1500)){
        set_dead();
    }
    else if ((x > tube2_x - 30 && x < tube2_x + 51) && (y > 1680 + tube2_y - 30 || y < tube2_y + 1500)){
        set_dead();
    }
    else if (y > 767) {
        set_dead();
    }
}

bool bird::need_to_jump(int X, int Y)
{
//    net.neu[0].value = norm(X - x, -50, 250); //50 - 100 350 - 100
//    net.neu[1].value = norm2(Y - y, 260, 720);//
//    net.neu[2].value = norm(v, -5, 3);
//    net.neu[3].value = norm2(767 - y, 0, 767);

    inf.dist_to_tube = X - x - 30;
    inf.dist_to_bottom = 767 - y;
    inf.dist_to_hole_top = -(Y - y - 180);
    inf.dist_to_hole_bottom = Y - y - 30;
    inf.dist_to_hole_center = Y - y - 105;

//    net.neu[0].value = X - x;
    net.neu[0].value = Y - y;
//    net.neu[2].value = v - 5;
//    net.neu[3].value = 767 - y;


//    b_log << "==Input: \n"
//          << "x-dist to tube: " << net.neu[0].value << "\n"
//          << "y-dist to tube: " << net.neu[1].value << "\n"
//          << "speed         : " << net.neu[2].value << "\n"
//          << "altitude      : " << net.neu[3].value << "\n";

//    b_log << "==Sinapses: \n";// << net << "\n";
//    b_log << *(net);// <<"\n";
    net.getAns();
    return (net.neu.back().activated_value > 0);
//    b_log << "==Res: " << net->neu.back().activated_value
//          << (need_jump ? " jump!" : "")
//          << "\n\n";
}

void bird::move(bool need_jump)
{
    score += 10;
    if (need_jump) {
        v = 3;
        e = 0;
    }
    net.clear_values();
    y -= v;
    if (e == 20) {
        v -= 1;
        e = 0;
    }
    else {
        e += 1;
    }
}

void bird::set_dead ()
{
    update_stats();
    is_alive = false;
}

void bird::update_stats ()
{
    stat.epoches_count++;
    stat.max_score = std::max (stat.max_score, score);
    stat.min_score = std::min (stat.min_score, score);
    stat.sum_score += score;
}
