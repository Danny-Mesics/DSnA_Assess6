#include "EntityDisplayApp.h"
#include <Windows.h>

EntityDisplayApp::EntityDisplayApp(int screenWidth, int screenHeight) : m_screenWidth(screenWidth), m_screenHeight(screenHeight) {

}

EntityDisplayApp::~EntityDisplayApp() {

}

bool EntityDisplayApp::Startup() {

	InitWindow(m_screenWidth, m_screenHeight, "EntityDisplayApp");
	SetTargetFPS(60);

	

	return true;
}

void EntityDisplayApp::Shutdown() {

	CloseWindow();        // Close window and OpenGL context
}

void EntityDisplayApp::Update(float deltaTime) {

	hGetEntityCount = OpenFileMapping(FILE_MAP_ALL_ACCESS,
		FALSE,
		L"EntityCount");

	if (hGetEntityCount == nullptr) {
		return;
	}


	count = (int*)MapViewOfFile(hGetEntityCount,
		FILE_MAP_ALL_ACCESS,
		0,
		0,
		sizeof(int));

	if (count == nullptr) {
		CloseHandle(hGetEntityCount);
		return;
	}

	hGetEntities = OpenFileMapping(FILE_MAP_ALL_ACCESS,
		FALSE,
		L"Entities");

	if (hGetEntities == nullptr) {
		UnmapViewOfFile(count);
		CloseHandle(hGetEntityCount);
		return;
	}

	entities = (Entity*)MapViewOfFile(hGetEntities,
		FILE_MAP_ALL_ACCESS,
		0,
		0,
		sizeof(Entity) * (*count));

	if (entities == nullptr) {
		UnmapViewOfFile(count);
		CloseHandle(hGetEntityCount);
		CloseHandle(hGetEntities);
		return;
	}

	for (int i = 0; i < *count; i++) {
		m_entities.push_back(entities[i]);
	}

	UnmapViewOfFile(count);
	UnmapViewOfFile(entities);
	CloseHandle(hGetEntityCount);
	CloseHandle(hGetEntities);
}

void EntityDisplayApp::Draw() {
	BeginDrawing();

	ClearBackground(RAYWHITE);

	// draw entities
	for (auto& entity : m_entities) {
		DrawRectanglePro(
			Rectangle{ entity.x, entity.y, entity.size, entity.size }, // rectangle
			Vector2{ entity.size / 2, entity.size / 2 }, // origin
			entity.rotation,
			Color{ entity.r, entity.g, entity.b, 255 });
	}

	// output some text, uses the last used colour
	DrawText("Press ESC to quit", 630, 15, 12, LIGHTGRAY);
	m_entities.clear();
	EndDrawing();
}