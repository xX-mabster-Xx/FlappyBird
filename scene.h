#pragma once

#include "bird.h"

#include <vector>
#include <QMainWindow>

class Scene
{
public:
    Scene(std::vector<bird> birds);

    void run();

    void do_smth();
    void move_tubes();
    void find_nearest_tube();
    void check_lives();
    bool all_dead();
    void new_gen();
    void mutations();
    void reset();
    void dbg__dump();
    const std::vector<bird> & getBirds() {return m_birds;}
    const int & get_Tube1x() {return tube1_x;}
    const int & get_Tube1y() {return tube1_y;}
    const int & get_Tube2x() {return tube2_x;}
    const int & get_Tube2y() {return tube2_y;}
    const int & get_cur_score() {return cur_score;}
    const int & get_gen_num() {return gen_num;}
    const int & get_max_score() {return max_score;}

private:
    void dbg__add_bird_to_stats ();

    std::vector<bird> m_birds;

    unsigned int t = 1;

    int tube1_x = 0;
    int tube2_x = 300;
    int tube1_y = -1200;
    int tube2_y = -1300;
    int X;
    int Y;
    int gen_num = 0;
    int max_score = 0;
    int cur_score = 0;
};
