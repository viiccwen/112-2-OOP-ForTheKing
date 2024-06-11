#include "global.h"
#include "frame.h"
#include <vector>

void PrintWalkFrame() {
	// upper, middle, lower frame
	for (int i = 0; i < GAME_ALL_WIDTH; i++) PrintString(i, 0, "-");
	for (int i = 0; i < GAME_ALL_WIDTH; i++) PrintString(i, MAP_HEIGHT + 1, "-");
	for (int i = 0; i < GAME_ALL_WIDTH; i++) PrintString(i, MAP_HEIGHT + ROLE_INFO_HEIGHT + 2, "-");

	// left, middle, right frame
	for (int i = 1; i <= MAP_HEIGHT; i++) PrintString(0, i, "|");
	for (int i = 1; i <= MAP_HEIGHT; i++) PrintString(MAP_WIDTH + 1, i, "|");
	for (int i = 1; i <= MAP_HEIGHT; i++) PrintString(GAME_ALL_WIDTH - 1, i, "|");

	// player's gap frame
	// vertical
	for (int roleGapIdx = 0; roleGapIdx < 4; roleGapIdx++) {
		for (int i = 21; i <= 35; i++) {
			PrintString(ROLE_INFO_WIDTH * roleGapIdx + roleGapIdx, i, "|");
		}
	}

	// current player status
	PrintString(42, 1, "Turn: ");
	PrintString(42, 2, "Player name: ");
	PrintString(42, 4, "Action Point: ");

	// Helper
	std::string wall = "wall: " + BG_GREY + WALL + CLOSE;
	std::string road = "road: " + BG_YELLOW + FG_BLACK + ROAD + CLOSE;
	std::string shop = "shop: " + BG_BLUE + SHOP + CLOSE;
	std::string _event = "event: " + BG_RED + ENEMY + CLOSE;
	std::vector<std::string> helper = {
	"--------------------------------------Helper-------------------------------------",
	"adjust focus: (A), (D)",
	"confirm:      (Enter)",
	"move:         (W), (A), (S), (D)",
	"end turn:     (P)",
	"open bag:     (I)",
	wall,
	road,
	shop,
	_event
	};
	for (int y = 11, idx = 0; y - 11 < helper.size(); y++, idx++) PrintString(42, y, helper[idx]);
}

void MarkCurrentPlayerFrame(int index) {
	for (int i = 0; i < GAME_ALL_WIDTH; i++) PrintString(i, MAP_HEIGHT + 1, "-");
	for (int i = 0; i < GAME_ALL_WIDTH; i++) PrintString(i, MAP_HEIGHT + ROLE_INFO_HEIGHT + 2, "-");

	for (int roleGapIdx = 0; roleGapIdx < 4; roleGapIdx++) {
		for (int i = 21; i <= 35; i++) {
			PrintString(ROLE_INFO_WIDTH * roleGapIdx + roleGapIdx, i, "|");
		}
	}

	for (int i = ROLE_INFO_WIDTH * (index - 1) + index; i <= ROLE_INFO_WIDTH * index + index; i++) PrintString(i, MAP_HEIGHT + 1, FG_GREEN + "-" + CLOSE);
	for (int i = ROLE_INFO_WIDTH * (index - 1) + index; i <= ROLE_INFO_WIDTH * index + index; i++) PrintString(i, MAP_HEIGHT + ROLE_INFO_HEIGHT + 2, FG_GREEN + "-" + CLOSE);

	for (int i = MAP_HEIGHT + 1; i < GAME_ALL_HEIGHT; i++) PrintString(ROLE_INFO_WIDTH * (index - 1) + (index - 1), i, FG_GREEN + "|" + CLOSE);
	for (int i = MAP_HEIGHT + 1; i < GAME_ALL_HEIGHT; i++) PrintString(ROLE_INFO_WIDTH * index + index, i, FG_GREEN + "|" + CLOSE);
}

void PrintShopFrame() {
	for (int x = 0; x < GAME_ALL_WIDTH; x++) {
		for (int y = 0; y < GAME_ALL_HEIGHT; y++) {
			PrintString(x, y, " ");
		}
	}
	for (int x = 0; x < 50; x++) {
		PrintString(x, 0, "-");
	}
	for (int x = 0; x < 50; x++) {
		PrintString(x, 24, "-");
	}
	for (int y = 0; y < 25; y++) {
		PrintString(0, y, "|");
	}
	for (int y = 0; y < 25; y++) {
		PrintString(49, y, "|");
	}
}

void PrintBagSpace() {
	for (int x = 1; x < GAME_ALL_WIDTH - 1; x++) {
		for (int y = 1; y <= MAP_HEIGHT; y++) {
			PrintString(x, y, " ");
		}
	}
}

void PrintCombatEventSpace() {
	for (int x = 1; x < GAME_ALL_WIDTH - 1; x++) {
		for (int y = 1; y <= MAP_HEIGHT; y++) {
			PrintString(x, y, " ");
		}
	}
}

void PrintUseItemFrame() {
	for (int x = 0; x < GAME_ALL_WIDTH; x++) {
		for (int y = 0; y < GAME_ALL_HEIGHT; y++) {
			PrintString(x, y, " ");
		}
	}
	for (int x = 0; x < 50; x++) {
		PrintString(x, 0, "-");
	}
	for (int x = 0; x < 50; x++) {
		PrintString(x, 24, "-");
	}
	for (int y = 0; y < 25; y++) {
		PrintString(0, y, "|");
	}
	for (int y = 0; y < 25; y++) {
		PrintString(49, y, "|");
	}
}

void PrintCombatFrame() {
	for (int x = 0; x < GAME_ALL_WIDTH; x++) {
		for (int y = 0; y < GAME_ALL_HEIGHT; y++) {
			PrintString(x, y, " ");
		}
	}
	for (int x = 0; x < 100; x++) {
		PrintString(x, 0, "-");
	}
	for (int x = 0; x < 100; x++) {
		PrintString(x, 24, "-");
	}


	for (int y = 0; y < 25; y++) {
		PrintString(0, y, "|");
	}
	for (int y = 0; y < 25; y++) {
		PrintString(24, y, "|");
	}

	for (int y = 0; y < 25; y++) {
		PrintString(75, y, "|");
	}

	for (int y = 0; y < 25; y++) {
		PrintString(99, y, "|");
	}

	for (int x = 25; x < 75; x++) {
		PrintString(x, 10, "-");
	}
}