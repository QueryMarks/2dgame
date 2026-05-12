#include <SDL.h>
#include "simple_logger.h"

#include "gf2d_graphics.h"
#include "gf2d_sprite.h"
#include "gfc_input.h"

#include "entity.h"
#include "player.h"
#include "enemy.h"
#include "level.h"
#include "camera.h"
#include "collider.h"
#include "shot.h"
#include "wall.h"
#include "current.h"
#include "gui.h"
#include "spike.h"
#include "tnt.h"
#include "damagezone.h"
#include "speaker.h"


#include "window.h"
#include "element.h"


#include "game.h"

static Game_Manager game_manager = { 0 };

int main(int argc, char * argv[])
{
    /*variable declarations*/
    int done = 0;
    const Uint8 * keys;
    
    int mx,my;
    float mf = 0;
    int time = 0;
    Sprite *mouse;
    Sprite* GUI;
    GFC_Color mouseGFC_Color = gfc_color8(255,120,180,255);

    game_manager.gamestate = TITLE;
    
    
    /*program initializtion*/
    init_logger("gf2d.log",0);
    slog("---==== BEGIN ====---");
    gf2d_graphics_initialize(
        "gf2d",
        1200,
        720,
        1200,
        720,
        gfc_vector4d(0,0,0,255),
        0);
    gf2d_graphics_set_frame_delay(16);
    gf2d_sprite_init(1024);
    collider_manager_init(1024, false);
    //collider_manager_init(1024, true);
	entity_manager_init(1024);
    SDL_ShowCursor(SDL_DISABLE);
    camera_set_size(gfc_vector2d(1200, 720));
    gfc_input_init("config/input.cfg");
    window_manager_init(32);
    window_new();
    /*demo setup*/
    level = level_load("maps/testlevel.json");
    level_setup_camera_bounds(level);
    mouse = gf2d_sprite_load_all("images/pointer2.png",32,32,16,0);
    player = player_entity_new(gfc_vector2d(100,100));
    //enemy_entity_new(gfc_vector2d(400, 100));
    /*enemy_entity_new(gfc_vector2d(800, 300));

    floater_enemy_entity_new(gfc_vector2d(1000, 800));

    turret_enemy_entity_new(gfc_vector2d(1400, 500));
    bruiser_enemy_entity_new(gfc_vector2d(1500, 100));
    */
    wall_entity_new(gfc_vector2d(64 * 31, 64 * 10), EXPLOSIVE, gfc_vector2d(64, 128));
    wall_entity_new(gfc_vector2d(64 * 32, 64 * 10), EXPLOSIVE, gfc_vector2d(64, 128));
    wall_entity_new(gfc_vector2d(64 * 31, 64 * 8), MELEE, gfc_vector2d(64, 128));
    wall_entity_new(gfc_vector2d(64 * 32, 64 * 8),MELEE,gfc_vector2d(64,128));

    current_entity_new(gfc_vector2d(64 * 33, 64 * 10), gfc_vector2d(0, 1), gfc_vector2d(64, 128));
    current_entity_new(gfc_vector2d(64 * 33, 64 * 8), gfc_vector2d(0, 1), gfc_vector2d(64, 128));

    spike_entity_new(gfc_vector2d(64 * 44, 64 * 12), 30.0f, gfc_vector2d(64, 64));
    tnt_entity_new(gfc_vector2d(64 * 49, 64 * 13));
    tnt_entity_new(gfc_vector2d(64 * 50, 64 * 13));
    tnt_entity_new(gfc_vector2d(64 * 51, 64 * 13));
    tnt_entity_new(gfc_vector2d(64 * 52, 64 * 13));
    damagezone_entity_new(gfc_vector2d(64 * 56, 64 * 10), 0.1f, gfc_vector2d(256, 256));
    speaker_entity_new(gfc_vector2d(64 * 60, 64 * 13));


    //enemy_entity_new(gfc_vector2d(950, 700));
    slog("press [escape] to quit");
    /*main game loop*/
    while(!done)
    {
        //SDL_PumpEvents();   // update SDL's internal event structures
         // get the keyboard state for this frame
        /*update things here*/
        SDL_GetMouseState(&mx,&my);
        mf+=0.1;
        if (mf >= 16.0)mf = 0;
        //ETHELYN think + update all
        gfc_input_update();
        keys = SDL_GetKeyboardState(NULL);
        collider_manager_check_collisions();
        entity_manager_think_all();
        entity_manager_update_all();
        window_manager_update_all();
        gf2d_graphics_clear_screen();// clears drawing buffers
        // all drawing should happen betweem clear_screen and next_frame
            //backgrounds drawn first
        level_draw(level);
		entity_manager_draw_all();
        collider_manager_draw_all();
        
        char buffer[32];
        if (player->_inuse == 1)
        {
            float temphp = roundf(player->health);
            int inttemphp = temphp;
            int weapon = (int)player->data;
            sprintf(buffer, "%s%i%s%i", "WEP: ", weapon, "  HP: ", inttemphp);
            write_gui(buffer);
        }
        //write_dialogue("Oh boy! It's dialogue!!!!");
        draw_gui();
        window_manager_draw_all();
        //UI elements last
        gf2d_sprite_draw(
            mouse,
            gfc_vector2d(mx,my),
            NULL,
            NULL,
            NULL,
            NULL,
            &mouseGFC_Color,
            (int)mf);

        gf2d_graphics_next_frame();// render current draw frame and skip to the next frame
        time += 1;
        if (keys[SDL_SCANCODE_ESCAPE])done = 1;
        // exit condition
        //slog("Rendering at %f FPS",gf2d_graphics_get_frames_per_second());
    }
    entity_free(player);
    level_free(level);
    slog("---==== END ====---");
    return 0;
}



/*eol@eof*/
