#pragma once
#include<string>
class Buff {
private:
	std::string name;
protected:
	float timer;
public:
	Buff(float time, std::string name) :timer(time), name(name) {};
	virtual void update(float time);
	virtual void gain(float duration);
	const std::string& getName() const { return name; };
	bool inEffect() const { return timer != 0; };
};


class coldDown :public Buff {
private:
	float cd;
public:
	coldDown(float cd) :Buff(0, "coldDown"), cd(cd) {};
	void reset() { Buff::gain(cd); };
	bool onColdDown() const { return inEffect(); };
};

class Skill {
private:
	coldDown cd;
public:
	Skill(float cd) :cd(cd) {};
	void update(float time) { cd.update(time); };
	virtual int activate() {
		if (cd.onColdDown()) {
			return -1;//activation failed due to colddown
		}
		else {
			cd.reset();
		}
	};
};


class BuffSkill :public Skill {
private:
	Buff buff;
	float buffDuration;
public:
	BuffSkill(float coldDown, float buffDuration, std::string buffName) :Skill(coldDown), buff(0, buffName), buffDuration(buffDuration) {};

	int activate() {
		if (Skill::activate() == -1) {
			return -1;
		}
		buff.gain(buffDuration);
		return 0;//successfully activated
	}
	bool inEffect() const {
		return buff.inEffect();
	}
	void update(float time) { Skill::update(time); buff.update(time); }
	const std::string& getSkillName() const { return buff.getName(); };
};
