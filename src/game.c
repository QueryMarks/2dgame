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

#include "editor.h"


#include "game.h"

static Game_Manager game_manager = { 0 };



int get_gamestate() {
    return game_manager.gamestate;
}


void game_start_title() {

    game_manager.gamestate = GS_TITLE;
    slog("title start");
    Window* window = window_new();
    Sprite* buttonSprite = gf2d_sprite_load_all("images/floater.png", 64, 64, 1, 1);
    button_new
    (
        window, 
        gfc_vector2d(100, 100), 
        buttonSprite, 
        gfc_vector2d(64, 64),
        onclick_start_level
    );
    slog("made the window moment");



}





void game_start_level(const char* path) {
    //window_new();
    collider_manager_init(1024, false);
    //collider_manager_init(1024, true);
    entity_manager_init(1024);
    window_manager_close();
    game_manager.gamestate = GS_MAIN;

    game_manager.level = level_load(path);
    level_setup_camera_bounds(game_manager.level);

    game_manager.player = player_entity_new(gfc_vector2d(100, 100));
    //enemy_entity_new(gfc_vector2d(400, 100));
    /*enemy_entity_new(gfc_vector2d(800, 300));

    floater_enemy_entity_new(gfc_vector2d(1000, 800));

    turret_enemy_entity_new(gfc_vector2d(1400, 500));
    bruiser_enemy_entity_new(gfc_vector2d(1500, 100));
    */
    wall_entity_new(gfc_vector2d(64 * 31, 64 * 10), EXPLOSIVE, gfc_vector2d(64, 128));
    wall_entity_new(gfc_vector2d(64 * 32, 64 * 10), EXPLOSIVE, gfc_vector2d(64, 128));
    wall_entity_new(gfc_vector2d(64 * 31, 64 * 8), MELEE, gfc_vector2d(64, 128));
    wall_entity_new(gfc_vector2d(64 * 32, 64 * 8), MELEE, gfc_vector2d(64, 128));

    current_entity_new(gfc_vector2d(64 * 33, 64 * 10), gfc_vector2d(0, 1), gfc_vector2d(64, 128));
    current_entity_new(gfc_vector2d(64 * 33, 64 * 8), gfc_vector2d(0, 1), gfc_vector2d(64, 128));

    spike_entity_new(gfc_vector2d(64 * 44, 64 * 12), 30.0f, gfc_vector2d(64, 64));
    tnt_entity_new(gfc_vector2d(64 * 49, 64 * 13));
    tnt_entity_new(gfc_vector2d(64 * 50, 64 * 13));
    tnt_entity_new(gfc_vector2d(64 * 51, 64 * 13));
    tnt_entity_new(gfc_vector2d(64 * 52, 64 * 13));
    damagezone_entity_new(gfc_vector2d(64 * 56, 64 * 10), 0.1f, gfc_vector2d(256, 256));
    speaker_entity_new(gfc_vector2d(64 * 60, 64 * 13));
    slog("loaded entire level");
    window_manager_init(32);



}






void game_title_exit() {
    game_manager.gamestate = GS_MAIN;
}

void game_state_change(int state) {
    game_manager.gamestate = state;
}

void game_to_title() {
    //if (game_manager.gamestate == GS_MAIN) {
        window_manager_close();
        entity_manager_close();
        collider_manager_close();
        window_manager_init(32);
        game_start_title();
    //}
}


void run_maingame()
{
    //ETHELYN think + update all
    slog("running maingame");
    collider_manager_check_collisions();
    entity_manager_think_all();
    entity_manager_update_all();
    window_manager_update_all();
    gf2d_graphics_clear_screen();// clears drawing buffers
    // all drawing should happen betweem clear_screen and next_frame
        //backgrounds drawn first
    level_draw(game_manager.level);
    entity_manager_draw_all();
    collider_manager_draw_all();

    char buffer[32];
    if (game_manager.player->_inuse == 1)
    {
        float temphp = roundf(game_manager.player->health);
        int inttemphp = temphp;
        int weapon = (int)game_manager.player->data;
        sprintf(buffer, "%s%i%s%i", "WEP: ", weapon, "  HP: ", inttemphp);
        write_gui(buffer);
    }
    //write_dialogue("Oh boy! It's dialogue!!!!");
    draw_gui();
    window_manager_draw_all();

    if (game_manager.gamestate == GS_TITLE) {
        game_to_title();
    }
}

