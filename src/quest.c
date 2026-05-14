#include "quest.h"
#include "simple_json.h"
#include "simple_logger.h"
#include "entity.h"
#include "eventer.h"
#include "window.h"
#include "element.h"
#include "audio.h"

typedef struct Quest_Tracker_S {
	int quests_max;
	Uint8* quest_progress;
}Quest_Tracker;

static Quest_Tracker _quest_tracker = { 0 };


void quest_init(const char* filename) {
	SJson* json = sj_load(filename);
	if (json != NULL) {
		SJson* save = sj_object_get_value(json, "save");
		SJson* quests = sj_object_get_value(save, "quests");
		
		_quest_tracker.quests_max = sj_array_get_count(quests);
		_quest_tracker.quest_progress = gfc_allocate_array(sizeof(Uint8), 3);

		for (int i = 0; i < _quest_tracker.quests_max; i++)
		{
			int quest_val = -1;
			SJson* quest_json = sj_array_get_nth(quests, i);
			sj_get_integer_value(quest_json, &quest_val);
			_quest_tracker.quest_progress[i] = quest_val;
		}
	}
	else {
		slog("no json found");
	}
}

void add_quest_progress(int quest, int add)
{
	if (_quest_tracker.quests_max != 0) {
		_quest_tracker.quest_progress[quest] += add;
	}
}
void set_quest_progress(int quest, int set)
{
	if (_quest_tracker.quests_max != 0) {
		_quest_tracker.quest_progress[quest] = set;
	}
}

int get_quest_progress(int quest)
{
	if (_quest_tracker.quests_max != 0)
	{
		return _quest_tracker.quest_progress[quest];
	}
}

void quest_interact_jump(Entity* entity) {
	EventerData* eventer = entity->data;
	int num = eventer->num;
	if (_quest_tracker.quests_max != 0 && eventer->num == 0)
	{
		if (_quest_tracker.quest_progress[QUEST_JUMP] == 0)
		{
			audio_sfx_play("audio/sfx/checkoutmyquest.wav");
			Window* window = window_new();
			Sprite* buttonSprite = gf2d_sprite_load_all("images/checkbox.png", 64, 64, 1, 1);
			button_new
			(
				window,
				gfc_vector2d(100, 100),
				buttonSprite,
				gfc_vector2d(64, 64),
				onclick_close
			);
			text_new(
				window,
				gfc_vector2d(200, 100),
				gfc_vector2d(600, 30),
				"you look agile. jump 10 times!"
			);
			add_quest_progress(QUEST_JUMP, 1);
		}
		else if (_quest_tracker.quest_progress[QUEST_JUMP] < 11)
		{
		}
		else if (_quest_tracker.quest_progress[QUEST_JUMP] == 11)
		{
			audio_sfx_play("audio/sfx/ohmyquest.wav");
			Window* window = window_new();
			Sprite* buttonSprite = gf2d_sprite_load_all("images/checkbox.png", 64, 64, 1, 1);
			button_new
			(
				window,
				gfc_vector2d(100, 100),
				buttonSprite,
				gfc_vector2d(64, 64),
				onclick_close
			);
			text_new(
				window,
				gfc_vector2d(200, 100),
				gfc_vector2d(600, 30),
				"wow! impressive jumps friend"
			);
			set_quest_progress(QUEST_JUMP, 12);
		}
	}
	
}


