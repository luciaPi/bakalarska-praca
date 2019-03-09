#include "Dataset.h"
#include <vector>

Dataset::~Dataset()
{
	while (all.size() > 0) {
		Object* last = all.back();
		delete last;
		all.pop_back();
	}
}

void Dataset::add(Object* object)
{
	all.push_back(object);
	numberOfObjects++;
}

int Dataset::getSize() const
{
	return numberOfObjects;
}

/*Object& Dataset::operator[](const int index)
{
	Object* object = all[index];
	return *object;
}*/

const Object& Dataset::operator[](const int index) const
{
	Object* object = all[index];
	return *object;
}

int Dataset::whichNumberOfObject(const Object & object) const
{
	int which = 0;
	for (Object *actual : all) {
		if (actual == &object) {
			return which;
		}
		which++;
	}
	return which;
}