void onclick_editor_entity_cycle() {
    editor_change_entity();
}


void game_edit_level(const char* path) {
    editor_init();
    collider_manager_init(1024, false);
    //collider_manager_init(1024, true);
    entity_manager_init(1024);
    window_manager_close();
    game_manager.gamestate = GS_EDITOR;
    window_manager_init(32);
    game_manager.level = level_load(path);
    level_setup_camera_bounds(game_manager.level);
    editor_init(game_manager.level);
    game_manager.player = player_entity_new(game_manager.level->playerSpawn);
    
    
    
    camera_enable_binding(0);
}
void game_start_editor() {
    slog("started editor");
    game_edit_level("maps/template.json");
}



void game_run_editor() {
    gf2d_graphics_clear_screen();
    level_draw(game_manager.level);
    window_manager_update_all();
    collider_manager_draw_all();
    entity_manager_draw_all();
    window_manager_draw_all();

    if (game_manager.gamestate == GS_MAIN)
    {
        game_start_level("maps/testlevel.json");
    }

    int camera_speed = 5;
    GFC_Vector2D new_camera_position = camera_get_position();
    if (gfc_input_key_down("s")) {
        new_camera_position.y += camera_speed;
    }
    if (gfc_input_key_down("w"))
    {
        new_camera_position.y -= camera_speed;
    }
    if (gfc_input_key_down("d")) {
        new_camera_position.x += camera_speed;
    }
    if (gfc_input_key_down("a"))
    {
        new_camera_position.x -= camera_speed;
    }

    int mx, my;
    SDL_GetMouseState(&mx, &my);
    //add menu stuff here, before we add the offset for placing stuff



    mx -= camera_get_offset().x;
    my -= camera_get_offset().y;
    if (gfc_input_key_pressed("1")) {
        editor_place_entity(gfc_vector2d(mx, my));
    }
    if (gfc_input_key_pressed("2")) {
        int tile_x = mx / 64;
        int tile_y = my / 64;
        if (tile_x >= 0
            && tile_x < game_manager.level->tileMapWidth
            && tile_y >= 0
            && tile_y < game_manager.level->tileMapHeight)
        {
            editor_place_tile(tile_x, tile_y);
        }
        
    }
    if (gfc_input_key_pressed("p"))
    {
        game_manager.player->position.x = mx;
        game_manager.player->position.y = my;
    }

    camera_set_position(new_camera_position);


    gfc_input_update();
    const Uint8* keys;
    keys = SDL_GetKeyboardState(NULL);
    if (keys[SDL_SCANCODE_RETURN])level_save(game_manager.level);
}


void game_run_menu() {
    gf2d_graphics_clear_screen();
    window_manager_update_all();
    window_manager_draw_all();
    if (game_manager.gamestate == GS_MAIN)
    {
        game_start_level("maps/testlevel.json");
    }
    gfc_input_update();
    const Uint8* keys;
    keys = SDL_GetKeyboardState(NULL);
    if (keys[SDL_SCANCODE_SPACE])game_start_editor();
}

int main(int argc, char * argv[])
{
    /*variable declarations*/
    int done = 0;
    
    
    int mx,my;
    float mf = 0;
    int time = 0;
    Sprite *mouse;
    Sprite* GUI;
    GFC_Color mouseGFC_Color = gfc_color8(255,120,180,255);

    
    
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
    
    SDL_ShowCursor(SDL_DISABLE);
    camera_set_size(gfc_vector2d(1200, 720));
    gfc_input_init("config/input.cfg");
    window_manager_init(32);
    mouse = gf2d_sprite_load_all("images/pointer2.png", 32, 32, 16, 0);
    /*demo setup*/
    //start_maingame();
    game_start_title();

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
        
        gfc_input_update();
        const Uint8* keys;
        keys = SDL_GetKeyboardState(NULL);


        if (game_manager.gamestate == GS_MAIN)
        {
            run_maingame();
        }
        else if (game_manager.gamestate == GS_TITLE)
        {
            game_run_menu();
        }
        else  if (game_manager.gamestate == GS_EDITOR)
        {
            game_run_editor();
        }



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
    entity_manager_close();
    level_free(game_manager.level);
    slog("---==== END ====---");
    return 0;
}



/*eol@eof*/
