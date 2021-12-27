#pragma once

enum screens {
	SCENE_INTRO,
	SCENE_MAIN,
	SCENE_MAIN_MENU,
	SCENE_GAME,
};

void change_scene(short curr_scene, short next_scene);
