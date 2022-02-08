#include "../plugin_sdk/plugin_sdk.hpp"
#include "Varus.h"
//TODO: Learn how to keep pressed a skill.
namespace Varus
{
    //Define the colors that will be used in on_draw()

#define Q_DRAW_COLOR (MAKE_COLOR(245, 40, 145, 0.8))
#define E_DRAW_COLOR (MAKE_COLOR(39,245,208,0.8))
#define R_DRAW_COLOR (MAKE_COLOR(249,29,0,0.98))

    //define Q spell static
#define qmax 1600
#define qmin 850
#define qWidth 55
#define qSpeed 1900
#define qDealy 500
#define qChargeTime 1500

    //DECLARE SPELLS
    script_spell* q = nullptr;
    script_spell* w = nullptr;
    script_spell* e = nullptr;
    script_spell* r = nullptr;

    //declaration of menu objects
    TreeTab* main_tab = nullptr;
    namespace draw_settings
    {
        TreeEntry* draw_range_q = nullptr;
        TreeEntry* draw_damage_w = nullptr;
        TreeEntry* draw_range_e = nullptr;
        TreeEntry* draw_range_r = nullptr;
    }
    namespace combo
    {
        TreeEntry* use_q = nullptr;
        TreeEntry* use_e = nullptr;

        TreeEntry* use_r = nullptr;
        TreeEntry* r_enemy_range = nullptr;

    }
    namespace harass
    {
        TreeEntry* use_q = nullptr;
        TreeEntry* use_e = nullptr;
    }
    namespace laneclear
    {
        TreeEntry* use_q = nullptr;
        TreeEntry* q_kill_minions = nullptr;
        TreeEntry* use_e = nullptr;
        TreeEntry* e_kill_minions = nullptr;
    }
    namespace jungleclear
    {
        TreeEntry* save_q_for_ks = nullptr;
        TreeEntry* use_q = nullptr;
        TreeEntry* use_e = nullptr;
    }

    namespace misc
    {
        TreeEntry* use_q_for_ks = nullptr;
        TreeEntry* use_q_for_drake = nullptr;
        TreeEntry* use_r_for_ks = nullptr;
    }
    namespace gapclose
    {
        TreeEntry* use_e = nullptr;
        TreeEntry* use_r = nullptr;
    }
    //Enum define myhero region
    enum Position
    {
        Line,
        Jungle
    };
    Position my_hero_region;

    void load()
    {
        //Register spells
        q = plugin_sdk->register_spell(spellslot::q, qmax);
    }
    void unload();
};