void quest_interact_floater(Entity* entity) {
	EventerData* eventer = entity->data;
	int num = eventer->num;
	if (_quest_tracker.quests_max != 0 && eventer->num == 0)
	{
		if (_quest_tracker.quest_progress[QUEST_FLOATER] == 0)
		{
			audio_sfx_play("audio/sfx/checkoutmyquest.wav");
			Window* window = window_new();
			Sprite* buttonSprite = gf2d_sprite_load_all("images/checkbox.png", 64, 64, 1, 1);
			button_new
			(
				window,
				gfc_vector2d(100, 100),
				buttonSprite,
				gfc_vector2d(64, 64),
				onclick_close
			);
			text_new(
				window,
				gfc_vector2d(200, 100),
				gfc_vector2d(600, 30),
				"blue floaty guys... defeat one..."
			);
			set_quest_progress(QUEST_FLOATER, 1);
		}
		else if (_quest_tracker.quest_progress[QUEST_FLOATER] < 2)
		{
		}
		else if (_quest_tracker.quest_progress[QUEST_FLOATER] == 2)
		{
			audio_sfx_play("audio/sfx/ohmyquest.wav");
			Window* window = window_new();
			Sprite* buttonSprite = gf2d_sprite_load_all("images/checkbox.png", 64, 64, 1, 1);
			button_new
			(
				window,
				gfc_vector2d(100, 100),
				buttonSprite,
				gfc_vector2d(64, 64),
				onclick_close
			);
			text_new(
				window,
				gfc_vector2d(200, 100),
				gfc_vector2d(600, 30),
				"the world is a better place"
			);
			set_quest_progress(QUEST_FLOATER, 3);
		}
	}

}


void quest_interact_talk1(Entity* entity) {
	EventerData* eventer = entity->data;
	int num = eventer->num;
	if (_quest_tracker.quests_max != 0 && eventer->num == 0)
	{
		if (_quest_tracker.quest_progress[QUEST_TALK] <= 0)
		{
			audio_sfx_play("audio/sfx/checkoutmyquest.wav");
			Window* window = window_new();
			Sprite* buttonSprite = gf2d_sprite_load_all("images/checkbox.png", 64, 64, 1, 1);
			button_new
			(
				window,
				gfc_vector2d(100, 100),
				buttonSprite,
				gfc_vector2d(64, 64),
				onclick_close
			);
			text_new(
				window,
				gfc_vector2d(200, 100),
				gfc_vector2d(600, 30),
				"can you talk to my scary red friend? tyvm"
			);
			set_quest_progress(QUEST_TALK, 1);
		}
		else if (_quest_tracker.quest_progress[QUEST_TALK] < 2)
		{
		}
		else if (_quest_tracker.quest_progress[QUEST_TALK] == 2)
		{
			audio_sfx_play("audio/sfx/ohmyquest.wav");
			Window* window = window_new();
			Sprite* buttonSprite = gf2d_sprite_load_all("images/checkbox.png", 64, 64, 1, 1);
			button_new
			(
				window,
				gfc_vector2d(100, 100),
				buttonSprite,
				gfc_vector2d(64, 64),
				onclick_close
			);
			text_new(
				window,
				gfc_vector2d(200, 100),
				gfc_vector2d(600, 30),
				"he called me WHAT????"
			);
			set_quest_progress(QUEST_TALK, 3);
		}
	}

}

void quest_interact_talk2(Entity* entity) {
	EventerData* eventer = entity->data;
	int num = eventer->num;
	if (_quest_tracker.quests_max != 0 && eventer->num == 0)
	{
		if (_quest_tracker.quest_progress[QUEST_TALK] == 0)
		{
			audio_sfx_play("audio/sfx/checkoutmyquest.wav");
			Window* window = window_new();
			Sprite* buttonSprite = gf2d_sprite_load_all("images/checkbox.png", 64, 64, 1, 1);
			button_new
			(
				window,
				gfc_vector2d(100, 100),
				buttonSprite,
				gfc_vector2d(64, 64),
				onclick_close
			);
			text_new(
				window,
				gfc_vector2d(200, 100),
				gfc_vector2d(600, 30),
				"don't talk to me. i'm... i'm thinking"
			);
			set_quest_progress(QUEST_TALK, -1);
		}
		else if (_quest_tracker.quest_progress[QUEST_TALK] == 1)
		{
			audio_sfx_play("audio/sfx/nincompoop.wav");
			Window* window = window_new();
			Sprite* buttonSprite = gf2d_sprite_load_all("images/checkbox.png", 64, 64, 1, 1);
			button_new
			(
				window,
				gfc_vector2d(100, 100),
				buttonSprite,
				gfc_vector2d(64, 64),
				onclick_close
			);
			text_new(
				window,
				gfc_vector2d(200, 100),
				gfc_vector2d(600, 30),
				"i care not for the quest giver..."
			);
			set_quest_progress(QUEST_TALK, 2);
		}
	}

}