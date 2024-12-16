#include "ItemDatabase.hh"

int main() {
	ItemDatabase::get().serialize();
	ItemDatabase::get().set_custom_items(false);
	ItemDatabase::get().encode(false);

	ItemDatabase::get().serialize();
	ItemDatabase::get().set_custom_items(true);
	ItemDatabase::get().encode(true);
	return 0;
}