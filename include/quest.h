#ifndef __QUEST_H__
#define __QUEST_H__
#include "gfc_types.h"

enum Quests_E {
	QUEST_JUMP,
	QUEST_FLOATER,
	QUEST_TALK
};

void quest_init(const char* filename);

void add_quest_progress(int quest, int add);
void set_quest_progress(int quest, int set);

int get_quest_progress(int quest);

void quest_interact_jump();
void quest_interact_floater();
void quest_interact_talk1();
void quest_interact_talk2();


#endif