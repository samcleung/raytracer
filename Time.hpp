#pragma once

class Time {
    static float t;
public:
	static float get_time() { return t; }
	static void update_time(float delta) { t += delta; }
};