#include "scene.h"
#include <random>
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <unordered_set>
#include <map>
#include <set>

std::ofstream dbg__evolution_graph("dumps/evol.gv");
std::unordered_map<int, dbg__bird_stat_t> dbg__birds_stat;
std::set<int> dbg__birds_in_evolution_graph;
std::unordered_map<int, int> dbg__all_birds_links;

void dbg__dump_birds_stats ()
{
//    std::set<std::tuple<int, int>> birds_links;

//    for (int bird_id : dbg__birds_in_evolution_graph)
//    {
//        int cur_id = bird_id;
//        auto it = dbg__all_birds_links.find(bird_id);
//        for (; it != dbg__all_birds_links.end (); it = dbg__all_birds_links.find(cur_id))
//        {
//            int parent_id = it->second;
//            if (parent_id >= 0)
//                birds_links.insert({parent_id, cur_id});
//            else
//                break;
//            cur_id = parent_id;
//        }
//    }

//    for (const auto & [parent_id, id] : birds_links)
//    {
//        dbg__evolution_graph << "  " << parent_id << " -> " << id << "\n";
//        dbg__birds_in_evolution_graph.insert(parent_id);
//        dbg__birds_in_evolution_graph.insert(id);
//    }

//    for (int bird_id : dbg__birds_in_evolution_graph)
//    {
//        if (bird_id < 0)
//            continue;
//        const auto & stat = dbg__birds_stat[bird_id];
////        int epoch_start = stat.epoch_start;
//        int score_scale = 400;
//        int min_score = stat.min_score / score_scale;
//        int max_score = stat.max_score / score_scale;
//        int avg_score = (stat.epoches_count > 0 ? stat.sum_score / stat.epoches_count / score_scale : min_score);
////        int epoches_count = stat.epoches_count;

//        dbg__evolution_graph << "  " << bird_id
//                             << " [shape=record,label=\"{"
//                             << bird_id << "|"
//                             << "{" << stat.epoch_start << "|" << stat.epoches_count << "}|"
//                             << "{" << min_score << "|" << avg_score << "|" << max_score << "}"
//                             << "}\"]\n";
//    }
}

//void Scene::dbg__add_bird_to_stats (int ind)
//{
//}

//std::mt19937 gen2(std::chrono::duration_cast<std::chrono::nanoseconds>((std::chrono::high_resolution_clock::now()).time_since_epoch()).count());
//std::mt19937 gen2(137);
std::default_random_engine gen2(137);

Scene::Scene(const std::vector<bird> birds)
    : m_birds(birds)
{}


void Scene::run()
{
    //dbg__evolution_graph << "digraph g {\n";
    //unsigned int k = 1200000;
    while (true) {
        do_smth();
    }
    for (const auto & b : m_birds)
        dbg__birds_stat[b.id] = b.stat;
    dbg__dump_birds_stats();
    //dbg__evolution_graph << "}\n";
}

double random1(double min, double max)
{
    std::uniform_real_distribution<> r(min, max);
    double result = r(gen2);
    return result;
}

double random2(double c, double sigm)
{
    std::normal_distribution<double> r(c, sigm);
    double result = r(gen2);
    return result;
}
int random(int min, int max)
{
    std::uniform_int_distribution<> r(min, max);
    return r(gen2);
}
void Scene::move_tubes()
{
    --tube1_x;
    if (tube1_x < -51) {
        tube1_x = 549;
        tube1_y = random(50, 510) - 1470;
    }
    --tube2_x;
    if (tube2_x < -51) {
        tube2_x = 549;
        tube2_y = random(50, 510) - 1470;
    }
}

void Scene::find_nearest_tube()
{
    if (tube1_x < 350 && tube1_x > 50) {
        X = tube1_x;
        Y = 1680 + tube1_y;
    }
    else {
        X = tube2_x;
        Y = 1680 + tube2_y;
    }
}
void Scene::check_lives()
{
    for (auto & x: m_birds) {
        x.check_and_update_is_alive(tube1_x, tube1_y, tube2_x, tube2_y);
    }
}
std::ofstream pr_out("pr_tree");

bool Scene::all_dead()
{
    size_t k = 0;
    while (k < m_birds.size() && m_birds[k].is_alive == 0)
        ++k;
    if (k == m_birds.size())
        return 1;
    return 0;
}

void Scene::dbg__dump()
{
    for (auto x: m_birds) {
        pr_out << x.p << "(" << x.score << ", " << x.t << ") ";
    }
    pr_out << std::endl;
}

void Scene::mutations()
{
    size_t best_birds_count = 2 * m_birds.size() / 10;
    size_t good_birds_count = 8 * m_birds.size() / 10;

//    // MK_DEBUG
//    best_birds_count = 1;
//    good_birds_count = 2;
//    if (m_birds[0].score == 1700)
//        best_birds_count = good_birds_count = 0;

    // сохраняются
    for (size_t i = 0; i < best_birds_count; ++i) {
        ++m_birds[i].t;
    }

    // мутируют из лучших
    size_t ind_of_best_bird = 0;
    for (size_t i = best_birds_count; i < good_birds_count; ++i) {
        const auto & parent = m_birds[ind_of_best_bird];
        ++ind_of_best_bird;
        if (ind_of_best_bird >= best_birds_count) ind_of_best_bird = 0;
        int old_bird_id = m_birds[i].id;

        dbg__birds_stat[old_bird_id] = m_birds[i].stat;
        auto dbg__epoches_count = m_birds[i].stat.epoches_count;
        if (dbg__epoches_count > 2)
        {
            dbg__birds_in_evolution_graph.insert(m_birds[i].p);
            dbg__birds_in_evolution_graph.insert(m_birds[i].id);
        }


        m_birds[i].mutate_from(parent);
        m_birds[i].stat.epoch_start = gen_num;
        dbg__all_birds_links.insert({m_birds[i].id, parent.id});
//        dbg__evolution_graph << "  " << parent.id << " [label = \"" << parent.score << "\"]\n";
    }

    // генерируются абсолютно новые птицы
    for (size_t i = good_birds_count; i < m_birds.size(); ++i) {
        dbg__birds_stat[m_birds[i].id] = m_birds[i].stat;
        m_birds[i] = bird(gen_num);
    }
}
void Scene::reset()
{
    for (size_t i = 0; i < m_birds.size(); ++i) {
        m_birds[i].reset_position();
        m_birds[i].score = 0;
    }
    tube1_x = 0;
    tube2_x = 300;
    tube1_y = -1200;
    tube2_y = -1300;
}

void Scene::new_gen()
{
    cur_score = 0;
    gen_num++;
    sort(m_birds.rbegin(), m_birds.rend(), [](const bird & a, const bird & b)
    {
        return a.score < b.score;
    });
    max_score = std::max(max_score, m_birds[0].score);
    std::cout << gen_num << ": " << m_birds[0].score << "(" << max_score << ")\n";
    std::cout.flush();
    mutations();
    dbg__dump();
    reset();
}


void Scene::do_smth()
{

    move_tubes();
    find_nearest_tube();
    check_lives();
    for (auto & x : m_birds) {
        if (x.is_alive) {
            bool need_jump = x.need_to_jump(X, Y);
            x.move(need_jump);
            cur_score = std::max(cur_score, x.score);
        }
    }
    if (all_dead()) {
        new_gen();
    }
}
