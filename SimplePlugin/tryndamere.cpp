#include "../plugin__sdk/plugin_sdk.hpp"
#include "tryndamere.h"

namespace tryndamere
{
	//define colors draw
#define Q_DRAW_COLOR (MAKE_COLOR(0,249,189,1))
#define W_DRAW_COLOR (MAKE_COLOR(249,0,0,1))
#define E_DRAW_COLOR (MAKE_COLOR(226,249,0,1))
    //TODO:: Define spell constant values
#define e_range 660.f
#define w_range 850.f
    //DECLARE SPELLS
    script_spell* q = nullptr;
    script_spell* w = nullptr;
    script_spell* e = nullptr;
    script_spell* r = nullptr;

    //declaration of menu objects
    TreeTab* main_tab = nullptr;
    namespace draw_settings
    {
        TreeEntry* draw_q_health = nullptr;
        TreeEntry* draw_range_w = nullptr;
        TreeEntry* draw_range_e = nullptr;
    }
    namespace combo
    {
        TreeEntry* use_q = nullptr;
        TreeEntry* use_w = nullptr;
        TreeEntry* use_e = nullptr;

    }
    namespace harass
    {
        TreeEntry* use_q = nullptr;
        TreeEntry* use_e = nullptr;
    }
    namespace laneclear
    {
        TreeEntry* use_q = nullptr;
        TreeEntry* use_e = nullptr;
    }

    namespace ultimate
    {
        TreeEntry* auto_r = nullptr;
    }
    namespace killsteal
    {
        TreeEntry* use_e = nullptr;
    }
    namespace antigapclose
    {
        TreeEntry* use_e = nullptr;
    }
    namespace flee {
        TreeEntry* use_e = nullptr;
        TreeEntry* use_w = nullptr;
    }

    //event handler functions
    void on_update();
    void on_draw();
    void on_before_attack(game_object_script sender, bool* process);

    //define logic
    void q_logic();
    void q_combo_logic();
    void w_logic();
    void e_farm_logic();
    void e_combo_logic();
    void e_flee_logic();
    void r_logic();
    void killsteal_logic();


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
        q = plugin_sdk->register_spell(spellslot::q,0);
        w = plugin_sdk->register_spell(spellslot::w, w_range);
        e = plugin_sdk->register_spell(spellslot::e, e_range);
        r = plugin_sdk->register_spell(spellslot::r, 0);

        //Create a menu
        main_tab = menu->create_tab("Rick Sanchez AIO", "Tryndamere");
        main_tab->set_assigned_texture(myhero->get_square_icon_portrait());
        auto combo = main_tab->add_tab(myhero->get_model() + ".combo", "Combo Settings");
        {
            combo::use_q = combo->add_checkbox(myhero->get_model() + ".comboUseQ", "Use Q", true);
            combo::use_q->set_texture(myhero->get_spell(spellslot::q)->get_icon_texture());

            combo::use_w = combo->add_checkbox(myhero->get_model() + ".comboUseW", "Use W in Team Fight", true);
            combo::use_w->set_texture(myhero->get_spell(spellslot::w)->get_icon_texture());

            combo::use_e = combo->add_checkbox(myhero->get_model() + ".comboUseE", "Use E", true);
            combo::use_e->set_texture(myhero->get_spell(spellslot::e)->get_icon_texture());
        }
        auto harass = main_tab->add_tab(myhero->get_model() + ".harass", "Harass Settings");
        {
            harass::use_q = harass->add_checkbox(myhero->get_model() + ".harassUseQ", "Use Q", true);
            harass::use_q->set_texture(myhero->get_spell(spellslot::q)->get_icon_texture());

            harass::use_e = harass->add_checkbox(myhero->get_model() + ".harassUseE", "Use E", true);
            harass::use_e->set_texture(myhero->get_spell(spellslot::e)->get_icon_texture());
        }
        auto laneclear = main_tab->add_tab(myhero->get_model() + ".laneclear", "laneclear Settings");
        {
            laneclear::use_q = laneclear->add_checkbox(myhero->get_model() + ".laneclearUseQ", "Use Q", true);
            laneclear::use_q->set_texture(myhero->get_spell(spellslot::q)->get_icon_texture());

            laneclear::use_e = laneclear->add_checkbox(myhero->get_model() + ".laneclearUseE", "Use E", true);
            laneclear::use_e->set_texture(myhero->get_spell(spellslot::e)->get_icon_texture());
        }
        auto ultimate = main_tab->add_tab(myhero->get_model() + ".ultimate", "ultimate Settings");
        {
            ultimate::auto_r = ultimate->add_checkbox(myhero->get_model() + ".ultimateAutoR", "Auto R", true);
            ultimate::auto_r->set_texture(myhero->get_spell(spellslot::r)->get_icon_texture());
        }
        auto killsteal = main_tab->add_tab(myhero->get_model()+".killsteal","killsteal Settings");
        {
            killsteal::use_e = killsteal->add_checkbox(myhero->get_model() + ".killstealUseE", "Use E to kill steal", true);
        }
        auto antigapclose = main_tab->add_tab(myhero->get_model() + ".antigapclose", "gapclose Settings");
        {
            antigapclose::use_e = antigapclose->add_checkbox(myhero->get_model() + ".gapcloseUseE", "Use E when enemy gapclose",true);
        }
        auto flee = main_tab->add_tab(myhero->get_model() + ".flee", "flee settings");
        {
            flee::use_e = flee->add_checkbox(myhero->get_model() + ".fleeUseE", "Use E", true);
            flee::use_w = flee->add_checkbox(myhero->get_model() + ".fleeUseW", "Use W if enemy is near", false);
        }
        // To add a new event you need to define a function and call add_calback

