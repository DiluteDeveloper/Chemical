#pragma once

template<typename T>
class AccessKey {
	friend T;
	AccessKey() = default;
};
