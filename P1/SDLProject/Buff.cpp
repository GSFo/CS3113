#include "Buff.h"

void Buff::update(float time) {
	if (timer > 0) {
		timer -= time;
		if (timer < 0) {
			timer = 0;
		}
	}
}

void Buff::gain(float duration) {
	timer = duration;
}