        event_handler<events::on_update>::add_callback(on_update);
        event_handler<events::on_draw>::add_callback(on_draw);
        event_handler<events::on_before_attack_orbwalker>::add_callback(on_before_attack);

    }
    void unload() {
        menu->delete_tab(main_tab);
        // Always remove all declared spells
       //
        plugin_sdk->remove_spell(q);
        plugin_sdk->remove_spell(w);
        plugin_sdk->remove_spell(e);
        plugin_sdk->remove_spell(r);
        // VERY important to remove always ALL events
       //
        event_handler<events::on_update>::remove_handler(on_update);
        event_handler<events::on_draw>::remove_handler(on_draw);
        event_handler<events::on_before_attack_orbwalker>::remove_handler(on_before_attack);
    }
    void on_update()
    {
        if (myhero->is_dead() || myhero->is_recalling())
        {
            if (ultimate::auto_r)
            {
                r_logic();
            }
            return;
        }

        if(orbwalker->can_move(0.05f))
        {
	        if(orbwalker->combo_mode())
	        {
		        if(q->is_ready() && combo::use_q->get_bool())
		        {
                    q_combo_logic();
		        }
                if (e->is_ready() && combo::use_e->get_bool())
                {
                    e_combo_logic();
                }
                if(w->is_ready()&& combo::use_w->get_bool())
                {
                    w_logic();
                }
	        }
            if(orbwalker->lane_clear_mode())
            {
	            if(q->is_ready() && laneclear::use_q->get_bool())
	            {
                    q_logic();
	            }
                if(e->is_ready() && laneclear::use_e->get_bool())
                {
                    e_farm_logic();
                }
            }
            if(orbwalker->harass())
            {
	            if(q->is_ready()&& harass::use_q->get_bool())
	            {
                    q_combo_logic();
	            }
                if(e->is_ready()&& harass::use_e->get_bool())
                {
                    e_combo_logic();
                }
            }
            if(orbwalker->flee_mode())
            {
                e_flee_logic();
            }
            if(orbwalker->none_mode())
            {
                q_logic();
                
                
            }
            if (ultimate::auto_r)
            {
                r_logic();
            }
            if(killsteal::use_e)
            {
                killsteal_logic();
            }
        }
    }
    void on_draw()
    {
        if (myhero->is_dead())
        {
            return;
        }
        if(draw_settings::draw_range_e->get_bool())
        {
            draw_manager->add_circle(myhero->get_position(), q->range(), E_DRAW_COLOR);

        }
        if (draw_settings::draw_range_w->get_bool())
        {
            draw_manager->add_circle(myhero->get_position(), q->range(), W_DRAW_COLOR);

        }
    }
    void on_before_attack(game_object_script sender, bool* process)
    {
	    
    }




    //logics of skills
    //COMBO SKILLS
    void q_combo_logic()
    {
        auto dmg_pred = health_prediction->get_incoming_damage(myhero, 1.f, true);
        int health = myhero->get_health();
        health = health - 100;
        if(dmg_pred>= health)
        {
            q->cast();
        }
    }
    void e_combo_logic()
    {
        auto e_target = target_selector->get_target(e->range(), damage_type::physical);
        if(e_target && e_target->is_ai_hero())
        {
            e->cast(e_target, hit_chance::medium);
        }

    }
    //LANE CLEAR
    void e_farm_logic()
    {
        auto position = e->get_cast_on_best_farm_position(1,false);
        if(!position.is_under_enemy_turret())
        {
            if (!e->cast(position))
            {
                e->cast_on_best_farm_position(1, true);
            }
        }
        
	    
    }
    //flee logic

    void e_flee_logic()
    {
        auto e_target = target_selector->get_target(e->range(), damage_type::physical);
        if (e_target && e_target->is_ai_hero())
        {
            e->cast(-e_target->get_position());
        }
    }

    //AUTO LOGIC

    void q_logic()
    {
	    if(myhero->get_health_percent()<20 && myhero->count_enemies_in_range(300)==0)
	    {
            q->cast();
	    }
    }


    void w_logic()
    {
        auto w_target = target_selector->get_target(w->range(), damage_type::physical);
        if (w_target && w_target->is_ai_hero())
        {
            
            auto facing = w_target->is_facing(myhero);
            if (!facing)
            {
                w->cast();
            }
            
        }
    }

    void killsteal_logic()
    {
        auto e_target = target_selector->get_target(e->range(), damage_type::physical);
        auto health_pred = health_prediction->get_health_prediction(e_target,1.f);
        auto e_dmg = e->get_damage(e_target);
        if(health_pred<= e_dmg)
        {
            e->cast(e_target);
        }
    }

    void r_logic()
    {
	    auto dmg_pred = health_prediction->get_incoming_damage(myhero, 1.f, true);
	    int health = myhero->get_health();
	    health = health - 100;
	    if (dmg_pred >= health)
	    {
	        r->cast();
	    }
    }



}