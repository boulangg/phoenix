#pragma once

#include <sys/types.h>

class CharacterDevice {
public:
	virtual ~CharacterDevice() {

	}

	dev_t getDeviceID() {
		return devID;
	}

	void setDeviceID(dev_t ID) {
		devID = ID;
	}

	virtual ssize_t read(char*, size_t, loff_t) {
		return 0;
	}
	virtual ssize_t write(const char *, size_t, loff_t) {
		return 0;
	}

private:
	dev_t devID;
};
